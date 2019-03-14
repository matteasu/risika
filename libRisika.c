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
    //dichiarazione variabili essenziali per l'avvio corretto del gioco
    Giocatore *g = NULL;
    FILE *saveFile;
    saveFile = fopen("Salvataggio.rsk", "rb");
    FILE *log;
    log = fopen(F_LOG, "a+");
    char scelta;
    Mazzo m;
    Tabellone t[N_TERRITORI];
    m.testa = NULL;
    importaTerritori(t);
    importaCarte(&m);
    Salvataggio s;
    int nGiocatori, currentP;
    int idP = -1;
    int i = 0;
    fprintf(log, "%s", "Inizio nuova partita\n");


    printf("Benvenuto in Risika\n n)Per iniziare una nuova partita\n c)Per caricare un salvataggio\n");
    scanf("%c", &scelta);
    if (scelta == 'c' || scelta == 'C') {
        if (saveFile != NULL) {
            int *carteDatrovare = NULL;
            int idC;
            NodoC *app;
            s = importaSalvataggio(saveFile, &m, t, &nGiocatori, &currentP, carteDatrovare);
            g = (Giocatore *) malloc(sizeof(Giocatore) * (nGiocatori));
            for (i = 0; i < nGiocatori; i++) {
                g[i] = s.g[i];
                contaArmateG(t, &g[i]);
                g[i].c = assegnaColore(g[i].c.id);

            }
            app = m.testa;
            printf("Mazzo carte main");
            while (app != NULL) {
                printf("%d ", app->c.idCarta);
                app = app->next;
            }
            stampaGiocatori(g, nGiocatori, t);


        } else {
            printf("Errore apertura salvataggio\n La partita iniziera' da capo\n");
            g = nuovaPartita(&nGiocatori, &m, t, log);
        }
    } else {
        g = nuovaPartita(&nGiocatori, &m, t, log);
    }
    NodoC *app;
    app = m.testa;

    fclose(log);
        while (fineGioco(nGiocatori) != true) {
            printf("Vuoi salvare?\n Premi s per continuare\n");
            getchar();
            scanf("%c", &scelta);
            if (scelta == 's' || scelta == 'S') {
                saveFile = fopen("asu.rsk", "wb");
                int nC = 0;
                app = m.testa;
                while (app != NULL) {
                    nC++;
                    app = app->next;
                }
                creaSalvataggio(saveFile, nGiocatori, 0, g, nC, m, t);
                fwrite(&s, sizeof(Salvataggio), 1, saveFile);
                fclose(saveFile);
            }
            for (i = 0; i < nGiocatori; i++) {
                //pulisciConsole();
                log = fopen(F_LOG, "a");
                fprintf(log, "%s", "Gioca il giocatore \n Fase di rinforzo\n");
                rinforzo(&g[i], t);
                attacco(&g[i], g, t, &idP);
                if (idP != -1) {
                    g = rimuoviGiocatore(g, idP, nGiocatori, t);
                    nGiocatori--;
                }
                spostamentoStrategio(&g[i], t);
                fclose(log);
                pulisciConsole();
                stampaGiocatori(g, nGiocatori, t);
            }
        }
    stampaGiocatori(g, nGiocatori, t);

}


Giocatore *nuovaPartita(int *nGiocatori, Mazzo *m, Tabellone t[], FILE *log) {
    Giocatore *giocatori;
    *nGiocatori = leggiGiocatori(MIN_G, MAX_G);
    giocatori = (Giocatore *) malloc(sizeof(Giocatore) * (*nGiocatori));
    pulisciConsole();
    giocatori = caricaGiocatori(*nGiocatori, log);
    preparazione(giocatori, *nGiocatori, m, t, log);
    return giocatori;
}

_Bool fineGioco(int nGiocatori) {
    _Bool x = false;

    if (nGiocatori == 1)
        x = true;
    return x;
}

/**
 * Procedura per la fase di preparazione dei giocatori
 * @param g vettore che conterra' i giocatori
 * @param nGiocatori numero di giocatori
 */
void preparazione(Giocatore *g, int nGiocatori, Mazzo *m, Tabellone t[], FILE *f) {
    ordinaVettore(g, nGiocatori, f);
    sceltaColore(g, nGiocatori, f);
    assegnaArmate(g, nGiocatori, f);
    distribuisciCarte(nGiocatori, m, g);
    assegnaArmateTerritori(nGiocatori, g, t);
    //mischio il mazzo di carte principale
    ass(m, N_CARTE);
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
    } while ((x < min || x > max));
    return x;
}


void creaSalvataggio(FILE *f, int nGiocatori, int currentP, Giocatore *g, int nC, Mazzo m, Tabellone t[]) {

    int i = 0, j;
    NodoC *app;
    fwrite(&nGiocatori, sizeof(int), 1, f);
    fwrite(&currentP, sizeof(int), 1, f);
    for (i = 0; i < nGiocatori; i++) {
        fwrite(&g[i].nome, sizeof(char) * 24, 1, f);
        fwrite(&g[i].id, sizeof(int), 1, f);
        fwrite(&g[i].nCarte, sizeof(int), 1, f);
        if (g[i].nCarte > 0) {
            app = g[i].ca.testa;
            for (j = 0; j < g[i].nCarte; j++) {
                fwrite(&app->c.idCarta, sizeof(int), 1, f);
                app = app->next;
            }
        }
    }
    for (i = 0; i < N_TERRITORI; i++) {
        fwrite(&i, sizeof(int), 1, f);
        fwrite(&t[i].idPropietario, sizeof(int), 1, f);
        fwrite(&t[i].nArmate, sizeof(int), 1, f);
    }
    fwrite(&nC, sizeof(int), 1, f);
    app = m.testa;
    for (i = 0; i < nC; i++) {
        fwrite(&app->c.idCarta, sizeof(int), 1, f);
        app = app->next;
    }

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
Giocatore *caricaGiocatori(int nGiocatori, FILE *f) {
    Giocatore *g = NULL;
    g = (Giocatore *) malloc(sizeof(Giocatore) * nGiocatori);
    if (g == NULL) {
        exit(-1);
    } else {
        int i;
        for (i = 0; i < nGiocatori; i++) {
            leggiNome(g[i].nome);
            pulisciConsole();
            g[i].nArmateinG = 0;
            g[i].id = i;
            g[i].t.testa = NULL;
            g[i].ca.testa = NULL;
            fprintf(f, "%s %s \n", "Ho inserito un giocatore, si chiama: ", g[i].nome);
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
void ordinaVettore(Giocatore *g, int nGiocatori, FILE *f) {
    fprintf(f, "%s\n", "Ordino casualmente il vettore dei giocatori");
    int primoGiocatore = generaCasuale(0, nGiocatori - 1);
    fprintf(f, "%s %s\n", g[primoGiocatore].nome, "e' il primo giocatore");
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
void sceltaColore(Giocatore *g, int nGiocatori, FILE *f) {
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
        fprintf(f, "%s %s %s\n", g[i].nome, " ha scelto il colore: ", colori[col].nome);
        col = -1;
        pulisciConsole();
    }
}

void stampaGiocatori(Giocatore *g, int nGiocatori, Tabellone t[]) {
    int i, j;
    for (i = 0; i < nGiocatori; i++) {
        printf("Giocatore: %d\n Nome: %s\n Colore: %s\n Numero Armate: %d \n \n", g[i].id, g[i].nome, g[i].c.nome,
               g[i].nArmateinG);

        for (j = 0; j < N_TERRITORI; j++) {
            if (t[j].idPropietario == g[i].id)
                printf("t %d -nA %d \n", t[j].t.id, t[t[j].t.id].nArmate);
        }
    }

}

void assegnaArmate(Giocatore *g, int nGiocatori, FILE *f) {
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
            break;
        case 6:
            num = 20;
            break;
        default:
            break;
    }
    for (i = 0; i < nGiocatori; i++) {
        fprintf(f, "%s %s %d %s \n", g[i].nome, " Ha ricevuto ", num, " armate");
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
    int i = 0, j = 0;
    _Bool trovato;
    if (f == NULL) {
        printf("File carte.txt non trovato\n scaricalo da http://bit.ly/carteRisika \n e inseriscilo nella cartella: "
               "cmake-build-debug\n");
        exit(-1);
    } else {
        while (fscanf(f, "%d", &a) != EOF) {
            fscanf(f, "%d", &t);
            c.idCarta = i;
            c.a = a;
            c.idTerritorio = t;
            if (m->testa == NULL) {
                //inserimento in testa
                m->testa = inserimentoInTesta(c);
            } else {
                it = m->testa;
                inserimentoInCoda(it, c);
            }
            i++;
        }
        fclose(f);
    }
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
    NodoC *nuovo, *it, *prev, *nex;
    prev = NULL;
    if (testa == NULL)
        testa = inserimentoInTesta(c);
    else {
        it = testa;
        while (it != NULL) {
            prev = it;
            it = it->next;
        }
        it = prev;
        while (it->next != NULL) {
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
    NodoC *it, *iS;
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
    while (j < carteDaDare) {
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
    _Bool ok;
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
        while (it != NULL) {
            t[it->c.idTerritorio].idPropietario = g[i].id;
            t[it->c.idTerritorio].nArmate = 1;
            g[i].nArmate--;
            g[i].nArmateinG++;
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
                            printf("%s 6)posiziona la tua ultima armata\n", g[j].nome);
                        } while (scelta != AINT);
                    }
                }
                pulisciConsole();
                posizionaArmate(&g[j], t, scelta);
                pulisciConsole();
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
    int i = 0, j;
    int idTer, nT = 0;
    _Bool ok;
    while (i < nRip) {
        do {
            ok = false;
            if (nA == 1)
                printf("In che territorio vuoi mettere questa armata ?\n");
            else
                printf("In che territorio vuoi mettere queste %d armate ?\n", nA);

            for (j = 0; j < N_TERRITORI; j++) {
                if (t[j].idPropietario == g->id) {
                    stampaNomeIdTerritorio(j, t);
                    printf(" %d\n", t[j].nArmate);
                }
                nT++;
            }

            scanf("%d", &idTer);
            //controllo del valore appena letto, sia mai che un giocatore cerchi di aumentare armate di territori
            //che non gli appartengono

            if (t[idTer].idPropietario == g->id)
                ok = true;
        } while (ok != true);
            t[idTer].nArmate += nA;
            g->nArmateinG += nA;
        if (g->nArmateinG > 100)
            g->nArmateinG = 100;
            g->nArmate -= nA;
            i++;

    }
    pulisciConsole();
}

/**
 * Procedura che rappresenta la fase di rinforzo
 * @param g giocatore attuale
 * @param t tabellone di gioco
 */
void rinforzo(Giocatore *g,Tabellone t[]){
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

    nTerritori = contaTerritoriGiocatore(t, g->id);
    nTerritori=nTerritori/3;
    g->nArmate = nTerritori;
    for (i = 0; i < NUM_FACOLTA; i++) {
        numF = 0;
        for (j = 0; j < N_TERRITORI; j++) {
            if (t[j].idPropietario == g->id) {
                if (t[j].t.f == f[i]) {
                    numF++;
                }
            }
        }
        if (numF == numeroF[i].nf)
            g->nArmate += numeroF[i].incr;
    }
    app = g->ca.testa;
    while (app != NULL) {
        nCarte++;
        app = app->next;
    }
    contaArmateG(t, g);
    if (g->nArmateinG < 100) {
        if (nCarte >= 3) {
            printf("%s Vuoi giocare un bonus di carte? s per si,altrimenti premi un altro tasto\n", g->nome);
            getchar();
            scanf("%c", &scelta);
            if (scelta == 's' || scelta == 'S')
                bonusCarte(g, t);
        }
        printf("Devi utilizzare queste %d armate\n"
               "Come vuoi giocarle?\n", g->nArmate);
        do {
            if (g->nArmate == 1) {
                armateInT(g, t, 1, 1);
                g->nArmate -= 1;
            } else {
                printf("1) Tutte in un territorio\n 2)Voglio dividerle in piu' territori\n");
                scanf("%d", &sceltaIn);
                if (sceltaIn == 1) {
                    armateInT(g, t, 1, g->nArmate);
                    g->nArmate = 0;
                } else {
                    do {
                        printf("Scegli da 1 a %d armate da mettere in un territorio\n", g->nArmate);
                        scanf("%d", &tI);
                        armateInT(g, t, 1, tI);
                    } while (tI < 1 || tI > g->nArmate);
                }
            }

        } while (sceltaIn != 1 && sceltaIn != 2 && g->nArmate > 0);
    } else {
        printf("Hai raggiunto il numero massimo di armate nel tabellone\n");
    }
}

void attacco(Giocatore *g, Giocatore giocatori[], Tabellone t[], int *idP) {
    char scelta, continuo;
    int tB, nA, tA = -1, difesa;
    _Bool ok = false;

    printf("Vuoi attaccare? Premi s\n",
           g->nome,
           g->nArmate);

    //pulizia buffer in modo da poter leggere la scelta dell'utente
    getchar();
    scanf("%c", &scelta);
    if (scelta == 'S' || scelta == 's') {

        do {
            //ok=false;
            do {
                baseAttacco(g, t, &tB);
                if (tB != -1) {

                    nA = richiestaNumeroArmate(*g, 1);
                    if (t[tB].nArmate - nA == 0) {
                        printf("Non puoi lasciare un territorio scoperto!\n");
                        ok = false;
                    } else {
                        if (sceltaTerritorioAttacco(*g, t, tB, &tA)) {
                            do {
                                difesa = richiestaNumeroArmate(giocatori[t[tA].idPropietario], 2);
                                if (t[tA].nArmate - difesa < 0) {
                                    printf("Non puoi lasciare un territorio scoperto!\n");
                                    ok = false;
                                } else
                                    ok = true;
                            } while (ok != true);
                            pulisciConsole();
                            attacca(g, &giocatori[t[tA].idPropietario], t, tA, tB, nA, difesa, idP);
                            if (g->t.testa != NULL) {
                                printf("Vuoi continuare l'attacco?\n premi qualsiasi tasto per continuare\n"
                                       "altrimenti premi f per terminare la fase d'attacco\n");
                                getchar();
                                scanf("%c", &continuo);
                            } else
                                continuo = 'f';
                        }
                    }

                } else {
                    printf("Tutti i tuoi territori hanno una sola armata e non puoi attaccare\n");
                    armateInT(g, t, 1, 1);
                    ok = true;
                    continuo = 'f';
                }
            } while (ok != true);

            //ok=false;

        } while (continuo != 'f' && continuo != 'F'); //caso 0 l'utente non vuole più attaccare


    }
}

_Bool baseAttacco(Giocatore *g, Tabellone t[], int *tB) {
    int c = 0;
    _Bool ok = false;
    int i, j, a;
    printf("Da che territorio vuoi far partire l'attacco?\n");
    for (i = 0; i < N_TERRITORI; i++) {
        if (t[i].idPropietario == g->id) {
            if (t[i].nArmate > 1) {
                //controllo se ha territori attacabili da questo territorio
                a = 0;
                for (j = 0; j < N_TERRITORI; j++) {
                    if (isAdjacent(t[i].t.id, t[j].t.id) && t[j].idPropietario != g->id)
                        a++;
                }
                c = 0;
                if (a > 0) {
                    c++;
                    stampaNomeIdTerritorio(t[i].t.id, t);
                    printf(" %d\n", t[i].nArmate);
                }
            }
        }
    }
    if (c > 0) {
        scanf("%d", tB);
        if (t[*tB].idPropietario == g->id)
            ok = true;
    } else
        *tB = -1;
    return ok;
}

void pulisciConsole() {
#ifdef _WIN32
    system("cls");
#endif
#ifdef unix
    system("clear");
#endif
}

_Bool sceltaTerritorioAttacco(Giocatore g, Tabellone t[], int tB, int *tA) {
    int i = 0, app = 0;

    _Bool ok;

    printf("%s, Scegli il territorio da attaccare\n"
           "Se vedi un elenco vuoto premi -1 per cambiare base d'attacco\n", g.nome);
    while (i < N_TERRITORI) {
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

void attacca(Giocatore *g1, Giocatore *g2, Tabellone t[], int tA, int tB, int nA, int nAD, int *idP) {
    int i, j, tg, td;
    int dA[3] = {0, 0, 0};
    int dD[3] = {0, 0, 0};
    _Bool haiPerso = false;
    Carta app;
    for (i = 0; i < nA; i++) {
        dA[i] = generaCasuale(1, 6);
        printf(" attacco %d ", dA[i]);
    }
    for (j = 0; j < nAD; j++) {
        dD[j] = generaCasuale(1, 6);
        printf(" difesa %d ", dD[j]);
    }
    i = j = 0;
    while (i < nA && j < nAD && haiPerso != true) {
        tg = trovaMax(dA, nA);
        td = trovaMax(dD, nAD);
        printf("max giocatore %d  max difesa %d\n", tg, td);
        if (tg == td) {
            printf("%d %d \n Dado %d vince la difesa\n", tg, td, i);
            t[tB].nArmate--;
            g1->nArmateinG--;
            //g1->nArmate--;
        } else {
            if (tg > td) {
                printf("%d %d \n Dado %d vince l'attaccante\n", tg, td, i);
                t[tA].nArmate--;
                g2->nArmateinG--;
                //g2->nArmate--;
                if (t[tA].nArmate == 0) {
                    printf("%s hai perso il territorio %s\n", g2->nome, t[tA].t.nome);
                    t[tA].idPropietario = g1->id;
                    t[tA].nArmate = nA;
                    //rimuovi elemento
                    //app = recuperaCarta(&g2->t, tA);
                    if (contaTerritoriGiocatore(t, g2->id) == 0)
                        *idP = g2->id;

                    //aggiungi in coda
                    //inserimentoInCoda(g1->t.testa, app);
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

int trovaMax(int v[], int n) {
    int m, i, posM = 0;
    m = v[0];
    for (i = 1; i < n; i++) {
        if (m < v[i]) {
            m = v[i];
            posM = i;
        }
    }
    v[posM] = 0;
    return m;
}


Carta recuperaCarta(Mazzo *m, int el) {
    Carta c;
    NodoC *app, *prev;
    app = m->testa;
    //Cerco la carta da eliminare
    if (app->next == NULL) {
        c = app->c;
        m->testa = NULL;
    } else {
        while (app->c.idCarta != el) {
            app = app->next;
        }
        c = app->c;
        //caso in cui l'elemento da eliminare sia la testa
        if (app->prev == NULL) {
            if (app->next != NULL) {
                m->testa = app->next;
            } else {
                printf("Hai perso!\n");
                m->testa = NULL;
            }
            m->testa->prev = NULL;
            free(app);
        } else {
            //caso in cui l'elemento da eliminare sia l'ultimo elemento della lista
            if (app->next == NULL) {
                prev = app->prev;
                prev->next = NULL;
                free(app);
            } else {
                //caso in cui l'elemento è in mezzo alla lista
                prev = app->prev;
                prev->next = app->next;
                prev->next->prev = prev;
                free(app);
            }
        }
    }
    //free(app);

    //printf("sa sa sa %d %d \n ari sa sa %d %d \n", app->c.idTerritorio, app->c.a, prev->c.idTerritorio, prev->c.a);
    return c;
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

_Bool eliminaGiocatore(Giocatore *g, int id, int nGioc, Giocatore *giocatori, Tabellone t[]) {
    NodoC *app;

    //printf("%s hai perso! \n", g[id].nome);
    _Bool ok = false;
    if (app == NULL) {
        printf("%s hai perso! \n", g[id].nome);
        giocatori = rimuoviGiocatore(giocatori, id, nGioc, t);
    }

    return ok;

}

int contaTerritoriGiocatore(Tabellone t[], int id) {
    int i, c = 0;
    for (i = 0; i < N_TERRITORI; i++) {
        if (t[i].idPropietario == id)
            c++;
    }
    return c;
}

void contaArmateG(Tabellone t[], Giocatore *g) {
    int i, c = 0;
    for (i = 0; i < N_TERRITORI; i++) {
        if (t[i].idPropietario == g->id) {
            c += t[i].nArmate;
        }
    }
    g->nArmateinG = c;
}


void bonusCarte(Giocatore *g, Tabellone t[]) {
    NodoC *app;
    int vino, caffe, birra, jolly, incremento, scelta, tI;
    vino = caffe = birra = jolly = incremento = 0;
    scelta = -1;
    app = g->ca.testa;
    while (app != NULL) {
        switch (app->c.a) {
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
        if (t[app->c.idTerritorio].idPropietario == g->id)
            incremento++;
        app = app->next;
    }
    if (vino == 3)
        incremento += 6;
    if (birra == 3)
        incremento += 4;
    if (caffe == 3)
        incremento += 2;
    if (caffe == 1 && birra == 1 && vino == 1)
        incremento += 8;
    if ((jolly == 1 && vino == 2) || (jolly == 1 && birra == 2) || (jolly == 1 && caffe == 2))
        incremento += 10;

    if (incremento > 0) {
        g->nArmate += incremento;
        printf("In seguito al tris di carte da te giocato hai diritto a %d armate\n"
               "Come vuoi giocarle?\n", incremento);
        do {
            printf("1) Tutte in un territorio\n 2)Voglio dividerle in piu' territori\n");
            scanf("%d", &scelta);
            if (scelta == 1)
                armateInT(g, t, 1, incremento);
            else {
                do {
                    printf("Scegli da 1 a %d armate da mettere in un territorio\n", incremento);
                    scanf("%d", &tI);
                    armateInT(g, t, 1, tI);
                    incremento -= tI;
                } while (tI < 1 || tI > incremento);
            }

        } while (scelta != 1 && scelta != 2 && incremento > 0);

    } else {
        printf("Purtroppo nel tuo mazzo di carte non sono presenti delle carte compatibli\n "
               "o delle carte contenenti territori da te posseduti\n");
    }
}

Giocatore *rimuoviGiocatore(Giocatore *g, int id, int nGiocatori, Tabellone t[]) {
    Giocatore app[nGiocatori - 1];
    printf("%s hai perso!\n", g[id].nome);
    printf("copio i giocatori ad ecezione del giocatore da eliminare\n");
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
    printf("Sto riallocando la memoria associata al vettore di giocatori\n");
    g = (Giocatore *) realloc(g, sizeof(Giocatore) * nGiocatori - 1);
    for (i = 0; i < nGiocatori - 1; i++) {
        printf("Sto copiando il giocatore %d %s\n", app[i].id, app[i].nome);
        g[i] = app[i];
    }
    return g;
}

void sistemaTabellone(Tabellone t[], int oldId, int newId) {
    int i;
    for (i = 0; i < N_TERRITORI; i++) {
        if (t[i].idPropietario == oldId) {
            t[i].idPropietario = newId;
        }
    }
}

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
            if (t[tD].idPropietario == g->id)
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

//typedef enum {Rosso,Nero,Viola,Verde,Giallo,Blu}Colore;
void statisticheVittoria(Giocatore *g, Statistiche stat[]) {
    switch (g->c.id) {
        case Rosso:
            stat[Rosso].npv++;
            break;
        case Nero:
            stat[Nero].npv++;
            break;
        case Viola:
            stat[Viola].npv++;
            break;
        case Verde:
            stat[Verde].npv++;
            break;
        case Giallo:
            stat[Giallo].npv++;
            break;
        case Blu:
            stat[Blu].npv++;
            break;
    }
}

Salvataggio importaSalvataggio(FILE *f, Mazzo *m, Tabellone t[], int *nGioc, int *currentP, int *cartedt) {
    Salvataggio s;
    _Bool trovato;
    NodoC *app, *it;
    Carta c;
    Carta *carte;
    int i, j, idC, idT, cF;
    int *carteDatrovare = NULL;
    fread(nGioc, sizeof(int), 1, f);
    fread(currentP, sizeof(int), 1, f);
    for (i = 0; i < *nGioc; i++) {
        s.g[i].id = i;
        fread(&s.g[i].nome, sizeof(char) * 24, 1, f);
        s.g[i].ca.testa = NULL;
        fread(&s.g[i].c.id, sizeof(int), 1, f);
        fread(&s.g[i].nCarte, sizeof(int), 1, f);
        for (j = 0; j < s.g[i].nCarte; j++) {
            fread(&idC, sizeof(int), 1, f);
            c = recuperaCarta(m, idC);
            if (s.g[i].ca.testa == NULL) {
                s.g[i].ca.testa = inserimentoInTesta(c);
            } else {
                inserimentoInCoda(s.g[i].ca.testa, c);
            }
        }
    }
    for (i = 0; i < N_TERRITORI; i++) {
        fseek(f, sizeof(int), SEEK_CUR);
        fread(&t[i].idPropietario, sizeof(int), 1, f);
        fread(&t[i].nArmate, sizeof(int), 1, f);

    }
    //provo  a ordinare le carte

    fread(&s.nCarte, sizeof(int), 1, f);
    for (i = 0; i < s.nCarte; i++) {
        fread(&s.carte[i], sizeof(int), 1, f);
    }
    app = m->testa;
    i = 0;
    while (app != NULL) {
        if (app->c.idCarta != s.carte[i]) {
            it = app->next;
            while (it->c.idCarta != s.carte[i]) {
                it = it->next;
            }
            c = it->c;
            it->c = app->c;
            app->c = c;
            i++;
            app = app->next;
        } else {
            app = app->next;
            i++;
        }
    }
    fclose(f);
    return s;
}

Colore assegnaColore(int id) {
    Colore colori[6] = {{0, "Rosso",  true},
                        {1, "Nero",   true},
                        {2, "Viola",  true},
                        {3, "Verde",  true},
                        {4, "Giallo", true},
                        {5, "Blu",    true}};
    Colore c;
    switch (id) {
        case Rosso:
            c = colori[Rosso];
            break;
        case Nero:
            c = colori[Nero];
            break;
        case Viola:
            c = colori[Viola];
            break;
        case Verde:
            c = colori[Verde];
            break;
        case Giallo:
            c = colori[Giallo];
            break;
        case Blu:
            c = colori[Blu];
            break;
    }
    return c;
}