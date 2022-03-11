#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

#define size 10

int main(int argv, char** args) {
	int myid, num, x, high, low, sommaLocale, somma;
	int a[size] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	MPI_Init(&argv, &args);
	
	MPI_Status(status);
	
	somma = 0;
	sommaLocale = 0;
	
	MPI_Comm_size(MPI_COMM_WORLD, &num);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	x = size / num;
	low = myid * x;
	
	
	for(int i = low; i < low + x; i++)
		sommaLocale += a[i];
	
	if(myid == 0) {
		somma = sommaLocale;
		for(int i = 1; i < num; i++) {
			MPI_Recv(&sommaLocale, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			somma += sommaLocale;
		}
	}
	
	else {
		MPI_Send(&sommaLocale, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	
	if(myid == 0) {
		printf("Somma totale %d\n", somma);
	}
	
	MPI_Finalize();
}
