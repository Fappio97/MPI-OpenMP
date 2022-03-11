#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define size 10
  
int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  
// array temporaneo per gli slaves
int a2[1000];
  
int main(int argc, char* argv[])
{
  
    int myId, numeroProcessi, elementiPerProcesso, elementiRicevuti;
  
    MPI_Status status;
  
    // creazione di processi paralleli
    MPI_Init(&argc, &argv);
  
    // cerca gli id dei processi
    // quanti processi sono stati avviati
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);
    MPI_Comm_size(MPI_COMM_WORLD, &numeroProcessi);
  
    // master
    if (myId == 0) {
        int indice, i;
        elementiPerProcesso = size / numeroProcessi;
  
        // se ci sono più di un processo
        if (numeroProcessi > 1) {
            // distribuisce le porzioni degli array
            // ai processi e calcola
            // la loro somma parziale
            for (i = 1; i < numeroProcessi - 1; i++) {
                indice = i * elementiPerProcesso;
  
                MPI_Send(&elementiPerProcesso, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&a[indice], elementiPerProcesso, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
  
            // l'ultimo processo aggiunge gli elementi finali
            indice = i * elementiPerProcesso;
            int elementiRimasti = size - indice;
  
            MPI_Send(&elementiRimasti, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&a[indice], elementiRimasti, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
  
        // il master fa la sua somma locale
        int somma = 0;
        for (i = 0; i < elementiPerProcesso; i++)
            somma += a[i];
  
        // prendo le varie somme locali e le sommo
        int tmp;
        for (i = 1; i < numeroProcessi; i++) {
            MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            int sorgente = status.MPI_SOURCE;
            somma += tmp;
        }
  
        // stampo il valore finale
        printf("Somma dell'array : %d\n", somma);
    }
    // i processi degli slave
    else {
        MPI_Recv(&elementiRicevuti, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  
        // li salva in un array secondario
        MPI_Recv(&a2, elementiRicevuti, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  
        // calcola la somma parziale
        int sommaParziale = 0;
        for (int i = 0; i < elementiRicevuti; i++)
            sommaParziale += a2[i];
  
        // invia le somme parziali al master
        MPI_Send(&sommaParziale, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  
    // elimino tutti gli MPI stati prima di chiudere
    MPI_Finalize();
  
    return 0;
}
