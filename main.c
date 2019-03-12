#include <stdio.h>
#include "libRisika.h"

int main() {
    srand(time(NULL));
    /*
    Giocatore *g;
    int nGiocatori =3;
    Mazzo m, sj;
    int idp=-1;
    FILE *f=NULL;
    f=fopen(F_LOG,"a+");
    sj.testa = NULL;
    m.testa = NULL;
    Tabellone t[N_TERRITORI];
    g = caricaGiocatori(nGiocatori,f);
    sceltaColore(g, nGiocatori,f);
    //importaCarte(&m);
    importaTerritori(t);
    assegnaArmate(g,nGiocatori,f);
    int i,j;
    //ass(&m,28);
    NodoC *it;
    it = m.testa;
    for(i=0;i<20;i++){
        t[i].idPropietario=0;
        t[i].nArmate=5;
    }

    t[i].idPropietario=1;
    t[i].nArmate=1;
    i++;
    for(j=0;j<5;j++){
        t[i].idPropietario=2;
        t[i].nArmate=1;
        i++;
    }
    //stampaGiocatori(g, nGiocatori, t);
    for(i=0;i<nGiocatori;i++){
        printf("%d \n",contaTerritoriGiocatore(t,i));
    }
    while(fineGioco(nGiocatori)!=true){
        for(i=0;i<nGiocatori;i++){
            rinforzo(&g[i],t);
            //attacco(&g[i],g,t,&idp);
            t[9].idPropietario=0;
            idp=1;
            if(idp!=-1){
                g=rimuoviGiocatore(g,idp,nGiocatori,t);
                nGiocatori--;
                //stampaGiocatori(g,nGiocatori,t);
            }
            stampaGiocatori(g,nGiocatori,t);
        }
    }
    free(g);
     */
    gioco();
    return 0;
}