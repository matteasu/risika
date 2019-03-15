//
// Liberia contenente le funzioni e le proceudre necessarie alla fase di preparazione del gioco
//

#include "libPrep.h"
#include "libRisika.h"

/**
 * Funzione per l'inizio di una nuova partita
 * @param nGiocatori numero di giocatori
 * @param m mazzo contenente le carte
 * @param t tabellone coi territori
 * @param log file di log
 * @return indirizzo del vettore contenente i giocatori
 */
Giocatore *nuovaPartita(int *nGiocatori, Mazzo *m, Tabellone t[], FILE *log) {
    Giocatore *giocatori;
    *nGiocatori = leggiGiocatori(MIN_G, MAX_G);
    giocatori = (Giocatore *) malloc(sizeof(Giocatore) * (*nGiocatori));
    pulisciConsole();
    giocatori = caricaGiocatori(*nGiocatori, log);
    preparazione(giocatori, *nGiocatori, m, t, log);
    return giocatori;
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
            g[i].nCarte = 0;
            g[i].id = i;
            g[i].t.testa = NULL;
            g[i].cg.testa = NULL;
            fprintf(f, "%s %s \n", "Ho inserito un giocatore, si chiama: ", g[i].nome);
        }
    }

    return g;
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
 * Procedura per la fase di preparazione dei giocatori
 * @param g vettore che conterra' i giocatori
 * @param nGiocatori numero di giocatori
 */
void preparazione(Giocatore *g, int nGiocatori, Mazzo *m, Tabellone t[], FILE *f) {
    //ordinamento dei giocatori
    ordinaVettore(g, nGiocatori, f);
    //scelta del colore delle armate
    sceltaColore(g, nGiocatori, f);
    //assegnamento delle armate di base
    assegnaArmate(g, nGiocatori, f);
    //assegnamento dei territori
    distribuisciCarte(nGiocatori, m, g, f);
    //posizionamento armate base
    assegnaArmateTerritori(nGiocatori, g, t, f);
    //mischio il mazzo di carte principale
    ass(m, N_CARTE);
    fprintf(f, "Mazzo di carte mischiato");
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
    _Bool ok;
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
            //controllo che l'utente non abbia inserito un valore out of range
            if (col >= 0 && col <= 6) {
                //se il colore è gia' stato scelto non lo assegno e richiedo il colore
                if (colori[col].inUse == true)
                    ok = false;
                else
                    ok = true;
            }
        } while (ok != true);
        //segno come in uso il colore appena scelto
        colori[col].inUse = true;
        g[i].c = colori[col];
        fprintf(f, "%s %s %s\n", g[i].nome, " ha scelto il colore: ", colori[col].nome);
        col = -1;
        pulisciConsole();
    }
}

/**
 * Procedura per l'assegnamento delle armate di base
 * @param g giocatori
 * @param nGiocatori
 * @param f log
 */
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
 * Procedura per la distribuzione delle carte ai giocatori
 * @param nGioc numero giocatori
 * @param m mazzo
 * @param g giocatori
 */
void distribuisciCarte(int nGioc, Mazzo *m, Giocatore *g, FILE *log) {
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
    //ass asuniStupidSorting mischio le carte
    ass(&sJ, N_CARTESJ);
    //distribuzione delle carte
    fprintf(log, "%s\n", "Inizio la distribuzione delle carte");
    daiCarte(g, &sJ, nGioc, N_CARTESJ, log);
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
void daiCarte(Giocatore g[], Mazzo *m, int nGioc, int nCarte, FILE *log) {
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
            fprintf(log, "%s %s %d\n", g[i].nome, "Ha ricevuto il territorio: ", m->testa->c.idTerritorio);
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
        fprintf(log, "%s %s %d\n", g[i].nome, "Ha ricevuto il territorio: ", m->testa->c.idTerritorio);
        //eliminazione in testa
        rimuoviCarta(m);
        //passo al prossimo giocatore
        i++;
        //passo alla prossima carta
        j++;
    }
}


/**
 * Asuni stupid sorting, procedura per mischiare il mazzo di carte
 * @param m mazzo
 * @param nCarte numero carte
 */
void ass(Mazzo *m, int nCarte) {
    int i = 0, nIterazioni, j;
    NodoC *it = m->testa;
    NodoC *app;
    Carta c;
    _Bool ok;
    for (i = 0; i < nCarte; i++) {
        do {
            //generazione della posizione della carta da scambiare con quella attuale
            nIterazioni = generaCasuale(0, nCarte) + generaCasuale(0, 2);
            if (nIterazioni < nCarte) {
                ok = true;
            } else
                ok = false;
        } while (ok != true);
        app = m->testa;
        //ricerca della carta da prendere
        for (j = 0; j < nIterazioni; j++) {
            app = app->next;
        }
        //scambio delle carte
        c = it->c;
        it->c = app->c;
        app->c = c;
        //passo alla prossima carta
        it = it->next;
    }
}

/**
 * Procedura che assegna un armata ad ogni terrotorio posseduto dal giocatore e che in seguito chiede
 * al giocatore dove mettere le restanti armate
 * @param nGiocatori numero di giocatori
 * @param g vettore contenente i giocatori
 * @param t tabellone di gioco
 */
void assegnaArmateTerritori(int nGiocatori, Giocatore g[], Tabellone t[], FILE *log) {
    int i = 0, j, scelta = -1;
    fprintf(log, "%s\n", "Inizia l'assegnamento delle armate nei territori");
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
            fprintf(log, "%s %s\n", t[it->c.idTerritorio].t.nome, "Ha ricevuto una armata");
        }
        fprintf(log, "%s %s %d\n", "Numero armate ", g[i].nome, g[i].nArmate);
    }
    i = 0;
    fprintf(log, "%s\n", "Posizionamento armate rimanenti");
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
                //procedura che posiziona effetivamente le armate
                posizionaArmate(&g[j], t, scelta, log);
                pulisciConsole();
                j++;
            } else {
                i++;
                j++;
            }
        }
    }
    fprintf(log, "%s\n", "Fine dell'assegnamento delle armate nei territori");
}

/**
 * Procedura che chiama i vari posizioni delle armate in base alla scelta
 * @param g giocatore
 * @param t tabellone
 * @param scelta tipologia di posizionamento
 */
void posizionaArmate(Giocatore *g, Tabellone t[], int scelta, FILE *log) {

    switch (scelta) {
        case TREAINT: //tre armate in un solo territorio
            armateInT(g, t, 1, 3, log);
            break;

        case DUEAINTET: //due armate in un territorio e una in un altro
            armateInT(g, t, 1, 2, log);
            armateInT(g, t, 1, 1, log);
            break;

        case TREDIVERSO: //una armata in tre territori diversi
            armateInT(g, t, 3, 1, log);
            break;
        case DUEINT: //due armate in un territorio
            armateInT(g, t, 1, 2, log);
            break;

        case DUET: // una armata in due territori diversi
            armateInT(g, t, 2, 1, log);
            break;

        case AINT: //una armata in un singolo territorio
            armateInT(g, t, 1, 1, log);
            break;

        default:
            break;


    }
}

