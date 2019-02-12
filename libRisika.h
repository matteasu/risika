//
// Created by Matteo on 24/01/2019.
//

#ifndef RISIKA_LIBRISIKA_H
#define RISIKA_LIBRISIKA_H

#include "stdio.h"
#include "malloc.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

//enumerazioni
typedef enum {
    Studi_Umanistici,
    Scienze_Economiche_Giuridiche_Politiche,
    Medicina_Chirurgia,
    Biologia_Farmacia,
    Ingegneria_Architettura,
    Facolta_di_Scienze
} Facolta;
//typedef enum {Rosso,Nero,Viola,Verde,Giallo,Blu}Colore;
typedef enum {
    Caffe, Birra, Vino, Jolly
} Arma;

typedef struct {
    int id;
    char nome[10];
    _Bool inUse;
} Colore;

// struttura territorio
typedef struct {
    int id;
    char nome[24];
    Facolta f;
} Territorio;

//struttura nodo per la lista delle carte
typedef struct {
    Arma a;
    int idTerritorio;
} Carta;

//Nodo per il mazzo di carte
typedef struct nodoc {
    struct nodoc *prev;
    struct nodoc *next;
    Carta c;
} NodoC;

//strutura per il mazzo di carte
typedef struct {
    NodoC *testa;
    NodoC *coda;
} Mazzo;

//struttura per i territori del g
typedef struct nodot {
    struct nodot *next;
    Carta card;
} NodoT;


//lista territori giocatore
typedef struct {
    NodoT *testa;
} TerritoriG;


// struttura giocatore
typedef struct {
    int id;
    char nome[24];
    Colore c;
    int nArmate;
    TerritoriG t;
} Giocatore;


#define MIN_G 3
#define MAX_G 6
#define N_TERRITORI 26


int leggiGiocatori(int min, int max);

void leggiNome(char c[]);

Giocatore *caricaGiocatori(int nGiocatori);

int generaCasuale(int min, int max);

void ordinaVettore(Giocatore *g, int nGiocatori);

void gioco();

Giocatore *preparazione(int nGiocatori, Mazzo *m);

void sceltaColore(Giocatore *g, int nGiocatori);

void assegnaArmate(Giocatore *g, int nGiocatori);

void stampaGiocatori(Giocatore *g, int nGiocatori);

void importaTerritori(Territorio t[]);

void ass(Mazzo *m, int nCarte);

void distribuisciCarte(int nGioc,Mazzo *m,Giocatore *g);

NodoC *nuovoNodoC();

NodoT *nuovoNodoT();

void rimuoviCarta(Mazzo *sj);

void importaCarte(Mazzo *m);

void daiCarte(Giocatore g[], Mazzo m,int nGioc,int nCarte);
#endif //RISIKA_LIBRISIKA_H
