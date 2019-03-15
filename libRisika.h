//
// Libreria contenente le macro principali e le funzioni generiche che vengono chiamate piu' volte durante l'esecuzione
//

#ifndef RISIKA_LIBRISIKA_H
#define RISIKA_LIBRISIKA_H

#define N_TERRITORI 26
#define NUM_FACOLTA 6

#define F_LOG "log.txt"
#define F_SAVE "Salvataggio.rsk"
#define F_STAT "stat"

#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "libRisika.h"
#include "docente.h"
#include "libPrep.h"
#include "libGioco.h"
#include "libFile.h"


typedef enum {
    Rosso, Nero, Viola, Verde, Giallo, Blu
} Colori;


typedef struct {
    int nf;
    int incr;
} Incrementi;


int generaCasuale(int min, int max);

void gioco();

void stampaGiocatori(Giocatore *g, int nGiocatori, Tabellone t[]);

void importaTerritori(Tabellone t[]);


NodoC *nuovoNodoC();

void stampaNomeIdTerritorio(int id, Tabellone t[]);

void stampaNomeTerritorio(int id, Tabellone t[]);

void armateInT(Giocatore *g, Tabellone t[], int nRip, int nA, FILE *log);

void importaCarte(Mazzo *m);

void inserimentoInCoda(NodoC *testa, Carta c);

void pulisciConsole();

int contaTerritoriGiocatore(Tabellone t[], int id);

NodoC *inserimentoInTesta(Carta c);

void rimuoviCarta(Mazzo *m);

int contaCarte(Mazzo *m);

int trovaMax(int v[], int n);

void contaArmateG(Tabellone t[], Giocatore *g);

Carta recuperaCarta(Mazzo *m, int el);


#endif //RISIKA_LIBRISIKA_H
