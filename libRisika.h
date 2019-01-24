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
typedef struct carta {
    struct carta *prev;
    struct carta *next;
    Arma a;
    Territorio t;
}Carta;

//strutura per il mazzo di carte
typedef struct {
    Carta *testa;
    Carta *coda;
}Mazzo;


#endif //RISIKA_LIBRISIKA_H
