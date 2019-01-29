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
    g=preparazione(nGiocatori);
    int i;
    printf("\n \n fuori main\n \n ");
    stampaGiocatori(g,nGiocatori);
}

/**
 * Procedura per la fase di preparazione dei giocatori
 * @param g vettore che conterra' i giocatori
 * @param nGiocatori numero di giocatori
 */
Giocatore * preparazione(int nGiocatori){
    Giocatore *g;
    int i;
    Territorio t[N_TERRITORI];
    g=caricaGiocatori(nGiocatori);
    ordinaVettore(g,nGiocatori);
    sceltaColore(g,nGiocatori);
    assegnaArmate(g,nGiocatori);
    importaDaFile(t);
    return g;
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
    }while(x<min || x>max);
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
Giocatore * caricaGiocatori(int nGiocatori){
    Giocatore *g=NULL;
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

    }
    return g;
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


void sceltaColore(Giocatore *g,int nGiocatori){
    int i,j,col;

    int nColori=6;
    Colore colori[6]={{0,"Rosso",false},{1,"Nero",false},{2,"Viola",false},{3,"Verde",false},{4,"Giallo",false},{5,"Blu",false}};

    for(i=0;i<nGiocatori;i++){
        printf("%s scegli uno dei seguenti colori:\n", g[i].nome);
        for (j = 0; j < nColori; j++) {
            if(colori[j].inUse==false)
                printf("%d - %s \n", colori[j].id,colori[j].nome);
        }
        scanf("%d",&col);
        colori[col].inUse=true;
        g[i].c=colori[col];
    }
}

void stampaGiocatori(Giocatore *g,int nGiocatori){
    int i;
    for(i=0;i<nGiocatori;i++){
        printf("Giocatore: %d\n Nome: %s\n Colore: %s\n Numero Armate: %d \n \n",g[i].id,g[i].nome,g[i].c.nome,g[i].nArmate);
    }
}

void assegnaArmate(Giocatore *g,int nGiocatori){
    int num,i;
    switch (nGiocatori) {
        case 3:
            num = 35;
            break;
        case 4:
            num = 30;
            break;
        case 5:
            num = 25;
        case 6:
            num = 20;
            break;
        default:
            break;
    }
    for(i=0;i<nGiocatori;i++){
        g[i].nArmate=num;
    }
}
/**
 * procedura per l'importazione dei territori e la relativa facolta' da file di testo
 * @param t vettore dove verranno inseriti i territori
 */
void importaDaFile(Territorio t[]){
    FILE *f=fopen("facolta.txt","r");
    int i=0;
    if(f==NULL){
        printf("caaaazzo\n");
        exit(-1);
    }else {

        while (fscanf(f, "%s", t[i].nome) != EOF) {
            t[i].id = i;
            fscanf(f, "%d", &t[i].f);
            i++;
        }
    }
    fclose(f);

}