#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {

	int numtasks, rank, destinatario, sorgente, errore, cont, tag = 1, rc;
	char* daRicevere;
	char daInviare[5] = "ciao";
	
	MPI_Init(&argc, &argv);
	
	MPI_Status status;
	
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank == 0) {
		sorgente = 1;
		destinatario = 1;
		rc = MPI_Send(&daInviare, strlen(daInviare) + 1, MPI_CHAR, destinatario, tag, MPI_COMM_WORLD);
		rc = MPI_Recv(&daRicevere, 100, MPI_CHAR, sorgente, tag, MPI_COMM_WORLD, &status);
	} else if(rank == 1) {
		destinatario = 0;
		sorgente = 0;
		rc = MPI_Recv(&daRicevere, 100, MPI_CHAR, sorgente, tag, MPI_COMM_WORLD, &status);
		rc = MPI_Send(&daInviare, strlen(daInviare) + 1, MPI_CHAR, destinatario, tag, MPI_COMM_WORLD);
	}
	rc = MPI_Get_count(&status, MPI_CHAR, &cont);
	
	printf("Task %d: received %d stringhe da il task %d con il tag %d \n", rank, cont, status.MPI_SOURCE, status.MPI_TAG);

	MPI_Finalize();

}
