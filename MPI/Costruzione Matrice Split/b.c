#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10

#define percentualePianteIniziali 0.75
#define percentualeNuovaPianta 0.5
#define percentualeInizioInfezione 0.1
#define numeroMinimoPropagazioneInfezione 1
#define percentualeSeccaPiante 0.3
#define numeroMinimoPropagazionePianteSecche 3

#define vuoto 0
#define germoglio 1
#define pianta 2
#define albero 3
#define germoglioSecco 4
#define piantaSecca 5
#define alberoSecco 6
#define germoglioInfetto 7
#define piantaInfetta 8
#define alberoInfetto 9

int numeroAdiacenti(int* vettore, const int indice, int statoIniziale, int statoFinale, int dimVettoreLocale) { 
	// in questo modo x punta alla colonna precedente al vicino alto sinistro di indice
	// y è sopra di lui
	// z ha l'indice della colonna successiva rispetto ad indice
    int somma = 0, x = indice - SIZE - 1, y = indice - 1, z = indice + SIZE - 1;
    
    for(int i = 0; i < 3; ++i, ++x, ++y, ++z) {
		if(x >= 0)
			if(vettore[x] >= statoIniziale && vettore[x] <= statoFinale)
				somma++;
		if(z < dimVettoreLocale)
			if(vettore[z] >= statoIniziale && vettore[z] <= statoFinale)
				somma++;
		if(y != indice) {
			if(indice % SIZE == 0 && y != indice - 1)
				somma++;
			if((indice - 1) % SIZE == 0 && y != indice + 1)
				somma++;
		}
	}

    return somma;
}

void cicloPiantaViva(int* vettore, const int indice, double r1, double r2, int dimVettoreLocale) {
//    printf("%f %f\n", r1, r2);
    if( r1 < percentualeInizioInfezione + (vettore[indice] * 0.05) || numeroAdiacenti(vettore, indice, germoglioInfetto, alberoInfetto, dimVettoreLocale) >= numeroMinimoPropagazioneInfezione) {
    	printf("Infezione %d\n", vettore[indice]);
        vettore[indice] += 6;
        printf("Infezione dopo %d\n", vettore[indice]);
//        printf("Numero infetti adiacenti allo stato %d, posizione %d %d, = %d\n", terreno[i][j].getStato(), i, j, numeroAdiacenti(i,j,germoglioInfetto, alberoInfetto));
//        printf("Numero minimo propagazione infezione = %d\n\n", percentuali.numeroMinimoPropagazioneInfezione);
    }
    else if( r2 < percentualeSeccaPiante + (vettore[indice] * 0.05) || numeroAdiacenti(vettore, indice, germoglioSecco, alberoSecco, dimVettoreLocale) >= numeroMinimoPropagazionePianteSecche) {
        printf("Secco %d\n", vettore[indice]);
        vettore[indice] += 3;
        printf("Secco dopo %d\n", vettore[indice]);
//        printf("Numero secchi adiacenti allo stato %d, posizione %d %d, = %d\n", terreno[i][j].getStato(), i, j, numeroAdiacenti(i,j,germoglioSecco, alberoSecco));
//        printf("Numero minimo propagazione secchi = %d\n\n", percentuali.numeroMinimoPropagazionePianteSecche);
    }
    else if(vettore[indice] == germoglio || vettore[indice] == pianta) {
    	printf("Germoglio %d\n", vettore[indice]);
        vettore[indice] += 1;
        printf("Germoglio dopo %d\n", vettore[indice]);
    }
}

void ciclo(int* vettore, const int indice, int dimVettoreLocale) {
    unsigned int seed = time(NULL);
    double random = (double)rand_r(&seed) / RAND_MAX;
    switch(vettore[indice]) {
		case vuoto: {
		 printf("Vuoto %d\n", vettore[indice]);
			if(random < percentualeNuovaPianta)
				vettore[indice] = germoglio;
            else
                vettore[indice] = vuoto;
            break;
        }
        case germoglioInfetto ... alberoInfetto: {
        printf("Infetto %d\n", vettore[indice]);
       		vettore[indice] = vuoto;
            break;
        }
        case germoglioSecco ... alberoSecco: {
        printf("Secco %d\n", vettore[indice]);
        	vettore[indice] = vuoto;
            break;
       }
       case germoglio ... albero: {
       printf("Vivo %d\n", vettore[indice]);
       		double random2 = (double)rand_r(&seed) / RAND_MAX;
            cicloPiantaViva(vettore, indice, random, random2, dimVettoreLocale);
            break;
       }
    }
}


int main() {

	int dimVettoreLocale = SIZE * SIZE + (2*SIZE);

	int* vettoreLocale = (int*) malloc(dimVettoreLocale* sizeof(int));
	
	int cont = 0, x = 0;
	for(int i = 0; i < dimVettoreLocale; ++i, cont++) {
		if(cont == 10) {
			cont = 0;
			x++;
		}
		vettoreLocale[i] = x;
	}
	
	for(int i = 0; i < dimVettoreLocale; ++i)
		printf("%d", vettoreLocale[i]);
	printf("\n");
	for(int j = SIZE; j < SIZE * 3; ++j)
				ciclo(vettoreLocale, j, dimVettoreLocale);
				
					for(int i = 0; i < dimVettoreLocale; ++i)
		printf("%d", vettoreLocale[i]);
	printf("\n");
return 0;
}
