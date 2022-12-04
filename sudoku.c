#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LUNGHEZZA_INPUT 81 //Stringa iniziale
#define DIM 9 //La griglia è 9x9

typedef struct {
    int num;    //Valore dell'elemento in una determinata coordinata, se la casella è vuota questo valore sarà uguale a 0.
    int isDefault; //controlla se l'elemento fa parte della stringa iniziale (1 è default, 0 è inserito dall'utente)
} elemento_t;

void avviaPartita(elemento_t campo[][DIM], int dim, char schema_iniziale[]);
void stampaMappa(elemento_t campo[][DIM], int dim);
void stampaElemento(elemento_t el);
void visualizzaMenu(elemento_t campo[][DIM], char schema_iniziale[]);
void inserisciValore(elemento_t campo[][DIM], char schema_iniziale[]);
void cancellaValore(elemento_t campo[][DIM], char schema_iniziale[]);
int cercaElemento(elemento_t campo[][DIM], int riga, int colonna);
int verificaVittoria(elemento_t campo[][DIM]);
void caricaSoluzione(elemento_t campo[][DIM], int dim, char schema_iniziale[]);
void riavviaPartita(elemento_t campo[][DIM], char schema_iniziale[]);
int checkUltimo(elemento_t campo[][DIM], int dim);
void esci();





int main(){
    char schema_iniziale[LUNGHEZZA_INPUT+1];
    elemento_t campo[DIM][DIM];

    int play_again;

    do{
        //Avvia partita: fa inserire la stringa corretta e crea il campo.
        avviaPartita(campo, DIM, schema_iniziale);

        //La stringa in avviaPartita() è sempre corretta, perchè il controllo è interno alla funzione,
        //quindi visualizzo mappa e successivamente menù.
        while(checkUltimo(campo, DIM) == 0){
            stampaMappa(campo, DIM);
            visualizzaMenu(campo, schema_iniziale);
        }

        //Implicito che checkUltimo() a questo punto sia true, ovvero abbiamo tutti i numeri
        //e non è stato chiamato exit(0):
        //Chiamo verificaVittoria e controllo solo sconfitta o vittoria,
        //non incmpleto perchè è impossibile (la condizione che ci porta qui è che sia stato inserito
        //l'ultimo numero.)
        if(verificaVittoria(campo) == 0)
            printf("SOLUZIONE ERRATA.\n");
        else {
            printf("Hai vinto. Vuoi fare un'altra partita?\n 1 - Si\n 0 - No\n");
            scanf("%d", &play_again);
        }
    } while(play_again);
            

    return 0;
}





void avviaPartita(elemento_t campo[][DIM], int dim, char schema_iniziale[]){
    
    int i, j, isValid;
    isValid = 0;
    
    do{
        printf("Inserire lo schema della nuova partita\n");
        scanf("%s", schema_iniziale);
        
        for(i=0; i<LUNGHEZZA_INPUT; i++){
            if((schema_iniziale[i]<'0' || schema_iniziale[i]>'9' || schema_iniziale[i] != '_')  && strlen(schema_iniziale) == LUNGHEZZA_INPUT){
                isValid = 1;
            }
        }
    }while(isValid == 0);

    for(i=0;i<dim;i++){
        for(j=0;j<dim;j++){
            if(schema_iniziale[dim*i+j] != '0' && schema_iniziale[dim*i+j] != '_'){
                campo[i][j].num = schema_iniziale[dim*i+j] - '0';
                campo[i][j].isDefault = 1;
            }
            else{
                campo[i][j].num = 0;
                campo[i][j].isDefault = 0;
            }
        }
    }
    
}





void stampaMappa(elemento_t campo[][DIM], int dim){
    int i,j;
    int xCampo, yCampo;

	printf("+ - - - + - - - + - - - +\n");

    for(j=0; j<11; j++){
       for(i=0; i<13; i++){
           if((i==0||i==4||i==8||i==12)&&(j==3||j==7||j==11)){
			    printf("+ ");
           }else if(i==0||i==4||i==8||i==12){
			    printf("| ");
           }else if(j==3||j==7||j==11){
			    printf("- ");
           } else{
               if(i>7){
                   xCampo=i-3;
               }else if(i>4){
                   xCampo=i-2;
               }else if(i>0){
                   xCampo=i-1;
               }
               if(j>7){
                   yCampo=j-2;
               }else if(j>3){
                   yCampo=j-1;
               }else if(j>=0){
                   yCampo=j;
               }
			   
			   //printf("x : %d, y: %d\n", xCampo,yCampo);
               stampaElemento(campo[yCampo][xCampo]);
           }
       }
	    printf("\n"); 
    }
    printf("+ - - - + - - - + - - - +\n");

}





void stampaElemento(elemento_t el){
    if(el.num==0){
        printf("  ");
    }else if(el.isDefault==1){
        printf("%d*",el.num);
    } else{
        printf("%d ",el.num);
    }
}





void visualizzaMenu(elemento_t campo[][DIM], char schema_iniziale[]){
    int x;
    printf("\n[-] | MENU | [-]\n");
    printf("1 - Avvia una nuova partita\n2 - Inserisci valore\n3 - Cancella valore\n4 - Verifica la soluzione attuale\n5 - Carica una soluzione e verificala\n6 - Riavvia partita attuale\n0 - Esci\n");
    do{
        printf("Quale operazione vuoi effettuare? ");
        scanf(" %d", &x);
        if(x == 1){avviaPartita(campo, DIM, schema_iniziale);}
        else if(x == 2){inserisciValore(campo, schema_iniziale);}
        else if(x == 3){cancellaValore(campo, schema_iniziale);}
        else if(x == 4){
            if(verificaVittoria(campo) == 0){
                printf("ERRATA\n");
            }
            else if(verificaVittoria(campo) == 1){
                printf("CORRETTA\n");
            }
            else{
                printf("VALIDA\n");
            }
        }
        else if(x == 5){caricaSoluzione(campo, DIM, schema_iniziale);}
        else if(x == 6){riavviaPartita(campo,schema_iniziale);}
        else if(x == 0){esci();}
        else {
            printf("Non hai inserito un valore valido, riprova");
        }
    } while (x < 1 || x > 6);

    /*
    inserisciValore();
    cancellaValore();
    verificaVittoria();
    riavviaPartita();
    esci();
    */  
}





void inserisciValore(elemento_t campo[][DIM], char schema_iniziale[]) {
    int riga, colonna, valore, flag;

    //Inserimento coordinate
    flag = 0; //flag diventa 0 se c'è un errore.
    printf("Inserisci le coordinate. \n");

    printf("Riga: ");
    scanf("%d", &riga);

    printf("Colonna: ");
    scanf("%d", &colonna);

    if (riga <= 0 || riga > 9 || colonna <= 0 || colonna > 9){
        flag = 1;
        printf("Le coordinate inserite non sono valide.\n");
    }
        
    if(cercaElemento(campo, riga-1, colonna-1)){
        flag = 1;
        printf("Esiste già un elemento in questa posizione.\n");
    }

    if(!flag) {
        printf("Inserisci il valore da inserire: ");
        scanf("%d", &valore);
        if(valore < 1 || valore > 9)
            printf("Valore non accettabile.\n");
        else
            campo[riga-1][colonna-1].num = valore;
    }
        printf("\n");
}





void cancellaValore(elemento_t campo[][DIM], char schema_iniziale[]){
    int riga, colonna;

    printf("Inserisci le coordinate.\n");
    printf("Riga: ");
    scanf("%d", &riga);

    printf("Colonna: ");
    scanf("%d", &colonna);
        
    if(riga <= 0 || riga > 9 || colonna <= 0 || riga > 9 )
        printf("Le coordinate inserite non sono corrette");
    else {
        if ((cercaElemento(campo, riga-1, colonna-1)) && !campo[riga-1][colonna-1].isDefault)
            campo[riga-1][colonna-1].num =  0;
        else
            printf("Le coordinate selezionate non sono valide: la casella non può essere cancellata o è già vuota.\n"); 
    }
    printf("\n");
    
}





int cercaElemento(elemento_t campo[][DIM], int riga, int colonna){ //1 se c'è elemento, 0 altrimenti.
    if(campo[riga][colonna].num != 0)
        return 1;
    else
        return 0;
}





int verificaVittoria (elemento_t campo[][DIM]){
    int i, j, k, larghezza, altezza;
    int quadrato[DIM];
    
    for(i=0;i<DIM;i++){
        for(j=0;j<DIM-1;j++){
            for(k=j+1;k<DIM;k++){
                //controllo righe
                if((campo[i][j].num == campo[i][k].num) && (campo[i][j].num != 0 || campo[i][k].num != 0)){   
                    return 0;
                }

                //controllo colonne
                if((campo[j][i].num == campo[k][i].num) && (campo[j][i].num != 0 || campo[k][i].num != 0)){
                    return 0;
                }
            }      
        }
    }
   //controllo quadrato
    larghezza = 0;
    altezza = 0;
    for(larghezza = 0; larghezza<=6; larghezza = larghezza+3){
        for(altezza = 0; altezza<=6; altezza = altezza+3){
            for(i=0;i<3;i++){
                for(j=0;j<3;j++){
                    quadrato[3*i+j] = campo[altezza+i][larghezza+j].num;
                } 
            }
            for(i=0;i<DIM-1;i++){
                for(j=i+1;j<DIM;j++){
                    if(quadrato[i] != 0 && (quadrato[i] == quadrato[j])){
                        return 0;
                    }
                }
            }
        }
    }
     
        

    for(i=0;i<DIM;i++){
        for(j=0;j<DIM;j++){
            if(campo[i][j].num==0){
                return 2; //Valido
            }else{
                return 1;//Hai vinto
            }
        }
    }
        

}





void riavviaPartita(elemento_t campo[][DIM], char schema_iniziale[]){
    //Controlla ogni elemento nella graglia, e resetta a 0 ogni elemento non default
    int i, j;
    
    for(i=0; i<DIM; i++)
        for(j=0; j<DIM; j++)
            if(!campo[i][j].isDefault)
                campo[i][j].num = 0;
    
    printf("Ho riavviato la partita.\n");
    /*
    stampaMappa();
    visualizzaMenu();
    */
}





void caricaSoluzione(elemento_t campo[][DIM], int dim, char schema_iniziale[]){
    int i, j, flag;
    elemento_t campo_inserito[DIM][DIM];
    char soluzione_inserita[LUNGHEZZA_INPUT+1];
    do{
        printf("Carica una soluzione completa o parziale: ");
        scanf("%s", soluzione_inserita);
    }while(strlen(soluzione_inserita) != LUNGHEZZA_INPUT);

    flag = 1;
    for(i=0; i<dim && flag; i++)
        for(j=0; j<dim && flag; j++)
            if(campo[i][j].isDefault == 1)
                if(schema_iniziale[dim*i+j] != soluzione_inserita[dim*i+j])
                    flag = 0;
    
    if(flag){
        for(i=0; i < dim; i++){
            for(j = 0; j<dim; j++){
                if(soluzione_inserita[dim*i+j] != '0' && soluzione_inserita[dim*i+j] != '_'){
                    campo_inserito[i][j].num = soluzione_inserita[dim*i+j] - '0';
                    if(campo[i][j].isDefault == 1)
                        campo_inserito[i][j].isDefault = 1;
                    else
                        campo_inserito[i][j].isDefault = 0;
                }
                else{
                    campo_inserito[i][j].num = 0;
                    campo_inserito[i][j].isDefault = 0;
                }
            }
        }
        if(verificaVittoria(campo_inserito) == 0){
            printf("Soluzione inserita ERRATA\n");
            //stampaMappa(campo, DIM);
            //visualizzaMenu(campo, schema_iniziale);
        }
        else if(verificaVittoria(campo_inserito) == 2){
            printf("Soluzione inserita VALIDA\n");
            for(i=0;i<dim;i++){
                for(j=0;j<dim;j++){
                    campo[i][j] = campo_inserito[i][j];
                }
            }
            //stampaMappa(campo, DIM);
            //visualizzaMenu(campo, schema_iniziale);
        }
        else{
            for(i=0;i<dim;i++){
                for(j=0;j<dim;j++){
                    campo[i][j] = campo_inserito[i][j];
                }
            }
            printf("Soluzione inserita CORRETTA\n");
            
        }
    }

    if(!flag)
        printf("Errore.\n");
                    
}




int checkUltimo(elemento_t campo[][DIM], int dim){
    int i, j;

    for(i=0; i<dim; i++)
        for(j=0; j<dim; j++)
            if(campo[i][j].num == 0) //Se si, non è l'ultimo
                return 0;

    return 1; //Solo se sono sttai inseriti tutti gli elementi
}





void esci(){
    exit(0);
}
