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
    int nGiocatori,i=0;
    Mazzo m;
    m.testa = NULL;
    Territorio territori[N_TERRITORI];
    Tabellone t[N_TERRITORI];
    nGiocatori = leggiGiocatori(MIN_G, MAX_G);
    g = preparazione(nGiocatori, &m, t);
    printf("\n \n fuori main\n \n ");
    for(i=0;i<nGiocatori;i++) {
        rinforzo(&g[i], t);
        attacco(&g[i], g, t);
    }
    /**
    printf("\n");
    for(i=0;i<N_TERRITORI;i++){
        printf(" %d %s %d  ",t[i].t.id,t[i].t.nome,t[i].idPropietario);
    }*/
    stampaGiocatori(g, nGiocatori);
}

/**
 * Procedura per la fase di preparazione dei giocatori
 * @param g vettore che conterra' i giocatori
 * @param nGiocatori numero di giocatori
 */
Giocatore *preparazione(int nGiocatori, Mazzo *m, Tabellone t[]) {
    Giocatore *g;
    g = caricaGiocatori(nGiocatori);
    ordinaVettore(g, nGiocatori);
    sceltaColore(g, nGiocatori);
    assegnaArmate(g, nGiocatori);
    importaTerritori(t);
    importaCarte(m);
    distribuisciCarte(nGiocatori, m, g);
    assegnaArmateTerritori(nGiocatori, g, t);
    //mischio il mazzo di carte principale
    ass(m, N_CARTE);
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
}

/**
 * Procedura per la scelta del colore di ogni giocatore
 * @param g vettore coi giocatori
 * @param nGiocatori numero di giocatori
 */
void sceltaColore(Giocatore *g, int nGiocatori) {
    int i, j, col;

    int nColori = 6;
    /**
     * Vettore con i possibili colori, il campo booleano serve per indicare se il colore è in uso o meno
     */
    Colore colori[6] = {{0, "Rosso",  false},
                        {1, "Nero",   false},
                        {2, "Viola",  false},
                        {3, "Verde",  false},
                        {4, "Giallo", false},
                        {5, "Blu",    false}};

    for (i = 0; i < nGiocatori; i++) {
        do {
            printf("%s scegli uno dei seguenti colori:\n", g[i].nome);
            for (j = 0; j < nColori; j++) {
                //stampo il colore solo se non è gia stato scelto da un altro giocatore
                if (colori[j].inUse == false)
                    printf("%d - %s \n", colori[j].id, colori[j].nome);
            }

            scanf("%d", &col);
        } while (col < 0 || col > 6);
        colori[col].inUse = true;
        g[i].c = colori[col];
        col = -1;
    }
}

void stampaGiocatori(Giocatore *g, int nGiocatori) {
    int i;
    NodoC *app;
    for (i = 0; i < nGiocatori; i++) {
        printf("Giocatore: %d\n Nome: %s\n Colore: %s\n Numero Armate: %d \n \n", g[i].id, g[i].nome, g[i].c.nome,
               g[i].nArmate);
        app = g[i].t.testa;
        while (app->next != NULL) {
            printf("a %d - t %d \n", app->c.a, app->c.idTerritorio);
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
void importaTerritori(Tabellone t[]) {
    //apertura del file contenente i vari territori
    FILE *f = fopen("facolta.txt", "r");
    Territorio app;
    int i = 0;
    if (f == NULL) {
        printf("File facolta.txt non trovato\n scaricalo da http://bit.ly/facolta \n e inseriscilo nella cartella: "
               "cmake-build-debug\n");
        exit(-1);
    } else {
        //File aperto, posso quindi leggerne il contenuto per caricare i territori nel tabellone
        while (fscanf(f, "%s", app.nome) != EOF) {
            app.id = i;
            fscanf(f, "%d", &app.f);
            t[i].t = app;
            i++;
        }
    }
    fclose(f);

}

void importaCarte(Mazzo *m) {
    FILE *f = fopen("carte.txt", "r");
    NodoC *it;
    int a, t;
    Carta c;
    if (f == NULL) {
        printf("File carte.txt non trovato\n scaricalo da http://bit.ly/carteRisika \n e inseriscilo nella cartella: "
               "cmake-build-debug\n");
        exit(-1);
    } else {
        while (fscanf(f, "%d", &a) != EOF) {
            fscanf(f, "%d", &t);
            c.a = a;
            c.idTerritorio = t;
            if (m->testa == NULL) {
                //inserimento in testa
                m->testa = inserimentoInTesta(c);
            } else {
                it = m->testa;
                inserimentoInCoda(it, c);
            }
        }
    }
    fclose(f);
}

NodoC *inserimentoInTesta(Carta c) {
    NodoC *nuovo;
    nuovo = nuovoNodoC();
    nuovo->next = NULL;
    nuovo->prev = NULL;
    nuovo->c = c;
    return nuovo;
}

void inserimentoInCoda(NodoC *testa, Carta c) {
    NodoC *nuovo, *it, *prev;
    if (testa == NULL)
        testa = inserimentoInTesta(c);
    else {
        it = testa;
        while (it->next != NULL) {
            prev = it;
            it = it->next;
        }
        nuovo = nuovoNodoC();
        nuovo->c = c;
        nuovo->next = NULL;
        nuovo->prev = prev;
        it->next = nuovo;
    }
}

void distribuisciCarte(int nGioc, Mazzo *m, Giocatore *g) {
    NodoC *it, *iS, *s;
    Mazzo sJ;
    Carta c;
    //it puntatore per scorrere il mazzo principale

    sJ.testa = NULL;
    it = m->testa;

    //copia delle carte escludendo i jolly
    while (it->c.a != 3) {
        //controllo se la testa e' vuota
        c = it->c;
        if (sJ.testa == NULL) {
            //creo la nuova carta e la metto in testa
            sJ.testa = inserimentoInTesta(c);
        } else {
            iS = sJ.testa;
            inserimentoInCoda(iS, c);
        }
        it = it->next;
    }
    //ass asuniStupidSorting
    ass(&sJ, N_CARTESJ);
    //distribuzione delle carte
    daiCarte(g, &sJ, nGioc, N_CARTESJ);
    free(sJ.testa);
}

/**
 * Procedura che distribuisce tutte le 26 carte (Ovvero le carte rimanenti dopo che vengono tolti i jolly)
 * ai giocatori
 * @param g Vettore contenente i giocatori
 * @param m Mazzo da cui attingere alle carte
 * @param nGioc numero di giocatori
 * @param nCarte numero di carte da distribuire
 */
void daiCarte(Giocatore g[], Mazzo *m, int nGioc, int nCarte) {
    int i, j = 0, carteDaDare, cr;
    //i e j son dei contatori
    NodoC *app;
    carteDaDare = (nCarte / nGioc) * nGioc; //Calcolo il numero di carte che ogni giocatore ricevera'
    cr = nCarte % nGioc; //carte rimanenti che rimangono da distribuire per arrivare a 26
    while (j <= carteDaDare) {
        for (i = 0; i < nGioc; i++) {
            //se il mazzo di carte del giocatore e' vuoto
            if (g[i].t.testa == NULL) {
                g[i].t.testa = inserimentoInTesta(m->testa->c);
            } else {
                //se il mazzo del giocatore non e' vuoto vado avanti sino a quando non trovo una posizione libera
                app = g[i].t.testa;
                inserimentoInCoda(app, m->testa->c);
            }
            j++;//dopo che la carta e' stata correttamente inserita nel nuovo mazzo vado avanti
            //eliminazione in testa dal mazzo principale
            rimuoviCarta(m);
        }
    }
    i = j = 0;
    //distribuzione delle carte rimanenti
    while (j < cr) {
        //nodo d'appoggio per scorrere il mazzo del giocatore
        app = g[i].t.testa;
        inserimentoInCoda(app, m->testa->c);
        //eliminazione in testa
        rimuoviCarta(m);
        //passo al prossimo giocatore
        i++;
        //passo alla prossima carta
        j++;
    }
}


void rimuoviCarta(Mazzo *m) {
    NodoC *vecchia;
    if (m->testa->next != NULL) {
        vecchia = m->testa->next;
        free(m->testa);
        m->testa = vecchia;
    }
}


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

void stampaNomeIdTerritorio(int id, Tabellone t[]) {
    printf("%d)", t[id].t.id);
    stampaNomeTerritorio(id, t);
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
    int i = 0, j, scelta = -1;

    NodoC *it;
    for (i = 0; i < nGiocatori; i++) {
        it = g[i].t.testa;
        //assegnamento armate iniziali
        while (it->next != NULL) {
            t[it->c.idTerritorio].idPropietario = g[i].id;
            t[it->c.idTerritorio].nArmate = 1;
            g[i].nArmate--;
            it = it->next;
        }
    }
    i = 0;
    while (i < nGiocatori) {
        j = i;
        while (j < nGiocatori) {
            if (g[j].nArmate > 0) {
                printf("\n \n numero armate %d \n \n", g[j].nArmate);
                if (g[j].nArmate >= 3) {
                    do {
                        printf("%s come vuoi posizionare queste 3 armate ?\n 1) per metterne 3 in un territorio\n"
                               "2) 2 in un territorio e la rimanente in un'altro territorio\n"
                               "3) tutte e 3 in territori diversi\n", g[j].nome);
                        scanf("%d", &scelta);
                    } while (scelta < TREAINT || scelta > TREDIVERSO);

                } else {
                    if (g[j].nArmate == 2) {
                        do {
                            printf("%s come vuoi posizionare queste %d armate ?\n 4) per metterne %d in un territorio\n"
                                   "5) %d in un territorio e %d in un'altro territorio\n", g[j].nome, g[j].nArmate,
                                   g[j].nArmate,
                                   g[j].nArmate - 1, g[j].nArmate - 1);
                            scanf("%d", &scelta);
                        } while (scelta < DUEINT || scelta > DUET);
                    } else {
                        do {
                            printf("%s 6)posiziona la tua ultima armata\n ");
                        } while (scelta != AINT);
                    }

                }
                posizionaArmate(&g[j], t, scelta);
                j++;
            } else {
                i++;
                j++;
            }
        }
    }
}

void posizionaArmate(Giocatore *g, Tabellone t[], int scelta) {

    switch (scelta) {
        case TREAINT:
            armateInT(g, t, 1, 3);
            break;

        case DUEAINTET:
            armateInT(g, t, 1, 2);
            armateInT(g, t, 1, 1);
            break;

        case TREDIVERSO:
            armateInT(g, t, 3, 1);
            break;

        case DUEINT:
            armateInT(g, t, 1, 2);
            break;

        case DUET:
            armateInT(g, t, 2, 1);
            break;

        case AINT:
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
    NodoC *it;
    int i = 0, j;
    int idTer, nT = 0;
    _Bool ok = false;
    while (i < nRip) {
        j = 0;
        do {
            it = g->t.testa;
            if (nA == 1)
                printf("In che territorio vuoi mettere questa armata ?\n");
            else
                printf("In che territorio vuoi mettere queste %d armate ?\n", nA);
            while (it->next != NULL) {
                stampaNomeIdTerritorio(it->c.idTerritorio, t);
                nT++;
                it = it->next;
            }
            scanf("%d", &idTer);
            it = g->t.testa;
            //controllo del valore appena letto, sia mai che un giocatore cerchi di aumentare armate di territori
            //che non gli appartengono
            while (j < nT) {
                while (it->next != NULL) {
                    if (idTer == it->c.idTerritorio)
                        ok = true;
                    it = it->next;
                }
                j++;
            }
        } while (ok != true);
        t[idTer].nArmate += nA;
        g->nArmate -= nA;
        i++;
    }
}

/**
 * Procedura che rappresenta la fase di rinforzo
 * @param g giocatore attuale
 * @param t tabellone di gioco
 */
void rinforzo(Giocatore *g,Tabellone t[]){
    NodoC *app; //Puntatore utilizzato per scorrere la lista coi territori del giocatore
    int nTerritori = 0, i, numF; //numero di territori del giocatore,contatore,variabile utilizzata per contare i territori
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

    app=g->t.testa;
    while(app->next!=NULL){
        nTerritori++;
        app=app->next;
    }
    nTerritori=nTerritori/3;
    g->nArmate=nTerritori;
    for (i = 0; i < NUM_FACOLTA; i++) {
        app = g->t.testa;
        numF = 0;
        while (app->next != NULL) {
            if (t[app->c.idTerritorio].t.f == f[i]) {
                numF++;
            }
            app = app->next;
        }
        if (numF == numeroF[i].nf)
            g->nArmate += numeroF[i].incr;
    }
}

void attacco(Giocatore *g, Giocatore giocatori[], Tabellone t[]) {
    char scelta;
    NodoC *app;
    int nT = 0, tB, j = 0, nA, tA, difesa;
    _Bool ok=false;
    printf("%s\n per attaccare premi S\n in caso contrario dovrai aggiungere una armata\n", g->nome);
    //pulizia buffer in modo da poter leggere la scelta dell'utente
    fflush(stdin);
    scanf("%c", &scelta);
    if (scelta == 'S' || scelta == 's') {
        tB = baseAttacco(g, t);
        do {
            do {
                printf("Con quante armate vuoi attaccare ?\n");
                scanf("%d", &nA);
            } while (nA < 1 || nA > 3);
            if (t[tB].nArmate - nA >= 1) {
                do {
                    printf("Scegli il territorio da attaccare \n");
                    while (j < N_TERRITORI) {
                        if (isAdjacent(tB, j) && (t[j].idPropietario != g->id)) {
                            stampaNomeIdTerritorio(j, t);
                            j++;
                        } else
                            j++;
                    }
                    scanf("%d", &tA);
                    for (j = 0; j < N_TERRITORI; j++) {
                        if (isAdjacent(tB, j)) {
                            if (tA == t[j].t.id)
                                ok = true;
                        }
                    }
                } while (ok != true);
                do {
                    printf("%s con quante armate ti vuoi difendere?\n", giocatori[t[tA].idPropietario].nome);
                    scanf("%d", &difesa);
                } while (difesa < 1 || difesa > 3);
            } else {
                printf("Non puoi lasciare un territorio scoperto\n");
                ok = false;
                tB = baseAttacco(g, t);
            }
        }while(ok!=true);
    } else
        armateInT(g, t, 1, 1);
}

int baseAttacco(Giocatore *g, Tabellone t[]) {
    int tB;
    NodoC *app;
    _Bool ok;
    do {
        app = g->t.testa;
        printf("Da che territorio vuoi far partire l'attacco?\n");
        while (app->next != NULL) {
            stampaNomeIdTerritorio(app->c.idTerritorio, t);
            app = app->next;
        }
        scanf("%d", &tB);
        app = g->t.testa;
        while (app->next != NULL) {
            if (tB == app->c.idTerritorio) {
                ok = true;
            }
            app = app->next;
        }

    } while (ok != true);
    return tB;
}