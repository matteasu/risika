#include <stdio.h>
#include "libRisika.h"

int main() {
    srand(time(NULL));
    Giocatore *g;
    int nGiocatori = 3;
    Mazzo m, sj;
    sj.testa = NULL;
    m.testa = NULL;
    Tabellone t[N_TERRITORI];
    g = caricaGiocatori(nGiocatori);
    sceltaColore(g, nGiocatori);
    //importaCarte(&m);
    //importaTerritori(t);
    //assegnaArmate(g,nGiocatori);
    int i;
    //ass(&m,28);
    NodoC *it;
    it = m.testa;
    /*
    while(it!=NULL){
        if(it->c.a!=3){
            if(sj.testa==NULL){
                sj.testa=inserimentoInTesta(it->c);
                it=it->next;
            }else{
                inserimentoInCoda(sj.testa,it->c);
                it=it->next;
            }
        }else{
            it=it->next;
        }
    }
    for(i=0;i<9;i++){
        if(m.testa->c.a!=3){
            if(g[0].t.testa==NULL)
                g[0].t.testa=inserimentoInTesta(sj.testa->c);
            else
                inserimentoInCoda(g[0].t.testa,sj.testa->c);
            rimuoviCarta(&sj);
        }else
            rimuoviCarta(&sj);
    }

        g[1].t.testa = inserimentoInTesta(sj.testa->c);
        rimuoviCarta(&sj);

    for(i=0;i<16;i++){
        if(m.testa->c.a!=3){
            if(g[2].t.testa==NULL)
                g[2].t.testa=inserimentoInTesta(sj.testa->c);
            else
                inserimentoInCoda(g[2].t.testa,sj.testa->c);
            rimuoviCarta(&sj);
        }else
            rimuoviCarta(&sj);
    }
    for(i=0;i<nGiocatori;i++){
        it=g[i].t.testa;
        while(it!=NULL){
            t[it->c.idTerritorio].idPropietario = g[i].id;
            t[it->c.idTerritorio].nArmate = 1;
            g[i].nArmate--;
            g[i].nArmateinG++;
            it = it->next;
        }
    }
     */
    stampaGiocatori(g, nGiocatori, t);
    pulisciConsole();
    g = rimuoviGiocatore(g, 1, nGiocatori);
    nGiocatori--;
    stampaGiocatori(g, nGiocatori, t);
    /*
    for(i=0;i<nGiocatori;i++){
        rinforzo(&g[i],t);
    }
    while(fineGioco(g,nGiocatori)!=true){
        for(i=0;i<nGiocatori;i++){
            attacco(&g[i],g,t);
             if(eliminaGiocatore(&g[i], g[i].id,t, nGiocatori, g)){
                 nGiocatori--;
                 stampaGiocatori(g,nGiocatori,t);
             }
        }
    }*/
    //gioco();
    return 0;
}