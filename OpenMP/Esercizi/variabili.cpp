#include<omp.h>
#include<stdlib.h>

#include<iostream>
using namespace std;

int main() {
int a = 3;

#pragma omp parallel private(a) //in questo modo la variabile non è inizializzata ed assume il valore 0
{
	printf("Numero thread = %d\n", omp_get_thread_num());
	a += omp_get_thread_num();
	printf("Risultato operazione = %d\n", a);
}
	
	printf("Valore finale a = %d\n\n", a);
	
#pragma omp parallel firstprivate(a) //in questo modo la variabile è inizializzata al valore prima della creazione della regione parallela, ovvero 3
{
	printf("Numero thread = %d\n", omp_get_thread_num());
	a += omp_get_thread_num();
	printf("Risultato operazione = %d\n", a);
}

	printf("Valore finale a = %d\n\n", a);


#pragma omp parallel shared(a) //in questo modo la variabile è in memoria condivisa e quindi viene ogni volta aggiornata
{
	printf("Numero thread = %d\n", omp_get_thread_num());
	a += omp_get_thread_num();
	printf("Risultato operazione = %d\n", a);
}

	printf("Valore finale a = %d\n\n", a);


return 0;
}
