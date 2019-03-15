//
// Created by Matteo on 15/03/2019.
//

#ifndef RISIKA_LIBPREP_H
#define RISIKA_LIBPREP_H
// struttura giocatore
#include <stdio.h>

#define N_CARTE 28
#define N_CARTESJ 26
#define TREAINT 1 //3 armate nello stesso territorio
#define DUEAINTET 2//2 armate in un territorio e una in un altro
#define TREDIVERSO 3 //3 armate in 3 territori diversi
#define DUEINT 4//2 armate in un territorio
#define DUET 5//una armata in un territorio e una in un altro
#define AINT 6//una armata in un territorio
typedef enum {
    Studi_Umanistici,
    Scienze_Economiche_Giuridiche_Politiche,
    Medicina_Chirurgia,
    Biologia_Farmacia,
    Ingegneria_Architettura,
    Facolta_di_Scienze
} Facolta;

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
    int id;
    char nome[24];
    Colore c;
    int nArmate;
    int nCarte;
    int nArmateinG;
    TerritoriG t;
    Mazzo cg;
    int vCarte[N_CARTE];
} Giocatore;

Giocatore *nuovaPartita(int *nGiocatori, Mazzo *m, Tabellone t[], FILE *log);

int leggiGiocatori(int min, int max);

Giocatore *caricaGiocatori(int nGiocatori, FILE *f);

void leggiNome(char c[]);

void preparazione(Giocatore *g, int nGiocatori, Mazzo *m, Tabellone t[], FILE *f);

void ordinaVettore(Giocatore *g, int nGiocatori, FILE *f);

void sceltaColore(Giocatore *g, int nGiocatori, FILE *f);

void assegnaArmate(Giocatore *g, int nGiocatori, FILE *f);

void distribuisciCarte(int nGioc, Mazzo *m, Giocatore *g);

void daiCarte(Giocatore g[], Mazzo *m, int nGioc, int nCarte);

void ass(Mazzo *m, int nCarte);

void assegnaArmateTerritori(int nGiocatori, Giocatore g[], Tabellone t[]);

void posizionaArmate(Giocatore *g, Tabellone t[], int scelta);

#endif //RISIKA_LIBPREP_H
