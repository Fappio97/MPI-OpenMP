#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
	int myID, size;
	int dimensione[2], torodalita[2], riordina;
	int cordinate[2], id;
	int destra, sinistra, sopra, sotto;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myID);
	if (size != 12)
    {
        printf("Please run with 12 processes.\n");fflush(stdout);
        MPI_Abort(MPI_COMM_WORLD, 1);
		MPI_Finalize();
		return 0;
    }
    
    dimensione[0] = 4;
	dimensione[1] = 3;
	torodalita[0] = 1;
	torodalita[1] = 0;
	riordina = 1;
	MPI_Comm comunicatore;
	
	MPI_Cart_create(MPI_COMM_WORLD, 2, dimensione, torodalita, riordina, &comunicatore);
	if(myID == 4) {
		MPI_Cart_coords(comunicatore, myID, 2, cordinate);
		printf("Il processo %d ha coorinate %d;%d\n", myID, cordinate[0], cordinate[1]);
		MPI_Cart_shift(comunicatore, 0, 1, &sinistra, &destra);
		MPI_Cart_shift(comunicatore, 1, 1, &sopra, &sotto);
		printf("Il processo %d, ha come vicino sinistro %d, vicino destro %d, vicino sopra %d, vicino sotto %d\n", myID, sinistra, destra, sopra, sotto);
	}
	if(myID == 1) {
		cordinate[0] = 3;
		cordinate[1] = 1;
		MPI_Cart_rank(comunicatore, cordinate, &myID);
		printf("Il processo nelle cordinate %d;%d è il numero %d\n", cordinate[0], cordinate[1], myID);
		MPI_Cart_shift(comunicatore, 0, 1, &sinistra, &destra);
		MPI_Cart_shift(comunicatore, 1, 1, &sopra, &sotto);
		printf("Il processo %d, ha come vicino sinistro %d, vicino destro %d, vicino sopra %d, vicino sotto %d\n", myID, sinistra, destra, sopra, sotto);
	}
	if(myID == 8) {
		MPI_Cart_shift(comunicatore, 0, 1, &sinistra, &destra);
		MPI_Cart_shift(comunicatore, 1, 1, &sopra, &sotto);
		printf("Il processo %d, ha come vicino sinistro %d, vicino destro %d, vicino sopra %d, vicino sotto %d\n", myID, sinistra, destra, sopra, sotto);
	
	}
	
	MPI_Finalize();
	
return 0;
}
