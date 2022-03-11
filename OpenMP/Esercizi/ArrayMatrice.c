#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	int** a, righe, colonne;
	int* b, size;
	
	printf("Inserisci la dimensione dell'array: ");
	scanf("%d", &size);
	
	b = malloc(size* sizeof(int));
	
	for(int i = 0; i < size; i++) {
		printf("Inserisci il valore di dell'array[%d] = ", i);
		scanf("%d", &b[i]);
	}
	
	printf("\nARRAY\n");
	#pragma parallel for num_threads(omp_get_num_threads())
	for(int i = 0; i < size; i++) {
		printf("array[%d] = %d\n", i, b[i]);
	}
	printf("\n");
	
	
	printf("Inserisci le colonne della matrice: ");
	scanf("%d", &righe);
	printf("Inserisci le colonne della matrice: ");
	scanf("%d", &colonne);
	
	a = (int**) malloc (righe* sizeof(int*));
	for(int i = 0; i < righe; i++)
		a[i] = malloc(colonne* sizeof(int));
		
	for(int i = 0; i < righe; i++) {
		for(int j = 0; j < colonne; j++) {
			printf("Inserisci il valore di della matrice[%d][%d] = ", i, j);
			scanf("%d", &a[i][j]);	
		}
	}	
	
	printf("\nMATRICE\n");
	#pragma parallel for num_threads(omp_get_num_threads())
	for(int i = 0; i < righe; i++) {
		for(int j = 0; j < colonne; j++) {
			printf("matrice[%d][%d] = %d\n", i, j, a[i][j]);
		}
		printf("\n");
	}
		
return 0;
}
