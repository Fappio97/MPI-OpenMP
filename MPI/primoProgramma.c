#include<mpi.h>
#include<stdio.h>

int main(int argc, char* argv[]) {

	int myid;
	int N;
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &N);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	printf("Processore %d di %d\n", myid, N);
	
	MPI_Finalize();
	
return 0;
}
