#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10

#define percentualePianteIniziali 0.75

#define vuoto 0
#define germoglio 1
#define pianta 2
#define albero 3
#define germoglioSecco 4
#define piantaSecca 5
#define alberoSecco 6
#define germoglioInfetto 7
#define piantaInfetta 8
#define alberoInfetto 9

#define for_i for(int i = 0; i < SIZE; ++i)
#define for_ij for_i for(int j = 0; j < SIZE; ++j)

void convertiMatriceInVettore(int** matrice, int* vettore) {
	int k;
	for(int i = 0; i < SIZE; ++i) {
		for(int j = 0; j < SIZE; ++j) {
			k = j * SIZE + i;
			vettore[k] = matrice[i][j];
		}
	}
}

void generaCasualmenteInizio(int** terreno) {
    unsigned int seed = time(NULL);
    for_ij {
        double random = (double)rand_r(&seed) / RAND_MAX;
        if (random < percentualePianteIniziali) {
            int r1 = rand_r(&seed)%3 +1;
            terreno[i][j] = r1;
        }
        else
            terreno[i][j] = vuoto;
    }

 /*   for_i
        printf("numero random %f\n", rand() / double(RAND_MAX));*/
}

int main(int argc, char* argv[]) {

	int myID, processi;

	// creo la matrice dinamicamente	
	int** matrice = (int**) malloc(SIZE* sizeof(int*));
	for(int i = 0; i < SIZE; ++i)
		matrice[i] = (int*) malloc(SIZE* sizeof(int));
		
	int dimVettore = SIZE * SIZE;
	
	// creo un vettore in cui linearizzo la matrice dinamica
	int* vettore = (int*) malloc(dimVettore* sizeof(int));
	// creo un vettoreLocale dove invierò i vari valori ai processori
	int* vettoreLocale;
		
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &processi);
	MPI_Comm_rank(MPI_COMM_WORLD, &myID);
	
	MPI_Status status;    
	MPI_Request request;
	
	// numero elementi che devono essere ripartiti fra i processori
	int elementiPerProcesso = dimVettore / processi;
	
	int resto;
	
	// la dimensione del vettoreLocale è pari agli elementi per processo + 2 volte la size delle colonne, che rappresentano le colonne ghost
	int dimVettoreLocale = elementiPerProcesso + (2 * SIZE);
	
	vettoreLocale = (int*) malloc(dimVettoreLocale* sizeof(int));
	
	// Creazione datatype, è continuo in quanto le colonne sul vettore
	// sono scandite linearmente
	MPI_Datatype datatype;
	MPI_Type_contiguous(SIZE, MPI_INT, &datatype);
    MPI_Type_commit(&datatype);
	
	if(myID == 0) {
//		generaCasualmenteInizio(matrice);
		for_ij
			matrice[i][j] = j;
		convertiMatriceInVettore(matrice, vettore);
/*		for(int i = 0; i < dimVettore; ++i)
			printf("%d ", vettore[i]);
		printf("\n");*/
/*		for(int i = 1; i < processi; ++i)
			MPI_Isend(&vettore[elementiPerProcesso * i], 1, datatype, i, i, MPI_COMM_WORLD, &request);*/
	} 
/*	else {
//		MPI_Irecv(&vettoreLocale[SIZE], 1, datatype, 0, myID, MPI_COMM_WORLD, &request);
		for(int i = elementiPerProcesso; i < dimVettoreLocale; ++i)
			vettoreLocale[i] = 0;
		for(int i = 0; i < SIZE; ++i)
			vettoreLocale[i] = 0;
		MPI_Wait(&request, &status);
		for(int i = 0; i < dimVettoreLocale; ++i)
			printf("%d ", vettoreLocale[i]);
		printf("\n");
	}*/
	for(int i = 0; i < dimVettoreLocale; ++i)
		vettoreLocale[i] = 0;
	
	// invio ad ogni processo, sul loro vettoreLocale, le colonne su cui
	// devono lavorare
	MPI_Scatter(vettore, elementiPerProcesso, MPI_INT, &vettoreLocale[SIZE], elementiPerProcesso, MPI_INT, 0, MPI_COMM_WORLD);
	
	for(int i = 0; i < dimVettoreLocale; ++i)
		printf("%d ", vettoreLocale[i]);
	printf("\n");
	
	
	int dimensione[1],cordinate[1];
	dimensione[0] = 0, cordinate[0] = 0;
	int myIDcartesiano;
	MPI_Comm comm1D;
	// creo una tipologia cartesiana
	int periodicita[1];
	periodicita[0] = 0;  
	// periodicità false, non mi serve nel mio automa
	int riordina = 1;
	MPI_Dims_create(processi, 1, dimensione);
	MPI_Cart_create(MPI_COMM_WORLD, 1, dimensione, periodicita, riordina, &comm1D);
	// cerco le cordinate di ogni processo sulla topologia
	MPI_Cart_coords(comm1D, myID, 1, cordinate);
	// uso le cordinate ottenute prima per ottenere il mio rank
	MPI_Cart_rank(comm1D, cordinate, &myIDcartesiano);
//		printf("PW[%d]: my_cart_rank PCM[%d], my coords = (%d)\n",myID, my_cart_rank, coord[0]);
	int sinistra, destra;
	MPI_Cart_shift(comm1D, 0, 1, &sinistra, &destra);
	printf("Processore %d vicino sinistro %d, vicino destro %d \n", myID, sinistra, destra);

	if(sinistra >= 0) {
		MPI_Irecv(&vettoreLocale[0], 1, datatype, sinistra, 2, MPI_COMM_WORLD, &request);
		MPI_Send(&vettoreLocale[SIZE], 1, datatype, sinistra, 3, MPI_COMM_WORLD);
	}
	if(destra >= 0) {
		MPI_Irecv(&vettoreLocale[dimVettoreLocale - SIZE], 1, datatype, destra, 3, MPI_COMM_WORLD, &request);
		MPI_Send(&vettoreLocale[dimVettoreLocale - (2 * SIZE)], 1, datatype, destra, 2, MPI_COMM_WORLD);
	}
	
/*	if(myIDcartesiano == 0) {
		MPI_Irecv(&vettoreLocale[dimVettoreLocale - SIZE], 1, datatype, 1, 3, MPI_COMM_WORLD, &request);
		MPI_Send(&vettoreLocale[dimVettoreLocale - (2 * SIZE)], 1, datatype, 1, 2, MPI_COMM_WORLD);
	}
	if(myIDcartesiano == 1) {
		MPI_Irecv(&vettoreLocale[0], 1, datatype, 0, 2, MPI_COMM_WORLD, &request);
		MPI_Send(&vettoreLocale[SIZE], 1, datatype, 0, 3, MPI_COMM_WORLD);
	}
*/	
	MPI_Wait(&request, &status);
	for(int i = 0; i < dimVettoreLocale; ++i)
		printf("%d ", vettoreLocale[i]);
	printf("\n");
	
 	
	
	MPI_Comm_free(&comm1D);
			
	for(int i = 0; i < SIZE; ++i)
		free(matrice[i]);
	free(matrice);
	
	free(vettore);
	
	free(vettoreLocale);	
	
	MPI_Finalize();
return 0;
}
