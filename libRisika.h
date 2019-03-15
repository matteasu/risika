//
// Created by Matteo on 24/01/2019.
//

#ifndef RISIKA_LIBRISIKA_H
#define RISIKA_LIBRISIKA_H
#define MIN_G 3
#define MAX_G 6
#define N_TERRITORI 26
#define NUM_FACOLTA 6
#define NUM_SU 5 //numero facolta studi umanistici
#define NUM_SE 3 //numero facolta scienze economice
#define NUM_MC 4 //numero facolta medicina e chirurgia
#define NUM_BF 5 //numero facolta biologia e farmacia
#define NUM_ING 5 //numero facolta ing
#define NUM_FS 4 //numero facolta scientifiche
#define F_LOG "log.txt"

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
void armateInT(Giocatore *g, Tabellone t[], int nRip, int nA);
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
