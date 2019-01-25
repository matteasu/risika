//
// Created by Matteo on 24/01/2019.
//

#ifndef RISIKA_LIBRISIKA_H
#define RISIKA_LIBRISIKA_H

//enumerazioni
typedef enum {Studi_Umanistici,Scienze_Economiche_Giuridiche_Politiche,Medicina_Chirurgia,Biologia_Farmacia,Ingegneria_Architettura,Facolta_di_Scienze}Facolta;
typedef enum {Rosso,Nero,Viola,Verde,Giallo,Blu}Colore;
typedef enum {Caffe,Birra,Vino} Arma;

// struttura giocatore
typedef struct {
    int id;
    char nome[24];
    Colore c;
    int nArmate;
}Giocatore;

// struttura territorio
typedef struct {
    int id;
    char nome [24];
    Facolta f;
}Territorio;

//struttura nodo per la lista delle carte
typedef struct {
    Arma a;
    int idTerritorio;
}Carta;

//Nodo per il mazzo di carte
typedef struct nodoc {
    struct nodoc *prev;
    struct nodoc *next;
    Carta c;
}NodoC;

//strutura per il mazzo di carte
typedef struct {
    NodoC *testa;
    NodoC *coda;
}Mazzo;

//struttura per i territori del g
typedef struct nodot{
    struct nodot *prev;
    struct nodot *next;
    Territorio t;
}NodoT;


//lista territori giocatore
typedef struct {
    NodoT *testa;
    NodoT *coda;
}TerritoriG;


#endif //RISIKA_LIBRISIKA_H
