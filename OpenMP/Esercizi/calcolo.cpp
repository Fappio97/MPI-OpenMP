#include<iostream>
using namespace std;

#include<omp.h>
#include<stdlib.h>

int main() {
	int x, y, somma = 0;
	double tempo = omp_get_wtime();
	
	#pragma omp parallel for private(x, y) // reduction(+:somma)
		for(int i = 0; i < omp_get_num_threads(); ++i) {
			printf("Valore i: %d\n", i);
			printf("Valore somma: %d\n", somma);
//			#pragma omp critical
			somma += i; 
//			printf("Nel thread num: %d\n", omp_get_thread_num());
		}
	
	printf("Risultato somma: %d\n", somma);
	printf("Tempo totale: %f\n", omp_get_wtime() - tempo);
		
return 0;
}
