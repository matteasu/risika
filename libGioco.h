//
// Created by Matteo on 15/03/2019.
//

#ifndef RISIKA_LIBGIOCO_H
#define RISIKA_LIBGIOCO_H

#include <stdio.h>
#include <stdbool.h>
#include "libPrep.h"
#include "libRisika.h"

void rinforzo(Giocatore *g, Tabellone t[], Mazzo *m);

int bonusCarte(Giocatore *g, Tabellone t[], Mazzo *m);

void attacco(Giocatore *g, Giocatore giocatori[], Tabellone t[], int *idP, Mazzo *m);

_Bool baseAttacco(Giocatore *g, Tabellone t[], int *tB);

int richiestaNumeroArmate(Giocatore g, int caso);

_Bool sceltaTerritorioAttacco(Giocatore g, Tabellone t[], int tB, int *tA);

void attacca(Giocatore *g1, Giocatore *g2, Tabellone t[], int tA, int tB, int nA, int nAD, int *idP, Mazzo *m);

Giocatore *rimuoviGiocatore(Giocatore *g, int pos, int nGiocatori, Tabellone t[]);

void sistemaTabellone(Tabellone t[], int oldId, int newId);

void spostamentoStrategio(Giocatore *g, Tabellone t[]);

void finePartita(Giocatore *g, FILE *log);

_Bool fineGioco(int nGiocatori);

#endif //RISIKA_LIBGIOCO_H
