//
// Created by Matteo on 24/01/2019.
//

#ifndef RISIKA_LIBRISIKA_H
#define RISIKA_LIBRISIKA_H

#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "docente.h"


#define MIN_G 3
#define MAX_G 6
#define N_TERRITORI 26
#define N_CARTE 28
#define N_CARTESJ 26
#define TREAINT 1 //3 armate nello stesso territorio
#define DUEAINTET 2//2 armate in un territorio e una in un altro
#define TREDIVERSO 3 //3 armate in 3 territori diversi
#define DUEINT 4//2 armate in un territorio
#define DUET 5//una armata in un territorio e una in un altro
#define AINT 6//una armata in un territorio
#define NUM_FACOLTA 6
#define NUM_SU 5 //numero facolta studi umanistici
#define NUM_SE 3 //numero facolta scienze economice
#define NUM_MC 4 //numero facolta medicina e chirurgia
#define NUM_BF 5 //numero facolta biologia e farmacia
#define NUM_ING 5 //numero facolta ing
#define NUM_FS 4 //numero facolta scientifiche
#define F_LOG "log.txt"



//enumerazioni
typedef enum {
    Studi_Umanistici,
    Scienze_Economiche_Giuridiche_Politiche,
    Medicina_Chirurgia,
    Biologia_Farmacia,
    Ingegneria_Architettura,
    Facolta_di_Scienze
} Facolta;
typedef enum {
    Rosso, Nero, Viola, Verde, Giallo, Blu
} Colori;
typedef enum {
    Caffe, Birra, Vino, Jolly
} Arma;

typedef struct {
    int id;
    char nome[10];
    _Bool inUse;
} Colore;

typedef struct {
    Colore c;
    int npv;
} Statistiche;

// struttura territorio
typedef struct {
    int id;
    char nome[24];
    Facolta f;
} Territorio;

typedef struct {
    Territorio t;
    int idPropietario;
    int nArmate;
} Tabellone;

//struttura nodo per la lista delle carte
typedef struct {
    Arma a;
    int idTerritorio;
    int idCarta;
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

} Mazzo;

//lista territori giocatore
typedef struct {
    NodoC *testa;
} TerritoriG;

typedef struct {
    NodoC *testa;
} Carte;

// struttura giocatore
typedef struct {
    int id;
    char nome[24];
    Colore c;
    int nArmate;
    int nCarte;
    int nArmateinG;
    TerritoriG t;
    Carte ca;
} Giocatore;


typedef struct {
    int nf;
    int incr;
} Incrementi;

typedef struct {
    int nGioc;
    int currentP;
    Giocatore g[MAX_G];
    Tabellone t;
    int nCarte;
    int carte[N_CARTE];
} Salvataggio;




int leggiGiocatori(int min, int max);

void leggiNome(char c[]);

Giocatore *caricaGiocatori(int nGiocatori, FILE *f);

int generaCasuale(int min, int max);

void ordinaVettore(Giocatore *g, int nGiocatori, FILE *f);

void gioco();

void preparazione(Giocatore *g, int nGiocatori, Mazzo *m, Tabellone t[], FILE *f);

void sceltaColore(Giocatore *g, int nGiocatori, FILE *f);

void assegnaArmate(Giocatore *g, int nGiocatori, FILE *f);

void stampaGiocatori(Giocatore *g, int nGiocatori, Tabellone t[]);

void importaTerritori(Tabellone t[]);

void ass(Mazzo *m, int nCarte);

void distribuisciCarte(int nGioc,Mazzo *m,Giocatore *g);

NodoC *nuovoNodoC();

void rimuoviCarta(Mazzo *m);

void posizionaArmate(Giocatore *g, Tabellone t[], int scelta);

void stampaNomeTerritorio(int id, Tabellone t[]);

void armateInT(Giocatore *g, Tabellone t[], int nRip, int nA);
void importaCarte(Mazzo *m);

Giocatore *rimuoviGiocatore(Giocatore *g, int pos, int nGiocatori, Tabellone t[]);
void inserimentoInCoda(NodoC *testa, Carta c);

void bonusCarte(Giocatore *g, Tabellone t[]);

_Bool sceltaTerritorioAttacco(Giocatore g, Tabellone t[], int tB, int *tA);
void pulisciConsole();

_Bool eliminaGiocatore(Giocatore *g, int id, int nGioc, Giocatore *giocatori, Tabellone t[]);

void sistemaTabellone(Tabellone t[], int oldId, int newId);
_Bool baseAttacco(Giocatore *g, Tabellone t[], int *tB);

int contaTerritoriGiocatore(Tabellone t[], int id);
NodoC *inserimentoInTesta(Carta c);
void assegnaArmateTerritori(int nGiocatori, Giocatore g[], Tabellone t[]);
void rinforzo(Giocatore *g,Tabellone t[]);
void daiCarte(Giocatore g[], Mazzo *m, int nGioc, int nCarte);

Salvataggio importaSalvataggio(FILE *f, Mazzo m, Tabellone t[]);
void caricaSalvataggio(FILE *f, int *nGiocatori, int *giocatoreCorrente);
int trovaMax(int v[], int n);

Carta recuperaCarta(Mazzo *m, int el);
int richiestaNumeroArmate(Giocatore g, int caso);

void statisticheVittoria(Giocatore *g, Statistiche stat[]);
void spostamentoStrategio(Giocatore *g, Tabellone t[]);

_Bool fineGioco(int nGiocatori);

void attacca(Giocatore *g1, Giocatore *g2, Tabellone t[], int tA, int tB, int nA, int nAD, int *idP);

Giocatore *nuovaPartita(int *nGiocatori, Mazzo *m, Tabellone t[], FILE *log);
void attacco(Giocatore *g, Giocatore giocatori[], Tabellone t[], int *idP);
#endif //RISIKA_LIBRISIKA_H
