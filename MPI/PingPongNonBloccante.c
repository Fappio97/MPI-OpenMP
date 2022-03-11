#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE 10
#define for_i for(int i = 0; i < SIZE; ++i)

int main(int argc, char* argv[]) {

	int myID, processi;
	
	int* a = (int*) malloc(SIZE* sizeof(int));
	int* b = (int*) malloc(SIZE* sizeof(int));
	
	MPI_Request request;
	MPI_Status status;
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &myID);
	MPI_Comm_size(MPI_COMM_WORLD, &processi);
	
	if(processi != 2)
		printf("Devi compilare con 2 processi\n");
	else {
		if(myID == 0) 
			for_i
				a[i] = i;
		
		else if(myID == 1)
			for_i
				a[i] = SIZE - 1 - i;
		
		if(myID == 0) {
			MPI_Irecv(b, SIZE, MPI_INT, 1 , 1, MPI_COMM_WORLD, &request);
			MPI_Send(a, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
			MPI_Wait(&request, &status);	// se cacci questa va direttamente avanti
			for(int i = 0; i < SIZE; ++i)
				printf("b[%d] = %d\n", i, b[i]);
		}
		else {
			MPI_Irecv(b, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
			MPI_Send(a, SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD);
		}
	}
	
	free(a);
	free(b);
	
	MPI_Finalize();
	
return 0;
}
