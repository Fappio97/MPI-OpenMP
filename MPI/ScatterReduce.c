#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

int main(int args, char* argv[]) {
	int myID, numProcessi, somma = 0, sommaLocale = 0, elementiPerProcesso;
	
	int* vettore = (int*) malloc(SIZE* sizeof(int));
	int* vettoreLocale;
		
	MPI_Init(&args, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &numProcessi);
	MPI_Comm_rank(MPI_COMM_WORLD, &myID);
	
	elementiPerProcesso = SIZE / numProcessi;
	vettoreLocale = (int*) malloc(elementiPerProcesso* sizeof(int));
	
	if(myID == 0) {
		for(int i = 0; i < SIZE; ++i)
			vettore[i] = i;		
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
			somma += vettore[SIZE - 1];
		printf("Somma totale = %d\n", somma);
		printf("Tempo totale = %f\n", fine - inizio);
	}
	
	free(vettore);	
	free(vettoreLocale);
	
	MPI_Finalize();
	
return 0;
}
