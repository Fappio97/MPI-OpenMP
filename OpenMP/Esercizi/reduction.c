#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main() {

	int somma = 0;
	double tempo = omp_get_wtime();

	#pragma omp parallel for reduction(+:somma)
		for(int i = 0; i < 10000; i++) {
			somma += i+omp_get_thread_num(); 
		}	

	printf("Tempo %f\n", omp_get_wtime() - tempo);

return 0;
}
