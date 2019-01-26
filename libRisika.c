//
// Created by Matteo on 24/01/2019.
//

#include "libRisika.h"

/**
 * Prodedura generale per il funzionamento del gioco
 * qua verrano dichiarate tutte le viariabili e verrano chiamate le procedure
 * necessarie al corretto svolgimento del gioco
 */
void gioco(){
    Giocatore *g=NULL;
    int nGiocatori;
    nGiocatori=leggiGiocatori(MIN_G,MAX_G);
    preparazione(g,nGiocatori);
}

/**
 * Procedura per la fase di preparazione dei giocatori
 * @param g vettore che conterra' i giocatori
 * @param nGiocatori numero di giocatori
 */
void preparazione(Giocatore *g,int nGiocatori){
    caricaGiocatori(nGiocatori,g);
    sceltaColore(g,nGiocatori);
}



/**
 * Funzione che legge il numero di giocatori e si assicura che sia un valore accettabile
 * @param min
 * @param max
 * @return numeroGiocatori
 */
int leggiGiocatori(int min,int max) {
    int x;
    do {
        printf("Inserisci il numero di giocatori\n Minimo %d Massimo %d :", min, max);
        scanf("%d", &x);
    }while(x<min && x>max);
    return x;
}
/**
 * Procedura per leggere il nome del giocatore
 * @param campo della struttura su cui andro' a scrivere
 */
void leggiNome(char c[]){

    printf("Inserisci il nome del giocatore, max 24 caratteri!!\n altrimenti il nome verra' tagliato :) \n");
    scanf("%s",c);

}


/**
 * Procedura per creare e caricare il vettore dinamico coi giocatori
 * @param nGiocatori numero di giocatori
 * @param g puntatore allla prima posizione
 */
void caricaGiocatori(int nGiocatori,Giocatore *g){
    g=(Giocatore*)malloc(sizeof(Giocatore)*nGiocatori);
    if(g==NULL) {
        exit(-1);
    }else{
        int i;
        for(i=0;i<nGiocatori;i++){
            leggiNome(g[i].nome);
            g[i].id=i;
        }
        printf("\n");
        printf("Stampa dei giocatori ordinati \n");
        for(i=0;i<nGiocatori;i++){
            printf("%s \n",g[i].nome);

        }
        printf("\n");
        ordinaVettore(g,nGiocatori);
    }
}



/**
 * Funzione per la generazione casuale
 * @param min
 * @param max
 * @return
 */
int generaCasuale(int min,int max){
    int r;
    r=min+rand()%(max-min+1);
    return r;
}
/**
 * Procedura per l'ordinamento del vettore di giocatori
 * @param g vettore coi giocatori
 * @param nGiocatori numero di giocatori
 */
void ordinaVettore(Giocatore *g,int nGiocatori){
    int primoGiocatore=generaCasuale(0,nGiocatori-1);
    //ordinamento del vettore
    Giocatore app[nGiocatori];
    int i,j;
    //copio il vettore
    for(i=0;i<nGiocatori;i++){
        app[i]=g[i];
    }
    i=primoGiocatore;
    j=0;
    //copio da primo giocatore a nGiocatori
    while(i<nGiocatori){
        g[j]=app[i];
        g[j].id=j;
        j++;
        i++;
    }
    //j=j+1;
    i=0;
    //giocatori precedenti al primo giocatore
    while(i<primoGiocatore){
        g[j]=app[i];
        g[j].id=j;
        j++;
        i++;
    }
    printf("Stampa dei giocatori ordinati \n");
    for(i=0;i<nGiocatori;i++){
        printf("%s - %d \n",g[i].nome,g[i].id);

    }
}

//<-- procedura da terminare -->
void sceltaColore(Giocatore *g,int nGiocatori){
    int i,j;
    int nColori=6;
    _Bool inUse=true;
    Colore c[6]={Rosso,Nero,Viola,Verde,Giallo,Blu};
    for(i=0;i<nGiocatori;i++){
        do {
            printf("%s scegli uno dei seguenti colori:\n", g[i].nome);
            for (j = 0; j < nColori; j++) {
                printf("%d \n", c[j]);
            }
            scanf("%d",g[i].c);

        }while(inUse!=true);


    }
}


