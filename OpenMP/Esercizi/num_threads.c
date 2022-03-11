#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	int a[10];
	double time = omp_get_wtime();	

	#pragma omp parallel num_threads(omp_get_num_threads())
	{
		#pragma omp for
		for(int i = 0; i < 10; i++) {
			a[i] = i + 5;
		}	
	}

	printf("Tempo %f\n", omp_get_wtime() - time);
return 0;
}
