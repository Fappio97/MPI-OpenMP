#include<omp.h>
#include<stdlib.h>

int main() {

	int somma = 0, operazione = 0;
	
	#pragma omp parallel private(operazione)		//di default sono passati shared
	{
		operazione += omp_get_thread_num();
		printf("Operazione %d\n", operazione);

		#pragma omp critical
		
		somma += operazione;
		
	}
	
	printf("Somma %d\n", somma);
	printf("Operazione %d\n", operazione);	//l'operazione è zero perché essendo passato come shared avevo una copia all'interno di ogni thread e quindi il suo valore non è mai cambiato

return 0;
}
