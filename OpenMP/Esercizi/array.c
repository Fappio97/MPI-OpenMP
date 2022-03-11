#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main( )
{
    int *a, *b, n;
    
    printf("Da quanti elementi è composto il tuo array? ");
    scanf("%d", &n);
    
    a = malloc(n* sizeof(int));
    b = malloc(n* sizeof(int));

    #pragma omp parallel num_threads(omp_get_num_threads())
    {
        #pragma omp for
        for (int i = 0; i < n; i++) {
            a[i] = i * i;
            b[i] = a[i];
        }

       	#pragma omp barrier 
        
        #pragma omp for
        for (int i = 0; i < n; i++) {
            a[i] = a[i] + b[4-i];
        }
  
    }
    
    printf("\nArray\n");
	for(int i = 0; i < n; i++)
		printf("a[%d] = %d\n", i, a[i]);
}
