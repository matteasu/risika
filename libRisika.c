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
    Mazzo m;
    m.testa=NULL;
    nGiocatori=leggiGiocatori(MIN_G,MAX_G);
    g=preparazione(nGiocatori,&m);
    printf("\n \n fuori main\n \n ");
    stampaGiocatori(g,nGiocatori);
}

/**
 * Procedura per la fase di preparazione dei giocatori
 * @param g vettore che conterra' i giocatori
 * @param nGiocatori numero di giocatori
 */
Giocatore * preparazione(int nGiocatori,Mazzo *m){
    Giocatore *g;
    Territorio t[N_TERRITORI];
    NodoC *it;
    g=caricaGiocatori(nGiocatori);
    ordinaVettore(g,nGiocatori);
    sceltaColore(g,nGiocatori);
    assegnaArmate(g,nGiocatori);
    importaTerritori(t);
    importaCarte(m);
    distribuisciCarte(nGiocatori,m);
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
void importaTerritori(Territorio t[]){
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

void importaCarte(Mazzo *m){
    FILE *f=fopen("carte.txt","r");
    NodoC *it,*prev,*nuovo;
    int a,t;
    if(f==NULL){
        printf("caaaazzo\n");
        exit(-1);
    }else{
        while(fscanf(f,"%d",&a)!=EOF){
            fscanf(f,"%d",&t);
            if(m->testa==NULL){
                it=nuovoNodo();
                it->c.a=a;
                it->c.idTerritorio=t;
                it->next=NULL;
                it->prev=NULL;
                m->testa=it;
            }else{
                //creo il nuovo nodo che dovra' contenere le informazioni
                nuovo=nuovoNodo();
                nuovo->c.idTerritorio=t;
                nuovo->c.a=a;
                nuovo->next=NULL;
                //cerco l'ultimo nodo nella lista e mi salvo il precedente
                it=m->testa;
                while(it->next!=NULL){
                    prev=it;
                    it=it->next;
                }
                nuovo->prev=prev;
                it->next=nuovo;
            }


        }
    }
}

void distribuisciCarte(int nGioc,Mazzo *m){
    NodoC *it,*s,*prev;
    Mazzo *sJ;
    sJ->testa=NULL;
    sJ->coda=NULL;
    it=m->testa;
    s=sJ->testa;
    //copia delle carte escludendo i jolly
    while(it->c.a!=3){
      if(s==NULL){
          s=nuovoNodo();
          s->c=it->c;
          s->next=nuovoNodo();
          s->prev=NULL;
          it=it->next;
          sJ->testa=s;
      }else{
          prev=s;
          s=s->next;
          s->c=it->c;
          s->next=nuovoNodo();
          s->prev=prev;
          it=it->next;
          sJ->coda=s;

      }

    }
    s=sJ->testa;
    int i=0;
    while(s->next!=NULL){
        printf("a %d- t %d ",s->c.a,s->c.idTerritorio);
        s=s->next;

    }
    //ass asuniStupidSorting
    ass(sJ,26);
    s=sJ->testa;
    printf("Looking for assss\n");
    while(s->next!=NULL){
        printf("a %d- t %d   ",s->c.a,s->c.idTerritorio);
        s=s->next;

    }


}

void ass(Mazzo *m,int nCarte){
    int i=0,nIterazioni,j;
    NodoC *it=m->testa;
    NodoC *app;
    Carta c;
    _Bool ok=false;
    for(i=0;i<nCarte;i++){
        do {
            nIterazioni = generaCasuale(0, nCarte )+generaCasuale(0,2);
            if(nIterazioni<=26){
                    ok=true;
                }else
                    ok=false;
        }while(ok!=true);
        app=m->testa;
        for(j=0;j<nIterazioni;j++){
            app=app->next;
        }
        c=it->c;
        it->c=app->c;
        app->c=c;
    }
}




NodoC * nuovoNodo(){
    NodoC *nuovoNodo = (NodoC*) malloc(sizeof(NodoC)); //Allocazione

    if (nuovoNodo == NULL) //Controllo errori
        exit(-1);

    return nuovoNodo;
}