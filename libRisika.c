//
// Created by Matteo on 24/01/2019.
//

#include "libRisika.h"

/**
 * Prodedura generale per il funzionamento del gioco
 * qua verrano dichiarate tutte le viariabili e verrano chiamate le procedure
 * necessarie al corretto svolgimento del gioco
 */
void gioco() {
    Giocatore *g = NULL;
    int nGiocatori;
    Mazzo m;
    m.testa = NULL;
    Territorio territori[N_TERRITORI];
    Tabellone t[N_TERRITORI];
    nGiocatori = leggiGiocatori(MIN_G, MAX_G);
    g = preparazione(nGiocatori, &m, territori, t);
    printf("\n \n fuori main\n \n ");
    stampaGiocatori(g, nGiocatori);
}

/**
 * Procedura per la fase di preparazione dei giocatori
 * @param g vettore che conterra' i giocatori
 * @param nGiocatori numero di giocatori
 */
Giocatore *preparazione(int nGiocatori, Mazzo *m, Territorio ter[], Tabellone t[]) {
    Giocatore *g;
    g = caricaGiocatori(nGiocatori);
    ordinaVettore(g, nGiocatori);
    sceltaColore(g, nGiocatori);
    assegnaArmate(g, nGiocatori);
    importaTerritori(ter, t);
    importaCarte(m);
    distribuisciCarte(nGiocatori, m, g);
    assegnaArmateTerritori(nGiocatori, g, t);
    return g;
}


/**
 * Funzione che legge il numero di giocatori e si assicura che sia un valore accettabile
 * @param min
 * @param max
 * @return numeroGiocatori
 */
int leggiGiocatori(int min, int max) {
    int x;
    do {
        printf("Inserisci il numero di giocatori\n Minimo %d Massimo %d :", min, max);
        scanf("%d", &x);
    } while (x < min || x > max);
    return x;
}

/**
 * Procedura per leggere il nome del giocatore
 * @param campo della struttura su cui andro' a scrivere
 */
void leggiNome(char c[]) {

    printf("Inserisci il nome del giocatore, max 24 caratteri!!\n altrimenti il nome verra' tagliato :) \n");
    scanf("%s", c);

}


/**
 * Procedura per creare e caricare il vettore dinamico coi giocatori
 * @param nGiocatori numero di giocatori
 * @param g puntatore allla prima posizione
 */
Giocatore *caricaGiocatori(int nGiocatori) {
    Giocatore *g = NULL;
    g = (Giocatore *) malloc(sizeof(Giocatore) * nGiocatori);
    if (g == NULL) {
        exit(-1);
    } else {
        int i;
        for (i = 0; i < nGiocatori; i++) {
            leggiNome(g[i].nome);
            g[i].id = i;
            g[i].t.testa = NULL;
        }
        printf("\n");
        printf("Stampa dei giocatori ordinati \n");
        for (i = 0; i < nGiocatori; i++) {
            printf("%s \n", g[i].nome);

        }
        printf("\n");

    }
    return g;
}


/**
 * Funzione per la generazione casuale
 * @param min
 * @param max
 * @return
 */
int generaCasuale(int min, int max) {
    int r;
    r = min + rand() % (max - min + 1);
    return r;
}

/**
 * Procedura per l'ordinamento del vettore di giocatori
 * @param g vettore coi giocatori
 * @param nGiocatori numero di giocatori
 */
void ordinaVettore(Giocatore *g, int nGiocatori) {
    int primoGiocatore = generaCasuale(0, nGiocatori - 1);
    //ordinamento del vettore
    Giocatore app[nGiocatori];
    int i, j;
    //copio il vettore
    for (i = 0; i < nGiocatori; i++) {
        app[i] = g[i];
    }
    i = primoGiocatore;
    j = 0;
    //copio da primo giocatore a nGiocatori
    while (i < nGiocatori) {
        g[j] = app[i];
        g[j].id = j;
        j++;
        i++;
    }
    //j=j+1;
    i = 0;
    //giocatori precedenti al primo giocatore
    while (i < primoGiocatore) {
        g[j] = app[i];
        g[j].id = j;
        j++;
        i++;
    }
    printf("Stampa dei giocatori ordinati \n");
    for (i = 0; i < nGiocatori; i++) {
        printf("%s - %d \n", g[i].nome, g[i].id);

    }
}


void sceltaColore(Giocatore *g, int nGiocatori) {
    int i, j, col;

    int nColori = 6;
    Colore colori[6] = {{0, "Rosso",  false},
                        {1, "Nero",   false},
                        {2, "Viola",  false},
                        {3, "Verde",  false},
                        {4, "Giallo", false},
                        {5, "Blu",    false}};

    for (i = 0; i < nGiocatori; i++) {
        printf("%s scegli uno dei seguenti colori:\n", g[i].nome);
        for (j = 0; j < nColori; j++) {
            if (colori[j].inUse == false)
                printf("%d - %s \n", colori[j].id, colori[j].nome);
        }
        scanf("%d", &col);
        colori[col].inUse = true;
        g[i].c = colori[col];
    }
}

void stampaGiocatori(Giocatore *g, int nGiocatori) {
    int i;
    NodoT *app;
    for (i = 0; i < nGiocatori; i++) {
        printf("Giocatore: %d\n Nome: %s\n Colore: %s\n Numero Armate: %d \n \n", g[i].id, g[i].nome, g[i].c.nome,
               g[i].nArmate);
        app = g[i].t.testa;
        while (app->next != NULL) {
            printf("a %d - t %d \n", app->card.a, app->card.idTerritorio);
            app = app->next;
        }
    }
}

void assegnaArmate(Giocatore *g, int nGiocatori) {
    int num, i;
    switch (nGiocatori) {
        case 3:
            num = 35;
            break;
        case 4:
            num = 30;
            break;
        case 5:
            num = 25;
        case 6:
            num = 20;
            break;
        default:
            break;
    }
    for (i = 0; i < nGiocatori; i++) {
        g[i].nArmate = num;
    }
}

/**
 * procedura per l'importazione dei territori e la relativa facolta' da file di testo
 * @param t vettore dove verranno inseriti i territori
 */
void importaTerritori(Territorio ter[], Tabellone t[]) {
    FILE *f = fopen("facolta.txt", "r");
    Territorio app;
    int i = 0;
    if (f == NULL) {
        printf("caaaazzo\n");
        exit(-1);
    } else {

        while (fscanf(f, "%s", app.nome) != EOF) {
            app.id = i;
            fscanf(f, "%d", &app.f);
            ter[i] = app;
            t[i].t = app;
            i++;
        }
    }
    fclose(f);

}

void importaCarte(Mazzo *m) {
    FILE *f = fopen("carte.txt", "r");
    NodoC *it, *prev, *nuovo;
    int a, t;
    if (f == NULL) {
        printf("caaaazzo\n");
        exit(-1);
    } else {
        while (fscanf(f, "%d", &a) != EOF) {
            fscanf(f, "%d", &t);
            if (m->testa == NULL) {
                it = nuovoNodoC();
                it->c.a = a;
                it->c.idTerritorio = t;
                it->next = NULL;
                it->prev = NULL;
                m->testa = it;
            } else {
                //creo il nuovo nodo che dovra' contenere le informazioni
                nuovo = nuovoNodoC();
                nuovo->c.idTerritorio = t;
                nuovo->c.a = a;
                nuovo->next = NULL;
                //cerco l'ultimo nodo nella lista e mi salvo il precedente
                it = m->testa;
                while (it->next != NULL) {
                    prev = it;
                    it = it->next;
                }
                nuovo->prev = prev;
                it->next = nuovo;
            }


        }
    }
}

void distribuisciCarte(int nGioc, Mazzo *m, Giocatore *g) {
    NodoC *it, *iS, *s, *prev;
    Mazzo sJ;
    //it puntatore per scorrere il mazzo principale

    sJ.testa = NULL;
    it = m->testa;
    int ncarte = 0;
    //copia delle carte escludendo i jolly
    while (it->c.a != 3) {
        //controllo se la testa e' vuota
        if (sJ.testa == NULL) {
            //creo la nuova carta e la metto in testa
            s = nuovoNodoC();
            s->c.idTerritorio = it->c.idTerritorio;
            s->c.a = it->c.a;
            s->next = NULL;
            s->prev = NULL;
            sJ.testa = s;
        } else {
            iS = sJ.testa;
            while (iS->next != NULL) {
                prev = iS;
                iS = iS->next;
            }
            //creo la nuova carta
            s = nuovoNodoC();
            s->c.idTerritorio = it->c.idTerritorio;
            s->c.a = it->c.a;
            s->next = NULL;
            s->prev = prev;
            iS->next = s;
        }
        //sJ.coda = s;
        it = it->next;
        ncarte++;
    }


    s = sJ.testa;
    //stampa delle carte prima che vengano mischiate
    while (s->next != NULL) {
        printf("a %d- t %d ", s->c.a, s->c.idTerritorio);
        s = s->next;
    }

    //ass asuniStupidSorting
    ass(&sJ, ncarte);


    //stampa delle carte dopo essere state mischiate
    s = sJ.testa;
    printf("Looking for assss\n");
    while (s->next != NULL) {
        printf("a %d- t %d   ", s->c.a, s->c.idTerritorio);
        s = s->next;
    }
    //distribuzione delle carte
    daiCarte(g, &sJ, nGioc, ncarte);

}

void daiCarte(Giocatore g[], Mazzo *m, int nGioc, int nCarte) {
    int i, j = 0, carteDaDare, cr;
    NodoC *vecc;
    NodoT *app, *nuovo;
    carteDaDare = (nCarte / nGioc) * nGioc;
    cr = nCarte % nGioc;
    while (j <= carteDaDare) {
        for (i = 0; i < nGioc; i++) {
            if (g[i].t.testa == NULL) {
                nuovo = nuovoNodoT();
                nuovo->next = NULL;
                nuovo->card = m->testa->c;
                g[i].t.testa = nuovo;
                //im=im->next;
            } else {
                app = g[i].t.testa;
                while (app->next != NULL) {
                    app = app->next;
                }
                nuovo = nuovoNodoT();
                nuovo->next = NULL;
                nuovo->card = m->testa->c;
                app->next = nuovo;
            }
            j++;
            if (m->testa->next != NULL) {
                vecc = m->testa->next;
                vecc->prev = NULL;
                free(m->testa);
                m->testa = vecc;
            }
        }
    }
    i = j = 0;

    while (j < cr) {
        app = g[i].t.testa;
        while (app->next != NULL) {
            app = app->next;
        }
        nuovo = nuovoNodoT();
        nuovo->next = NULL;
        nuovo->card = m->testa->c;
        app->next = nuovo;
        if (m->testa->next != NULL) {
            vecc = m->testa->next;
            vecc->prev = NULL;
            free(m->testa);
            m->testa = vecc;
        }
        i++;
        j++;

    }
}

/**
while (m->testa->next != NULL) {
    for (i = 0; i < nGioc; i++) {
        if (g[i].t.testa == NULL) {
            nuovo = nuovoNodoT();
            nuovo->next = NULL;
            nuovo->card = m->testa->c;
            g[i].t.testa = nuovo;
            //im=im->next;
        } else {
            app = g[i].t.testa;
            while (app->next != NULL) {
                app = app->next;
            }
            nuovo = nuovoNodoT();
            nuovo->next = NULL;
            nuovo->card = m->testa->c;
            app->next = nuovo;
        }
        if (m->testa->next != NULL) {
            vecc = m->testa->next;
            vecc->prev=NULL;
            free(m->testa);
            m->testa = vecc;
        }else{
            vecc = m->testa;
            vecc->prev=NULL;
            vecc->next=NULL;
            m->testa = vecc;
        }
    }
}*/




void ass(Mazzo *m, int nCarte) {
    int i = 0, nIterazioni, j;
    NodoC *it = m->testa;
    NodoC *app;
    Carta c;
    _Bool ok = false;
    for (i = 0; i < nCarte; i++) {
        do {
            nIterazioni = generaCasuale(0, nCarte) + generaCasuale(0, 2);
            if (nIterazioni < nCarte) {
                ok = true;
            } else
                ok = false;
        } while (ok != true);
        app = m->testa;
        for (j = 0; j < nIterazioni; j++) {
            app = app->next;
        }
        c = it->c;
        it->c = app->c;
        app->c = c;
        it = it->next;
    }
}


NodoC *nuovoNodoC() {
    NodoC *nuovoNodo = (NodoC *) malloc(sizeof(NodoC)); //Allocazione

    if (nuovoNodo == NULL) //Controllo errori
        exit(-1);

    return nuovoNodo;
}

NodoT *nuovoNodoT() {
    NodoT *nuovoNodo = (NodoT *) malloc(sizeof(NodoT));
    if (nuovoNodo == NULL)
        exit(-1);
    return nuovoNodo;
}

void stampaNomeTerritorio(int id, Tabellone t[]) {

    printf("%s \n", t[id].t.nome);

}

/**
 * Procedura che assegna un armata ad ogni terrotorio posseduto dal giocatore e che in seguito chiede
 * al giocatore dove mettere le restanti armate
 * @param nGiocatori numero di giocatori
 * @param g vettore contenente i giocatori
 * @param t tabellone di gioco
 */
void assegnaArmateTerritori(int nGiocatori, Giocatore g[], Tabellone t[]) {
    int i = 0, scelta = -1;
    _Bool ok = true;
    NodoT *it;
    for (i = 0; i < nGiocatori; i++) {
        it = g[i].t.testa;
        //assegnamento armate iniziali
        while (it->next != NULL) {
            t[it->card.idTerritorio].idPropietario = g->id;
            t[it->card.idTerritorio].nArmate = 1;
            g[i].nArmate--;
            it = it->next;
        }
    }/**
    while(ok){
        i=0;
        while(i<nGiocatori) {
            if (g[i].nArmate > 0) {
                if (g[i].nArmate >= 3) {
                    do {
                        printf("%s come vuoi posizionare queste 3 armate ?\n 1) per metterne 3 in un territorio\n"
                               "2) 2 in un territorio e la rimanente in un'altro territorio\n"
                               "3) tutte e 3 in territori diversi\n", g[i].nome);
                        scanf("%d", &scelta);
                    } while (scelta < 1 || scelta > 3);

                } else {
                    if (g[i].nArmate == 2) {
                        do {
                            printf("%s come vuoi posizionare queste %d armate ?\n 4) per metterne %d in un territorio\n"
                                   "5) %d in un territorio e %d in un'altro territorio\n", g[i].nome, g[i].nArmate,
                                   g[i].nArmate,
                                   g[i].nArmate - 1, g[i].nArmate - 1);
                            scanf("%d", &scelta);
                        } while (scelta < 4 || scelta > 5);
                    } else {
                        do {
                            printf("%s 6)posiziona la tua ultima armata\n ");
                        } while (scelta != 6);
                    }

                }
                posizionaArmate(&g[i], t, scelta);
                i++;
            }else
                ok=false;
        }
    }*/
}

void posizionaArmate(Giocatore *g, Tabellone t[], int scelta) {

    switch (scelta) {
        case 1:
            armateInT(g, t, 1, 3);
            break;

        case 2:
            armateInT(g, t, 1, 2);
            armateInT(g, t, 1, 1);
            break;

        case 3:
            armateInT(g, t, 3, 1);
            break;

        case 4:
            armateInT(g, t, 1, 2);
            break;

        case 5:
            armateInT(g, t, 2, 1);
            break;

        case 6:
            armateInT(g, t, 1, 1);
            break;

        default:
            break;


    }
}

/**
 * Procedura per aumentare il numero di armate in un determinato territorio
 * @param g giocatore che vuole aumentare le armate
 * @param t vettore contenente i dati del tabellone
 * @param nRip numero di  volte in cui devo chiedere in quale territorio mettere le armate
 * @param nA numero di armate da impiegare
 */
void armateInT(Giocatore *g, Tabellone t[], int nRip, int nA) {
    NodoT *it;
    int i = 0;
    int idTer;
    while (i < nRip) {
        it = g->t.testa;
        if (nA == 1)
            printf("In che territorio vuoi mettere questa armata ?\n");
        else
            printf("In che territorio vuoi mettere queste %d armate ?\n", nA);
        while (it->next != NULL) {
            printf("%d)", it->card.idTerritorio);
            stampaNomeTerritorio(it->card.idTerritorio, t);
            it = it->next;
        }
        scanf("%d", &idTer);
        t[idTer].nArmate += nA;
        g->nArmate -= nA;
        i++;
    }
}

