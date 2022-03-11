#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	int p;
	int my_rank;
	
	MPI_Init(&argc, &argv);    
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
	MPI_Status status;
	MPI_Request request;
	
	int* vettore = (int*) malloc(100* sizeof(int));
	
	if(my_rank == 0) {
		for(int i = 0; i < 50; i++)
			vettore[i] = 0;
		for(int i = 50; i < 100; i++)
			vettore[i] = 1;
		MPI_Send(vettore+50, 50, MPI_INT, 1, 0, MPI_COMM_WORLD);
	} else {
		MPI_Irecv(&vettore[0], 50, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
		for(int i = 50; i < 100; ++i)
			vettore[i] = 0;
		MPI_Wait(&request, &status);
		for(int i = 0; i < 100; ++i)
			printf("%d ", vettore[i]);
		printf("\n");
	}
	MPI_Finalize();
return 0;
}
