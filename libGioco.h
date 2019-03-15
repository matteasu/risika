//
// Libreria contenente le funzioni e le procedure usate per le fasi di gioco
//

#ifndef RISIKA_LIBGIOCO_H
#define RISIKA_LIBGIOCO_H
#define NUM_SU 5 //numero facolta studi umanistici
#define NUM_SE 3 //numero facolta scienze economice
#define NUM_MC 4 //numero facolta medicina e chirurgia
#define NUM_BF 5 //numero facolta biologia e farmacia
#define NUM_ING 5 //numero facolta ing
#define NUM_FS 4 //numero facolta scientifiche
#define INCR_SU 3 //incremento studi umanistici
#define INCR_SE 2 //incremento scienze economice
#define INCR_MC 2 //incremento medicina e chirurgia
#define INCR_BF 4 //incremento biologia e farmacia
#define INCR_ING 4 //incremento ing
#define INCR_FS 2 //incremento facolta scientifiche
#include <stdio.h>
#include <stdbool.h>
#include "libPrep.h"
#include "libRisika.h"

void rinforzo(Giocatore *g, Tabellone t[], Mazzo *m, FILE *log);

int bonusCarte(Giocatore *g, Tabellone t[], Mazzo *m);

void attacco(Giocatore *g, Giocatore giocatori[], Tabellone t[], int *idP, Mazzo *m, FILE *log);

_Bool baseAttacco(Giocatore *g, Tabellone t[], int *tB);

int richiestaNumeroArmate(Giocatore g, int caso);

_Bool sceltaTerritorioAttacco(Giocatore g, Tabellone t[], int tB, int *tA);

void
attacca(Giocatore *g1, Giocatore *g2, Tabellone t[], int tA, int tB, int nA, int nAD, int *idP, Mazzo *m, FILE *log);

Giocatore *rimuoviGiocatore(Giocatore *g, int pos, int nGiocatori, Tabellone t[], FILE *log);

void sistemaTabellone(Tabellone t[], int oldId, int newId);

void spostamentoStrategio(Giocatore *g, Tabellone t[], FILE *log);

void finePartita(Giocatore *g, FILE *log);

_Bool fineGioco(int nGiocatori);

#endif //RISIKA_LIBGIOCO_H
