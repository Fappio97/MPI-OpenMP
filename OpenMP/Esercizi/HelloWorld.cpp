#include<omp.h>
#include <stdlib.h>

#include<iostream>
using namespace std;

int main() {	
	int numeroThread;
	double tempo= 0;
	
		tempo = omp_get_wtime();
		printf("Numero core disponibili %d\n", omp_get_num_procs());

		if(omp_in_parallel())
			printf("Siamo in una parte parallela\n");
		else
			printf("Non siamo in una parte parallela\n");
			
		tempo = omp_get_wtime();
	
		#pragma omp parallel private(numeroThread)
		{
			if(omp_in_parallel())
				printf("Siamo in una parte parallela\n");
			else
				printf("Non siamo in una parte parallela\n");
			numeroThread = omp_get_thread_num();
			printf("Hello world from thread %d\n", numeroThread);
			
			
			if(numeroThread == 0) {
				printf("Numero threads %d\n", omp_get_num_threads());
			}
		}
		
		printf("Tempo di esecuzione %f", omp_get_wtime() - tempo);
		printf("ms\n");
		
return 0;
}
