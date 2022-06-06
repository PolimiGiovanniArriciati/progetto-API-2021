// distanza--> diagonale della mattrice
//
//  main.c
//	Progetto di Algoritmi e  Principi dell'Informatica
//
//  Created by Giovanni  Arriciati  on 02/07/21.
//
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

#include <stdio.h>

#define MAX 4294967295                      //2^64 = 18446744073709551615					//2^32 = 4294967295
#define OFFSET '0'

typedef struct classifica_s{
    unsigned long value;
    unsigned int grafo;
}classifica_t;

int main(void){
	unsigned int podio, dim;								            //variabili statiche
	short int SiMinimo, UscitaRapida;         //Variabili di supporto
    unsigned int i, j, k, num_graph, Nodo_Min, Min, sum, IniziaClassifica;
    unsigned long Valore, LunghezzaRiga;
    classifica_t tmp;
    char str_in[15];
	
	fscanf(stdin,"%u %u\n", &dim, &podio);
	unsigned int M[dim][dim];                           //ottimizzazione: se dim > 32 uso un min heap?
	classifica_t classifica[podio];                           //Meglio implementato come un min-heap? Accesso in n ma ordinamento in log(n)
  	LunghezzaRiga = 8*dim;
    char RigaMatrice[LunghezzaRiga];
    IniziaClassifica = podio;
	num_graph = 0;
    
    while(fgets(str_in, 15, stdin)){

        if(str_in[0] == 'A'){                               //Iniz. Matrice di adiacenza e distanze:

//PRIMA RIGA:
            Nodo_Min = 0;
            Min = MAX;

            fgets(RigaMatrice, LunghezzaRiga, stdin);
            
            M[0][0] = 0;                            // primo elemento
            
            k=0;
            while(RigaMatrice[k] != ','){
                M[0][0] *= 10; 
                M[0][0] += (int)RigaMatrice[k] - OFFSET;
                k++;
            }
            k++;
            M[0][0] = 0;                                //Conservo la lunghezza del cammino minimo verso un j-esimo nodo nel j-esimo elemento.

            for(j = 1; j < dim - 1; j++){
                M[0][j] = 0;                        // elementi intermedi
                while(RigaMatrice[k] != ','){
                    M[0][j] *= 10; 
                    M[0][j] += (int)RigaMatrice[k]-OFFSET;
                    k++;
                }
                k++;

                M[j][j] = M[0][j];                      //Conservo la lunghezza del cammino minimo verso un j-esimo nodo nel j-esimo elemento.
                if(M[j][j] < Min && M[j][j]){
                    Min = M[j][j];
                    Nodo_Min = j;
                }
            }
            M[0][j] = 0;                             // ultimo elemento
            while(RigaMatrice[k] != '\n'){
                M[0][j] *= 10;
                M[0][j] += (int)RigaMatrice[k]-OFFSET;
                k++;
            }

            M[j][j] = M[0][j];                          //Conservo la lunghezza del cammino minimo verso un j-esimo nodo nel j-esimo elemento.
            if(M[j][j] < Min && M[j][j]){
                Min = M[j][j];
                Nodo_Min = j;
            }                       //distanza minima da nodo zero valutata.


//ALTRE RIGHE:
            for(i = 1; i < dim; i++){
            fgets(RigaMatrice, LunghezzaRiga, stdin);
            k = 0;
                for(j = 0; j < dim-1; j++){
                    if(i!=j){
                        M[i][j] = 0;
                        while(RigaMatrice[k] != ','){
                            M[i][j] *= 10; 
                            M[i][j] += (int)RigaMatrice[k]-OFFSET;
                            k++;
                        }
                    }else{
                        while(RigaMatrice[k] != ','){
                            M[i][j+1] *= 10; 
                            M[i][j+1] += (int)RigaMatrice[k]-OFFSET;
                            k++;
                        }
                    }
                    k++;
                }
                if(i!=j){
                M[i][j] = 0;
                    while(RigaMatrice[k] != '\n'){
                        M[i][j] *= 10; 
                        M[i][j] += (int)RigaMatrice[k]-OFFSET;
                        k++;
                    }
                }
                M[i][0] = 1;
                                          //distanza minima da ogni nodo ≠ 0 non valutata.
            }

     //Nell'imput ho tolto le printf che ho usato per orientarmi/debuggare perchè erano veramente troppe >.<

//Dijkstra
            
            SiMinimo = 1;
            M[Nodo_Min][0] = 0;                         //Riga che sto per valutare: la "tolgo" dall'insieme Q
            while(SiMinimo){
                i = Nodo_Min;
                SiMinimo = 0;
                Min = MAX;
                for(j=1; j < dim; j++){
                    //printf("A:  i: %u, j: %u, Dist[j]: %u\n", i, j, Dist[j]);
                    if(i != j && M[j][0]){
                        sum = M[i][i] + M[i][j];
                        if((M[j][j] > sum && M[i][j]) || (!M[j][j] && M[i][j]) )
                            M[j][j] = sum;

                        if(M[j][j] < Min && M[j][j]){//entrerò in questo ciclo ogni volta che ci sarà un nuovo cammino minimo,
                            Min = M[j][j];
                            Nodo_Min = j;
                            SiMinimo = 1;
                            }
                        //printf("B:  i: %u, j: %u, Dist[j]: %u\n", i, j, Dist[j]);
                       }
                       //puts("nessuna modifica");
                }
                M[Nodo_Min][0] = 0;     //Riga che sto per valutare: la "tolgo" dall'insieme Q
                                        //indico che il nodo Nodo_Min è stato scelto come minimo tra i nodi rimanen
            }//DIJKSTRA

//Somma Cammini Minimi:
            Valore = 0;
            for(i = 0; i < dim; i++){
                Valore += M[i][i];
            }

//ordino la classifica come maxheap:

            UscitaRapida = 0;
            if(IniziaClassifica == podio){
                classifica[0].value = Valore;
                classifica[0].grafo = num_graph;
                IniziaClassifica--;
            }else if(IniziaClassifica){
                i = podio - IniziaClassifica;
                if(Valore > classifica[0].value){
                    classifica[i].value = classifica[0].value;
                    classifica[i].grafo = classifica[0].grafo;
                    classifica[0].value = Valore;
                    classifica[0].grafo = num_graph;
                }else{                                             //altrimenti il mio nuovo valore va' in fondo e poi 'heapify'
                    classifica[i].value = Valore;
                    classifica[i].grafo = num_graph;
                }
                j = (i-1)/2;
                while(j >= 0 && !UscitaRapida){
                    if(classifica[i].value > classifica[j].value){
                        tmp.value = classifica[i].value;
                        tmp.grafo = classifica[i].grafo;
                        classifica[i].value = classifica[j].value;
                        classifica[i].grafo = classifica[j].grafo;
                        classifica[j].value = tmp.value;
                        classifica[j].grafo = tmp.grafo;
                        i = j;
                        j = (i-1)/2;
                    }else
                        UscitaRapida = 1;
                }//ordino heap dal basso
            IniziaClassifica--;
            }else if(Valore < classifica[0].value){                // c'è un valore da inserire in classifica (minore del massimo)
                classifica[0].value = Valore;
                classifica[0].grafo = num_graph;
                i = 0;
                while(!UscitaRapida){
                    j = (i * 2) + 1;
                    if(j < podio){
                        if(j+1 < podio){
                            if(classifica[i].value >= classifica[j].value){
                                if(classifica[i].value < classifica[j+1].value){
                                    tmp.value = classifica[i].value;
                                    tmp.grafo = classifica[i].grafo;
                                    classifica[i].value = classifica[j+1].value;
                                    classifica[i].grafo = classifica[j+1].grafo;
                                    classifica[j+1].value = tmp.value;
                                    classifica[j+1].grafo = tmp.grafo;
                                    i = j+1;
                                }else
                                    UscitaRapida = 1;
                            }else{
                                if(classifica[i].value >= classifica[j+1].value){
                                    tmp.value = classifica[i].value;
                                    tmp.grafo = classifica[i].grafo;
                                    classifica[i].value = classifica[j].value;
                                    classifica[i].grafo = classifica[j].grafo;
                                    classifica[j].value = tmp.value;
                                    classifica[j].grafo = tmp.grafo;
                                    i = j;
                                }else{
                                    if(classifica[j].value < classifica[j+1].value){
                                        tmp.value = classifica[i].value;
                                        tmp.grafo = classifica[i].grafo;
                                        classifica[i].value = classifica[j+1].value;
                                        classifica[i].grafo = classifica[j+1].grafo;
                                        classifica[j+1].value = tmp.value;
                                        classifica[j+1].grafo = tmp.grafo;
                                        i = j+1;
                                    }else{
                                        tmp.value = classifica[i].value;
                                        tmp.grafo = classifica[i].grafo;
                                        classifica[i].value = classifica[j].value;
                                        classifica[i].grafo = classifica[j].grafo;
                                        classifica[j].value = tmp.value;
                                        classifica[j].grafo = tmp.grafo;
                                        i = j;
                                    }
                                }
                            }
                        }else{
                            if(classifica[i].value < classifica[j].value){
                                tmp.value = classifica[i].value;
                                tmp.grafo = classifica[i].grafo;
                                classifica[i].value = classifica[j].value;
                                classifica[i].grafo = classifica[j].grafo;
                                classifica[j].value = tmp.value;
                                classifica[j].grafo = tmp.grafo;
                                i = j;
                            }else
                                UscitaRapida = 1;    
                        }
                    }else
                        UscitaRapida = 1;
                }//ordino heap dall'alto
            }
            num_graph++;

//debugging heap:
            /*for(i = 0; i < podio; i++)
             *  printf("Il grafo numero %lu con Peso %lu è in posizione %d. Valore controllo: %lu\n", classifica[i][1], classifica[i][0], i, Valore);
             *printf("\n\n\n");*/
        }//fine ciclo 'AggiungiGrafo'
////Stampa:
        if(str_in[0] == 'T'){
            if(IniziaClassifica != podio){
                for(i=0; i < podio - 1 && i < num_graph - 1; i++){                          //stdout
                    printf("%u ", classifica[i].grafo);
                }
                printf("%u", classifica[i].grafo);
            }
            puts("");
                //printf("Valore grafo: %lu\n", classifica[i][0]);
        }//Fine comando 'TopK'

	}//Fine ciclo gets()

    return 0;

}//FINE PROGRAMMA


//comando di sottoposizione:  gcc main.c -Wall -Werror -std=gnu11 -pipe -static -s -o Main
