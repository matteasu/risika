//
// Created by Matteo on 15/03/2019.
//

#include "libGioco.h"


/**
 * Procedura che rappresenta la fase di rinforzo
 * @param g giocatore attuale
 * @param t tabellone di gioco
 */
void rinforzo(Giocatore *g, Tabellone t[], Mazzo *m) {
    NodoC *app; //Puntatore utilizzato per scorrere la lista coi territori del giocatore
    char scelta;
    int tI, sceltaIn;
    int nCarte = 0;
    int nTerritori = 0, i, j, numF; //numero di territori del giocatore,contatore,variabile utilizzata per contare i territori
    //di una certa facolta'
    //struttura contenente gli identificatori di ogni facolta
    Facolta f[NUM_FACOLTA] = {Studi_Umanistici,
                              Scienze_Economiche_Giuridiche_Politiche,
                              Medicina_Chirurgia, Biologia_Farmacia,
                              Ingegneria_Architettura, Facolta_di_Scienze};
    //struttura contenente il numero di territori di ogni facolta e il rispettivo incremento nel caso si abbiano tutti i
    //territori
    Incrementi numeroF[NUM_FACOLTA] = {{NUM_SU,  3},
                                       {NUM_SE,  2},
                                       {NUM_MC,  2},
                                       {NUM_BF,  4},
                                       {NUM_ING, 4},
                                       {NUM_FS,  2}};
    //calcolo il numero di armate in base al numero di territori
    nTerritori = contaTerritoriGiocatore(t, g->id);
    nTerritori = nTerritori / 3;
    g->nArmate = nTerritori;
    //ciclo per la verifica delle armate bonus in base ai territori di una certa facolta'
    for (i = 0; i < NUM_FACOLTA; i++) {
        numF = 0;
        //controllo se il giocatore ha tutti i territori di una certa facolta
        for (j = 0; j < N_TERRITORI; j++) {
            if (t[j].idPropietario == g->id) {
                if (t[j].t.f == f[i]) {
                    numF++;
                }
            }
        }
        //se ha tutti i territori do l'incremento di armate
        if (numF == numeroF[i].nf)
            g->nArmate += numeroF[i].incr;
    }
    //conto le carte per vedere se puo' giocare un tris
    app = g->cg.testa;
    while (app != NULL) {
        nCarte++;
        app = app->next;
    }
    g->nCarte = nCarte;
    //conto le armate in gioco per vedere se può' usufrire di questo incremento
    contaArmateG(t, g);
    if (g->nArmateinG < 100) {
        if (nCarte >= 3) {
            printf("%s Vuoi giocare un bonus di carte? s per si,altrimenti premi un altro tasto\n", g->nome);
            getchar();
            scanf("%c", &scelta);
            if (scelta == 's' || scelta == 'S')
                g->nArmate += bonusCarte(g, t, m);
        }
        if (g->nArmateinG + g->nArmate > 100) {
            g->nArmate = 100 - g->nArmateinG;
        }
        if (g->nArmate > 0) {
            printf("Devi utilizzare queste %d armate\n"
                   "Come vuoi giocarle?\n", g->nArmate);
            do {
                if (g->nArmate == 1) {
                    armateInT(g, t, 1, 1);
                } else {
                    printf("1) Tutte in un territorio\n 2)Voglio dividerle in piu' territori\n");
                    scanf("%d", &sceltaIn);
                    if (sceltaIn == 1) {
                        armateInT(g, t, 1, g->nArmate);
                        g->nArmate = 0;
                    } else {
                        do {
                            if (g->nArmate == 1) {
                                armateInT(g, t, 1, 1);
                            } else {
                                do {
                                    printf("Scegli da 1 a %d armate da mettere in un territorio\n", g->nArmate);
                                    scanf("%d", &tI);

                                } while (tI < 1 || tI > g->nArmate);
                                armateInT(g, t, 1, tI);
                            }
                        } while (g->nArmate != 0);
                    }
                }

            } while (sceltaIn != 1 && sceltaIn != 2 && g->nArmate != 0);
        }
    } else {
        printf("Hai raggiunto il numero massimo di armate nel tabellone\n");
    }
}


/**
 * Funzione per giocare i tris di carte
 * @param g giocatore
 * @param t tabellone
 * @param m mazzo
 * @return
 */
int bonusCarte(Giocatore *g, Tabellone t[], Mazzo *m) {
    NodoC *app;
    int i = 0, vino, caffe, birra, jolly, incremento, cApp, j, l;
    int carte[3] = {-1, -1, -1}; //vettore con le carte giocate
    Carta carteG[g->nCarte]; //vettore d'appoggio
    Carta c;
    _Bool ok;
    vino = caffe = birra = jolly = incremento = 0;
    app = g->cg.testa;
    //copio le carte nel vettore d'appoggio
    while (app != NULL) {
        carteG[i] = app->c;
        i++;
        app = app->next;
    }
    printf("%s scegli le carte da giocare per il tris\n", g->nome);
    for (i = 0; i < 3; i++) {
        do {
            for (j = 0; j < g->nCarte; j++) {
                ok = true;
                //controllo se la carta che sto per stampare e' gia' stata scelta
                for (l = 0; l < 3; l++) {
                    if (carte[l] == carteG[j].idCarta)
                        ok = false;
                }
                if (ok)
                    printf("%d) arma:%d\n", carteG[j].idCarta, carteG[j].a);
            }
            scanf("%d", &cApp);
            ok = false;
            //controllo se la carta giocata mi appartiene
            for (j = 0; j < g->nCarte; j++) {
                if (cApp == carteG[j].idCarta) {
                    ok = true;
                }
            }
            if (ok == false)
                printf("Stai cercando di giocare una carta che non ti appartiene\n");
        } while (ok != true);
        carte[i] = cApp;
    }


    for (i = 0; i < 3; i++) {
        ok = false;
        j = 0;
        while (ok != true) {
            //recupero la carte scelte dal giocatore per verificare i bonus
            if (carteG[j].idCarta != carte[i]) {
                j++;
            } else
                ok = true;
        }
        //se la carta giocata contiene un territorio posseduto dal giocatore incremento di uno le armate
        if (t[carteG[j].idTerritorio].idPropietario == g->id)
            incremento++;
        switch (carteG[j].a) {
            case Vino:
                vino++;
                break;
            case Birra:
                birra++;
                break;
            case Caffe:
                caffe++;
                break;
            case Jolly:
                jolly++;
                break;
            default:
                break;
        }

    }
    if (vino == 3)
        incremento += 6;

    if (birra == 3) {
        incremento += 4;

    }
    if (caffe == 3) {
        incremento += 2;
    }
    if (caffe == 1 && birra == 1 && vino == 1) {
        incremento += 8;
    }
    if ((jolly == 1 && vino == 2) || (jolly == 1 && birra == 2) || (jolly == 1 && caffe == 2))
        incremento += 10;

    if (incremento > 0) {
        for (i = 0; i < 3; i++) {
            //rimetto nel mazzo le carte appena giocate
            c = recuperaCarta(&g->cg, carte[i]);
            inserimentoInCoda(m->testa, c);
        }

    } else {
        printf("Purtroppo nel tuo mazzo di carte non sono presenti delle carte compatibli\n");
    }
    g->nCarte -= 3;
    return incremento;
}


/**
 * Procedura per l'attacco
 * @param g giocatore attaccante
 * @param giocatori vettore coi giocatori
 * @param t tabellone
 * @param idP id Giocatore perdente
 * @param m mazzo di carte
 */
void attacco(Giocatore *g, Giocatore giocatori[], Tabellone t[], int *idP, Mazzo *m) {
    char scelta, continuo;
    int tB, nA, tA = -1, difesa;
    _Bool ok = false;

    printf("%s Vuoi attaccare? Premi s\n", g->nome);

    //pulizia buffer in modo da poter leggere la scelta dell'utente
    getchar();
    scanf("%c", &scelta);
    if (scelta == 'S' || scelta == 's') {

        do {
            //ok=false;
            do {
                //richiesta della base d'attacco, se la procedura restituisce -1 vuol dire che non si dispongono
                //territori da cui e' possibile attaccare
                baseAttacco(g, t, &tB);
                if (tB != -1) {
                    //richiedo il numero di armate con cui si vuole attaccare
                    nA = richiestaNumeroArmate(*g, 1);
                    if (t[tB].nArmate - nA == 0) {
                        printf("Non puoi lasciare un territorio scoperto!\n");
                        ok = false;
                    } else {
                        //scelta del territorio da attaccare
                        if (sceltaTerritorioAttacco(*g, t, tB, &tA)) {
                            do {
                                //chiedo al propietario del giocatore attaccato con quante armate si vuole difendere
                                difesa = richiestaNumeroArmate(giocatori[t[tA].idPropietario], 2);
                                if (t[tA].nArmate - difesa < 0) {
                                    printf("Non puoi lasciare un territorio scoperto!\n");
                                    ok = false;
                                } else
                                    ok = true;
                            } while (ok != true);
                            pulisciConsole();
                            //fase d'attacco
                            attacca(g, &giocatori[t[tA].idPropietario], t, tA, tB, nA, difesa, idP, m);
                            //se il giocatore ha ancora armate in gioco chiedo se vuole continuare ad attaccare
                            if (g->nArmateinG > 0) {
                                printf("Vuoi continuare l'attacco?\n premi qualsiasi tasto per continuare\n"
                                       "altrimenti premi f per terminare la fase d'attacco\n");
                                getchar();
                                scanf("%c", &continuo);
                            } else
                                continuo = 'f';
                        }
                    }

                } else {
                    printf("Non puoi attaccare da nessun territorio,sistema meglio le armate nella fase di spostamento strategico\n");
                    ok = true;
                    continuo = 'f';
                }
            } while (ok != true);
        } while (continuo != 'f' && continuo != 'F'); //caso 0 l'utente non vuole più attaccare
    }
}


/**
 * Funzione per la richiesta della base d'attacco
 * @param g Giocatore
 * @param t tabellone
 * @param tB id base
 * @return true in fase di successo
 */
_Bool baseAttacco(Giocatore *g, Tabellone t[], int *tB) {
    int c = 0;
    _Bool ok = false;
    int i, j, a;
    printf("Da che territorio vuoi far partire l'attacco?\n");
    for (i = 0; i < N_TERRITORI; i++) {
        if (t[i].idPropietario == g->id) {
            if (t[i].nArmate > 1) {
                //conto le adiacenze
                a = 0;
                for (j = 0; j < N_TERRITORI; j++) {
                    if (isAdjacent(t[i].t.id, t[j].t.id) && t[j].idPropietario != g->id)
                        a++;
                }
                //conto i territori da cui posso effetivamente far partire un attacco
                if (a > 0) {
                    c++;
                    stampaNomeIdTerritorio(t[i].t.id, t);
                    printf(" %d\n", t[i].nArmate);
                }
            }
        }
    }
    //se il giocatore ha almeno un territorio leggo il territorio da cui vuole attaccare
    if (c > 0) {
        scanf("%d", tB);
        if (t[*tB].idPropietario == g->id)
            ok = true;
    } else
        *tB = -1; //altrimenti restituisco -1
    return ok;
}

/**
 * Funzione per la scelta del territorio da attaccare
 * @param g giocatore
 * @param t tabellone
 * @param tB base d'attacco
 * @param tA terriotorio da attaccare
 * @return true in caso di successo
 */
_Bool sceltaTerritorioAttacco(Giocatore g, Tabellone t[], int tB, int *tA) {
    int i = 0, app = 0;

    _Bool ok;

    printf("%s, Scegli il territorio da attaccare\n"
           "Se vedi un elenco vuoto premi -1 per cambiare base d'attacco\n", g.nome);
    while (i < N_TERRITORI) {
        //stampo tutti i territori adiacenti alla base e che non appartengono all'attaccante
        if (isAdjacent(tB, t[i].t.id) && t[i].idPropietario != g.id) {
            stampaNomeIdTerritorio(i, t);
            printf("%d\n", t[i].nArmate);
            app++;
        }
        i++;
    }

    //verifica del valore inserito
    scanf("%d", tA);
    if (*tA == -1)
        ok = false;
    else {
        if (isAdjacent(tB, *tA) && t[*tA].idPropietario != g.id)
            ok = true;
        else
            ok = false;
    }


    return ok;
}

/**
 * Procedura per l'attacco
 * @param g1 giocatore attaccante
 * @param g2 giocatore difensore
 * @param t tabellone
 * @param tA territorio attacato
 * @param tB base dell'attacco
 * @param nA numero armate attacco
 * @param nAD numero armate difesa
 * @param idP id giocatore perdente
 * @param m mazzo
 */
void attacca(Giocatore *g1, Giocatore *g2, Tabellone t[], int tA, int tB, int nA, int nAD, int *idP, Mazzo *m) {
    int i, j, tg, td;
    int dA[3] = {0, 0, 0};
    int dD[3] = {0, 0, 0};
    //tiro i dati per il giocatore che attacca
    for (i = 0; i < nA; i++) {
        dA[i] = generaCasuale(1, 6);
        printf(" attacco %d ", dA[i]);
    }
    //tiro i dadi per il difensore
    for (j = 0; j < nAD; j++) {
        dD[j] = generaCasuale(1, 6);
        printf(" difesa %d ", dD[j]);
    }
    i = j = 0;

    while (i < nA && j < nAD) {
        //trovo il valore massimo fra i vari dadi tirati
        tg = trovaMax(dA, nA);
        td = trovaMax(dD, nAD);
        printf("max giocatore %d  max difesa %d\n", tg, td);
        if (tg == td) {
            //pareggio
            printf("%d %d \n Dado %d vince la difesa\n", tg, td, i);
            t[tB].nArmate--;
            g1->nArmateinG--;
        } else {
            if (tg > td) {
                printf("%d %d \n Dado %d vince l'attaccante\n", tg, td, i);
                t[tA].nArmate--;
                g2->nArmateinG--;
                //g2->nArmate--;
                if (t[tA].nArmate == 0) {
                    printf("%s hai perso il territorio %s\n", g2->nome, t[tA].t.nome);
                    //aggiornamento del territorio appena conquistato
                    t[tA].idPropietario = g1->id;
                    t[tA].nArmate = nA;
                    //dopo aver conquistato un territorio pesco una carta
                    if (m->testa != NULL) {
                        if (g1->cg.testa == NULL) {
                            g1->cg.testa = inserimentoInTesta(m->testa->c);
                        } else {
                            inserimentoInCoda(g1->cg.testa, m->testa->c);
                        }
                        g1->nCarte++;
                        //rimuovo la carta appena presa dal mazzo
                        rimuoviCarta(m);
                    }
                    //se il giocatore attaccato si ritrova senza territori dichiaro la sconfitta
                    if (contaTerritoriGiocatore(t, g2->id) == 0) {
                        //copio le carte del giocatore che ha appena perso
                        *idP = g2->id;
                        while (g2->cg.testa != NULL) {
                            if (g1->cg.testa == NULL) {
                                g1->cg.testa = inserimentoInTesta(g2->cg.testa->c);
                            } else
                                inserimentoInCoda(g1->cg.testa, g2->cg.testa->c);
                            g1->nCarte++;
                            rimuoviCarta(&g2->cg);
                        }
                    }
                }
            } else {
                printf("%d %d \n Dado %d vince la difesa\n", tg, td, i);
                t[tB].nArmate--;
                g1->nArmateinG--;
                //g1->nArmate--;
            }
        }
        i++;
        j++;
    }
}


int richiestaNumeroArmate(Giocatore g, int caso) {
    int nA;
    switch (caso) {
        case 1:
            do {
                printf("%s, con quante armate vuoi attaccare?\n "
                       "Ricorda che puoi schierare massimo 3 armate!\n", g.nome);
                scanf("%d", &nA);
            } while (nA < 1 || nA > 3);
            break;
        case 2:
            do {
                printf("%s, con quante armate ti vuoi difendere?\n "
                       "Ricorda che puoi schierare massimo 3 armate!\n", g.nome);
                scanf("%d", &nA);
            } while (nA < 1 || nA > 3);
            break;
        default:
            break;
    }
    return nA;
}


/**
 * Funzione che rimuove il giocatore che ha appena perso
 * @param g giocatori
 * @param id idperdente
 * @param nGiocatori numero giocatori
 * @param t tabellone
 * @return indirizzo del nuovo vettore
 */
Giocatore *rimuoviGiocatore(Giocatore *g, int id, int nGiocatori, Tabellone t[]) {
    Giocatore app[nGiocatori - 1];
    printf("%s hai perso!\n", g[id].nome);
    //copio i giocatori ad ecezione del giocatore da eliminare
    int i = 0, j = 0, idV;
    while (i < nGiocatori) {
        if (g[i].id != id) {
            app[j] = g[i];
            if (g[i].id > id) {
                idV = app[j].id;
                app[j].id -= 1;
                sistemaTabellone(t, idV, app[j].id);
            }
            j++;
        }
        i++;
    }
    //Sto riallocando la memoria associata al vettore di giocatori
    g = (Giocatore *) realloc(g, sizeof(Giocatore) * nGiocatori - 1);
    for (i = 0; i < nGiocatori - 1; i++) {
        //printf("Sto copiando il giocatore %d %s\n", app[i].id, app[i].nome);
        g[i] = app[i];
    }
    return g;
}

/**
 * Procedura che viene chiamata dopo l'eliminazione di un giocatore in caso di aggiornamento dell'id di uno dei giocatori
 * @param t tabellone
 * @param oldId vecchio id del giocatore
 * @param newId nuovo id del giocatore
 */
void sistemaTabellone(Tabellone t[], int oldId, int newId) {
    int i;
    for (i = 0; i < N_TERRITORI; i++) {
        if (t[i].idPropietario == oldId) {
            t[i].idPropietario = newId;
        }
    }
}

/**
 * Procedura per svolgere lo spostamento strategico
 * @param g Giocatore
 * @param t tabellone
 */
void spostamentoStrategio(Giocatore *g, Tabellone t[]) {
    char scelta;
    int tB, tD, i, j, nA, nT = 0;
    _Bool ok = false;
    printf("Vuoi effettuare uno spostamento strategico?\n Premi s per continuare\n");
    getchar();
    scanf("%c", &scelta);
    if (scelta == 'S' || scelta == 's') {
        do {
            printf("Da che territorio vuoi spostare delle armate?\n");
            for (j = 0; j < N_TERRITORI; j++) {
                nT = 0;
                //stampo solo i territori da cui e' possibile svolgere uno spostamento
                if (t[j].idPropietario == g->id) {
                    for (i = 0; i < N_TERRITORI; i++) {
                        if (isAdjacent(t[j].t.id, t[i].t.id) && t[i].idPropietario == g->id) {
                            nT++;
                        }
                    }
                    if (nT > 0) {
                        stampaNomeIdTerritorio(t[j].t.id, t);
                        printf(" %d\n", t[j].nArmate);
                    }
                }
            }
            scanf("%d", &tB);
            if (t[tB].idPropietario == g->id)
                ok = true;
        } while (ok != true);
        ok = false;
        do {
            printf("In che territorio vuoi aggiungere delle armate?\n");
            for (i = 0; i < N_TERRITORI; i++) {
                if (isAdjacent(tB, t[i].t.id) && t[i].idPropietario == g->id) {
                    stampaNomeIdTerritorio(i, t);
                    printf(" %d\n", t[i].nArmate);
                }
            }
            scanf("%d", &tD);
            if (t[tD].idPropietario == g->id && isAdjacent(tB, tD))
                ok = true;
        } while (ok != true);
        ok = false;
        do {
            printf("Quante armate vuoi spostare da %s a %s?\n", t[tB].t.nome, t[tD].t.nome);
            scanf("%d", &nA);
            if (t[tB].nArmate - nA < 1 || nA >= t[tB].nArmate) {
                printf("Devi lasciare sempre almeno una armata per territorio!\n");
            } else
                ok = true;
        } while (ok != true);
        t[tB].nArmate -= nA;
        t[tD].nArmate += nA;
        printf("Hai spostato %d armate da %s a %s\n", nA, t[tB].t.nome, t[tD].t.nome);
    }
}

/**
 * Funzione che determina la fine della partita
 * @param nGiocatori //numero giocatori
 * @return true se nGiocatori==1,altrimenti false
 */
_Bool fineGioco(int nGiocatori) {
    _Bool x = false;

    if (nGiocatori == 1)
        x = true;
    return x;
}

/**
 * Procedura che annuncia la fine della partita
 * @param g Vincitore
 * @param log file di log
 */
void finePartita(Giocatore *g, FILE *log) {
    printf("Ha vinto %s!", g->nome);
    fprintf(log, "%s %s %s\n", "Ha vinto", g->nome, "!");
}
