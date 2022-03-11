#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 4

#define for_ij for(int i = 0; i < SIZE; ++i) for(int j = 0; j < SIZE; ++j)

int convertiMatriceInVettore(int** matrice, int* vettore) {
	int k;
	for_ij {
		k = j * SIZE + i;
		vettore[k] = matrice[i][j];
	}
}

int main(int args, char* argv[]) {
	int myID, numProcessi, somma = 0, sommaLocale = 0, elementiPerProcesso;
	
	int** matrice = (int**) malloc(SIZE* sizeof(int*));
	for(int i = 0; i < SIZE; ++i) 
		matrice[i] = (int*) malloc(SIZE* sizeof(int));
	
	int* vettoreLocale;
	
	int dimVettore = SIZE * SIZE;
	
	int* vettore = (int*) malloc(dimVettore* sizeof(int));
		
	MPI_Init(&args, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &numProcessi);
	MPI_Comm_rank(MPI_COMM_WORLD, &myID);
	
	vettoreLocale = (int*) malloc(dimVettore* sizeof(int));
	
	elementiPerProcesso = dimVettore / numProcessi;
	
	if(myID == 0) {
		for(int i = 0; i < SIZE; ++i)
			for(int j = 0; j < SIZE; ++j)
				matrice[i][j] = i;
		convertiMatriceInVettore(matrice, vettore);
	}
		
	MPI_Barrier(MPI_COMM_WORLD);
	double inizio = MPI_Wtime();
		
	MPI_Scatter(vettore, elementiPerProcesso, MPI_INT, vettoreLocale, elementiPerProcesso, MPI_INT, 0, MPI_COMM_WORLD);
		
	for(int i = 0; i < elementiPerProcesso; ++i)
		sommaLocale += vettoreLocale[i];
			
	MPI_Reduce(&sommaLocale, &somma, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	double fine = MPI_Wtime();
		
	if(myID == 0) {
		if(numProcessi % 2 != 0)
			somma += vettore[SIZE * SIZE -1];
		printf("Somma totale = %d\n", somma);
		printf("Tempo totale = %f\n", fine - inizio);
	}
	
	for(int i = 0; i < SIZE; ++i)
		free(matrice[i]);
	free(matrice);
	
	free(vettore);
	free(vettoreLocale);	
	
	MPI_Finalize();
	
return 0;
}
