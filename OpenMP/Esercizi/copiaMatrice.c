#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	int colonne, righe;
	printf("Inserire le righe della matrice ");
	scanf("%d", &righe);
	printf("Inserire le colonne della matrice ");
	scanf("%d", &colonne);
 
 	int** a; 
 	int** b;

	a = (int**) malloc(righe* sizeof(int*));
	b = (int**) malloc(righe* sizeof(int*));
	
	for(int i = 0; i < righe; i++) {
		a[i] = malloc(righe* sizeof(int));
		b[i] = malloc(righe* sizeof(int));
	}
		
	for(int i = 0; i < righe; i++) {
		for(int j = 0; j < colonne; j++) {
			printf("Inserire l'elemento nella riga %d e nella colonna %d: ", i, j);
			scanf("%d", &a[i][j]);
		}
	}
			
	double time = omp_get_wtime();
			
	#pragma omp parallel for num_threads(omp_get_num_threads())
		for(int i = 0; i < righe; i++) {
			for(int j = 0; j < colonne; j++) {
				b[i][j] = a[i][j];
			}
		}
	
	printf("Tempo: %f\n", (omp_get_wtime() - time));
	
	for(int i = 0; i < righe; i++) {
		for(int j = 0; j < colonne; j++) {
			printf("b[%d][%d] = %d\n", i, j, b[i][j]);
		}
		printf("\n");
	}
	
return 0;
}
