/* spedizone della terza colonna di una matrice dal processo 0 al processo 1 
 *     process 1 
 * 
 * Note: Dovrebbe eseguito su due processi. 
 * 
 */

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define SIZE 10

int convertiMatriceInVettore(int** matrice, int* vettore) {
	int k;
	for_ij {
		k = j * SIZE + i;
		vettore[k] = matrice[i][j];
	}
}

int main(int argc, char* argv[]) {
	int p;
	int my_rank, elementiPerProcesso;
	float** A = (float**) malloc(SIZE* sizeof(float*));
	for(int i = 0; i < SIZE; ++i)
		A[i] = (float*) malloc(SIZE* sizeof(float)); 
		
	int dimVettore = SIZE * SIZE;
	
	int* vettore = (int*) malloc(dimVettore* sizeof(int));
	int* vettoreLocale;
	 
	MPI_Status status;    
	MPI_Datatype column_mpi_t;
	
	int i, j;   
	 
	MPI_Init(&argc, &argv);    
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);  
	
	elementiPerProcesso = dimVettore / processi;
	vettoreLocale = (int*) malloc((elementiPerProcesso + (2 * SIZE))* sizeof(int));
	  
	MPI_Type_vector(10, 1, 10, MPI_FLOAT, &column_mpi_t);    
	MPI_Type_commit(&column_mpi_t);
	
	if (my_rank == 0) {
		printf("Rank 0\n");
		for (i = 0; i < 10; i++) {
			for (j = 0; j < 10; j++)  {              
				A[i][j] = (float) j;
				printf("%3.1f ", A[i][j]);  
			}
			printf("\n");
		}      
		convertiMatriceInVettore(matrice, vettore);
		MPI_Send(&(A[0][2]), 1, column_mpi_t, 1, 0, MPI_COMM_WORLD);    
	} else { 
		/* my_rank = 1 */  
		MPI_Recv(&(A[0][2]), 1, column_mpi_t, 0, 0, MPI_COMM_WORLD, &status);
		for (i = 0; i < 10; i++)
			printf("%3.1f ", A[i][2]);        
		printf("\n");   
	}    
	MPI_Finalize();
return 0;
}  /* main */
