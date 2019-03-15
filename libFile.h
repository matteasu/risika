//
// Created by Matteo on 15/03/2019.
//

#ifndef RISIKA_LIBFILE_H
#define RISIKA_LIBFILE_H

#include "stdio.h"
#include "libPrep.h"
#include "libRisika.h"

typedef struct {
    char colore[10];
    int nP;
} Stat;
typedef struct {
    int nGioc;
    int currentP;
    Giocatore g[MAX_G];
    Tabellone t[N_TERRITORI];
    int nCarte;
    int carte[N_CARTE];
} Salvataggio;

void scriviStatistiche(FILE *f, Stat s[]);

void statisticheVittoria(Giocatore *g, Stat s[]);

void leggiStatistiche(FILE *f, Stat s[]);

Salvataggio importaSalvataggio(FILE *f, Mazzo *m, Tabellone t[], int *nGioc, int *currentP, int *nCarte, FILE *log);

Colore assegnaColore(int id);

void creaSalvataggio(FILE *f, int nGiocatori, int currentP, Giocatore *g, int nC, Mazzo m, Tabellone t[]);

#endif //RISIKA_LIBFILE_H
