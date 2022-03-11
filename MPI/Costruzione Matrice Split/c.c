#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// grandezza matrice partenza SIZE * SIZE
#define SIZE 10

// numero volte ciclo
#define VOLTE 1

// percentuali
#define percentualePianteIniziali 0.75
#define percentualeNuovaPianta 0.5
#define percentualeInizioInfezione 0.1
#define numeroMinimoPropagazioneInfezione 1
#define percentualeSeccaPiante 0.3
#define numeroMinimoPropagazionePianteSecche 3

// stati zolla terreno
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

#define for_i for(int i = 0; i < SIZE; ++i)
#define for_ij for_i for(int j = 0; j < SIZE; ++j)


// converte la matrice dinamica in vettore in modo tale da avere elementi continui in memoria
void convertiMatriceInVettore(int** matrice, int* vettore) {
	int k;
	for(int i = 0; i < SIZE; ++i) {
		for(int j = 0; j < SIZE; ++j) {
			k = j * SIZE + i;
			vettore[k] = matrice[i][j];
		}
	}
}

// genera casualmente le piante nel terreno iniziale
void generaCasualmenteInizio(int** terreno) {
    unsigned int seed = time(NULL);
    for_ij {
        double random = (double)rand_r(&seed) / RAND_MAX;
        if (random < percentualePianteIniziali) {
            int r1 = rand_r(&seed)%3 +1;
            terreno[i][j] = r1;
        }
        else
            terreno[i][j] = vuoto;
    }

 /*   for_i
        printf("numero random %f\n", rand() / double(RAND_MAX));*/
}

// mi restituisce il numero di adiacenti
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
    //	printf("Infezione %d\n", vettore[indice]);
        vettore[indice] += 6;
   //     printf("Infezione dopo %d\n", vettore[indice]);
//        printf("Numero infetti adiacenti allo stato %d, posizione %d %d, = %d\n", terreno[i][j].getStato(), i, j, numeroAdiacenti(i,j,germoglioInfetto, alberoInfetto));
//        printf("Numero minimo propagazione infezione = %d\n\n", percentuali.numeroMinimoPropagazioneInfezione);
    }
    else if( r2 < percentualeSeccaPiante + (vettore[indice] * 0.05) || numeroAdiacenti(vettore, indice, germoglioSecco, alberoSecco, dimVettoreLocale) >= numeroMinimoPropagazionePianteSecche) {
  //      printf("Secco %d\n", vettore[indice]);
        vettore[indice] += 3;
   //     printf("Secco dopo %d\n", vettore[indice]);
//        printf("Numero secchi adiacenti allo stato %d, posizione %d %d, = %d\n", terreno[i][j].getStato(), i, j, numeroAdiacenti(i,j,germoglioSecco, alberoSecco));
//        printf("Numero minimo propagazione secchi = %d\n\n", percentuali.numeroMinimoPropagazionePianteSecche);
    }
    else if(vettore[indice] == germoglio || vettore[indice] == pianta) {
 //   	printf("Germoglio %d\n", vettore[indice]);
        vettore[indice] += 1;
//        printf("Germoglio dopo %d\n", vettore[indice]);
    }
}

void ciclo(int* vettore, const int indice, int dimVettoreLocale) {
    unsigned int seed = time(NULL);
    double random = (double)rand_r(&seed) / RAND_MAX;
    switch(vettore[indice]) {
		case vuoto: {
			if(random < percentualeNuovaPianta)
				vettore[indice] = germoglio;
            else
                vettore[indice] = vuoto;
            break;
        }
        case germoglioInfetto ... alberoInfetto: {
       		vettore[indice] = vuoto;
            break;
        }
        case germoglioSecco ... alberoSecco: {
        	vettore[indice] = vuoto;
            break;
       }
       case germoglio ... albero: {
       		double random2 = (double)rand_r(&seed) / RAND_MAX;
            cicloPiantaViva(vettore, indice, random, random2, dimVettoreLocale);
            break;
       }
       default:
       		break;
    }
}

int main(int argc, char* argv[]) {

	int myID, processi;

	// creo la matrice dinamicamente	
	int** matrice = (int**) malloc(SIZE* sizeof(int*));
	for(int i = 0; i < SIZE; ++i)
		matrice[i] = (int*) malloc(SIZE* sizeof(int));
		
	int dimVettore = SIZE * SIZE;
	
	// creo un vettore in cui linearizzo la matrice dinamica
	int* vettore = (int*) malloc(dimVettore* sizeof(int));
	// creo un vettoreLocale dove invierò i vari valori ai processori
	int* vettoreLocale;
		
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &processi);
	MPI_Comm_rank(MPI_COMM_WORLD, &myID);
	
	MPI_Status status;    
	MPI_Request request;
	
	if(SIZE % processi != 0) {
		if(myID == 0)
			printf("Compila con un numero di processi sottomultiplo di %d\n", SIZE);
		MPI_Finalize();
		exit(1);
	}
	
	// numero elementi che devono essere ripartiti fra i processori
	int elementiPerProcesso = dimVettore / processi;
	
	//devi fare che esce se non invochi processori multipli
	
	// la dimensione del vettoreLocale è pari agli elementi per processo + 2 volte la size delle colonne, che rappresentano le colonne ghost
	int dimVettoreLocale = elementiPerProcesso + (2 * SIZE);
	
	vettoreLocale = (int*) malloc(dimVettoreLocale* sizeof(int));
	
	// Creazione datatype, è continuo in quanto le colonne sul vettore
	// sono scandite linearmente
	MPI_Datatype datatype;
	MPI_Type_contiguous(SIZE, MPI_INT, &datatype);
    	MPI_Type_commit(&datatype);
	
	if(myID == 0) {
//		generaCasualmenteInizio(matrice);
		for_ij
			matrice[i][j] = j;
		convertiMatriceInVettore(matrice, vettore);
		
		// stampa matrice
/*		for_ij
			printf("matrice[%d][%d] = %d\n", i, j, matrice[i][j]);*/
			
		// stampa vettore ottenuto dalla conversione della matrice dinamica
/*		for(int i = 0; i < dimVettore; ++i)
			printf("%d ", vettore[i]);
		printf("\n");*/
		
		// send di prova
/*		for(int i = 1; i < processi; ++i)
			MPI_Isend(&vettore[elementiPerProcesso * i], 1, datatype, i, i, MPI_COMM_WORLD, &request);*/
	} 
/*	else {
		// recv di prova
//		MPI_Irecv(&vettoreLocale[SIZE], 1, datatype, 0, myID, MPI_COMM_WORLD, &request);

		// inizializzo le parti che non ricevo a -1
		for(int i = elementiPerProcesso; i < dimVettoreLocale; ++i)
			vettoreLocale[i] = -1;
		for(int i = 0; i < SIZE; ++i)
			vettoreLocale[i] = 0;
			
		// wait per la Irecv
		MPI_Wait(&request, &status);
		
		// stampa del vettore ottenuto dopo le send recv di prova
		for(int i = 0; i < dimVettoreLocale; ++i)
			printf("%d ", vettoreLocale[i]);
		printf("\n");
	}*/
	
	// inizializzo le parti che non ricevo a -1
	for(int i = 0; i < dimVettoreLocale; ++i)
		vettoreLocale[i] = -1;
		
	if(myID == 0) {
		int cont = 0, x = 0;
		for(int i = 0; cont < SIZE * SIZE; i += SIZE, ++cont) {
			if(i >= dimVettore) {
				printf("\n");
				x++;
				i = x;
			}
			printf("%d", vettore[i]);
		}
	}
	
	// invio ad ogni processo, sul loro vettoreLocale, le colonne su cui devono lavorare
	// da notare che gliele invio sul vettoreLocale non dalla posizione 0 ma dalla poszione
	// SIZE poiché gli elementi che vanno da 0 a SIZE rappresenteranno la cella ghost di sinistra
	// per quel processore e siccome il vettoreLocale ha dimensione dimVettoreLocale + (2 * SIZE)
	// non si riempirà mai no lo scatter le ultime SIZE posizioni che rappresentano la cella ghost
	// di destra
	MPI_Scatter(vettore, elementiPerProcesso, MPI_INT, &vettoreLocale[SIZE], elementiPerProcesso, MPI_INT, 0, MPI_COMM_WORLD);
	
	// stampa prova dopo lo scatter
/*	for(int i = 0; i < dimVettoreLocale; ++i)
		printf("%d ", vettoreLocale[i]);
	printf("\n");*/
/*	----------------------------------------------------------------------	
	// creazione di elementi per la topologia cartesiana
	// la topologia cartesiana sarà di una dimensione, non avrà periodicità
	// perché il mio automa cellulare non prevede ciò
	int dimensione[1],cordinate[1];
	dimensione[0] = 0, cordinate[0] = 0;
	int myIDcartesiano;
	MPI_Comm comm1D;
	int periodicita[1];
	periodicita[0] = 0;  
	// periodicità false, non mi serve nel mio automa
	int riordina = 1;
	MPI_Dims_create(processi, 1, dimensione);
	MPI_Cart_create(MPI_COMM_WORLD, 1, dimensione, periodicita, riordina, &comm1D);
	// cerco le cordinate di ogni processo sulla topologia
	MPI_Cart_coords(comm1D, myID, 1, cordinate);
	// uso le cordinate ottenute prima per ottenere il mio rank
	MPI_Cart_rank(comm1D, cordinate, &myIDcartesiano);
//		printf("PW[%d]: my_cart_rank PCM[%d], my coords = (%d)\n",myID, my_cart_rank, coord[0]);
	int sinistra, destra;
	MPI_Cart_shift(comm1D, 0, 1, &sinistra, &destra);
//	printf("Processore %d vicino sinistro %d, vicino destro %d \n", myID, sinistra, destra);
*////-----------
	MPI_Barrier(MPI_COMM_WORLD);
	double inizio = MPI_Wtime();
 //------------------------------------------------------------------------------------
	// ciclo
	for(int i = 0; i < VOLTE; ++i) {
	
		// per ogni processore mi domande se esso ha un vicino sinistro e destro
		// dato che la funzione shift restituisce un valore negativo se non possiede quel vicino
		// per ogni vicino scambio, per i vicini da sinistra l'ultima cella del vicino e la metto 
		// nelle posizione da 0 a SIZE, per quelle da destra nell'ultime posizioni del vettoreLocale
/*-----------------		if(sinistra >= 0) {
			MPI_Irecv(&vettoreLocale[0], 1, datatype, sinistra, 2, MPI_COMM_WORLD, &request);
			MPI_Send(&vettoreLocale[SIZE], 1, datatype, sinistra, 3, MPI_COMM_WORLD);
		}
		if(destra >= 0) {
			MPI_Irecv(&vettoreLocale[dimVettoreLocale - SIZE], 1, datatype, destra, 3, MPI_COMM_WORLD, &request);
			MPI_Send(&vettoreLocale[dimVettoreLocale - (2 * SIZE)], 1, datatype, destra, 2, MPI_COMM_WORLD);
		}*/ //---------------------------------
	
	// prova per lo scambio
/*	if(myIDcartesiano == 0) {
		MPI_Irecv(&vettoreLocale[dimVettoreLocale - SIZE], 1, datatype, 1, 3, MPI_COMM_WORLD, &request);
		MPI_Send(&vettoreLocale[dimVettoreLocale - (2 * SIZE)], 1, datatype, 1, 2, MPI_COMM_WORLD);
	}
	if(myIDcartesiano == 1) {
		MPI_Irecv(&vettoreLocale[0], 1, datatype, 0, 2, MPI_COMM_WORLD, &request);
		MPI_Send(&vettoreLocale[SIZE], 1, datatype, 0, 3, MPI_COMM_WORLD);
	}
*/	
		// con questo for escludo le celle adiacenti alle celle ghost, sia destra che sinistra
		// poiché non sono ancora sicuro che siano avvenuti gli scambi e per velocità queste
		// possono essere effettuate indifferentemente dalla recv, così le evolvo
//		printf("dimensioneVettore %d, SIZE*2 %d, fine %d\n", dimVettoreLocale, SIZE * 2, dimVettoreLocale - (SIZE * 2));
/*-------------		for(int j = SIZE * 2; j < dimVettoreLocale - (SIZE * 2); ++j)
			ciclo(vettoreLocale, j, dimVettoreLocale);
			
		// metto una barrier perché per poter andare avanti devo essere avvenuti gli scambi
		MPI_Wait(&request, &status);
		// effettuo il ciclo da SIZE perché le ccelle da 0 a SIZE sono ghost e quindi non vanno
		// evolute in questo processo
		for(int j = SIZE; j < SIZE * 2; ++j)
			ciclo(vettoreLocale, j, dimVettoreLocale);
		for(int j = dimVettoreLocale - (SIZE * 2); j < dimVettoreLocale; ++j)
			ciclo(vettoreLocale, j, dimVettoreLocale);
	*/ //--------------------------------------------------
//	MPI_Wait(&request, &status);

		for(int i = SIZE; i < dimVettoreLocale - SIZE; ++i) {
			vettoreLocale[i]++;
		}

		MPI_Gather(&vettoreLocale[SIZE], elementiPerProcesso, MPI_INT, vettore, elementiPerProcesso, MPI_INT, 0, MPI_COMM_WORLD);

/*		for(int i = 0; i < dimVettoreLocale; ++i)
			if(vettoreLocale[i] != -1)
				printf("%d ", vettoreLocale[i]);
		printf("\n");*/
		
		MPI_Barrier(MPI_COMM_WORLD);
		printf("\n");
/*		if(myID == 0) {
		int cont = 0, x = 0;
		for(int i = 0; cont < SIZE * SIZE; i += SIZE, ++cont) {
			if(i >= dimVettore) {
				printf("\n");
				x++;
				i = x;
			}
			printf("%d", vettore[i]);
		}
		}*/
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	double fine = MPI_Wtime();
	
	if(myID == 0)
		printf("Durata totale: %f\n", fine - inizio);
 	
	// deallocazioni
//	MPI_Comm_free(&comm1D);
			
	for(int i = 0; i < SIZE; ++i)
		free(matrice[i]);
	free(matrice);
	
	free(vettore);
	
	free(vettoreLocale);	
	
	MPI_Finalize();
return 0;
}

