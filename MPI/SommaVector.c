#include <mpi.h>
#include <stdio.h>

int main(int args, char* argv[]) {
	int myID, numProcessi, somma = 0, sommaLocale = 0, elementiPerProcesso;
	
	int a[] = {1,2,3,4,5,6,7,8,9,10};
		
	MPI_Init(&args, &argv);
	
	double inizio = MPI_Wtime();

	MPI_Status status;
	
	MPI_Comm_size(MPI_COMM_WORLD, &numProcessi);
	MPI_Comm_rank(MPI_COMM_WORLD, &myID);
	
	elementiPerProcesso = 10 / numProcessi;
	int low = myID * elementiPerProcesso;
	
	for(int i = low; i < low + elementiPerProcesso && i < 10; i++) {
		sommaLocale += a[i];
	}
	
	if(myID == 0) {
		somma = sommaLocale;
		for(int i = 1; i < numProcessi; i++) {
			MPI_Recv(&sommaLocale, 1, MPI_INT, i, i, MPI_COMM_WORLD, &status);
			somma += sommaLocale;
		}
		
		printf("Somma totale %d\n", somma);
	}
	else {
		for(int i = 1; i < numProcessi; i++)
			MPI_Send(&sommaLocale, 1, MPI_INT, 0, i, MPI_COMM_WORLD);
	}
	
	double fine = MPI_Wtime();
	
	if(myID == 0) {
		printf("Tempo totale = %f\n", fine - inizio);
	}
	
	
	
	
	MPI_Finalize();
	
return 0;
}
