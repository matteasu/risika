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
    Giocatore *g = NULL;//vettore dinamico che conterra' i giocatori
    FILE *saveFile;//file salvataggio
    FILE *statistiche;//file statistiche
    Stat st[6] = {{"Rosso",  0},
                  {"Nero",   0},
                  {"Viola",  0},
                  {"Verde",  0},
                  {"Giallo", 0},
                  {"Blu",    0}};
    //vettore con le statistiche di vittoria per ogni colore
    statistiche = fopen("stat", "rb");//apertura file statistiche
    if (statistiche != NULL) {
        leggiStatistiche(statistiche, st);//lettura e stampa delle statistiche aggiornate
        fclose(statistiche);//chiusura del file
    }
    saveFile = fopen("Salvataggio.rsk", "rb");//apertura file salvataggio
    FILE *log;//file log
    log = fopen(F_LOG, "a+");//apertura del file log
    char scelta;//variabile dove memorizzero' la scelta del giocatore per la modalita' di gioco
    Mazzo m;//struttura di tipo lista che conterra' le carte
    Tabellone t[N_TERRITORI];//tabellone di gioco
    m.testa = NULL;//inzializzazione della testa del mazzo di carte
    importaTerritori(t);//importo i territori dal file in modo da avere i nomi e le facolta' gia' pronte
    importaCarte(&m);//importo le carte per avere il tutto gia' pronto per le fasi sucessive di gioco
    Salvataggio s;//struttura contenente alcuni campi necessari per il salvataggio
    NodoC *app;
    int nGiocatori, currentP, nCarte;
    int idP = -1;//variabile che conterra' l'id del giocatore che ha appena perso
    int i = 0;
    fprintf(log, "%s", "Inizio nuova partita\n");
    nCarte = N_CARTE;

    printf("Benvenuto in Risika\n n)Per iniziare una nuova partita\n c)Per caricare un salvataggio\n");
    scanf("%c", &scelta);
    //carico dal salvataggio
    if (scelta == 'c' || scelta == 'C') {
        if (saveFile != NULL) {
            fprintf(log, "%s", "Caricamento della partita dal file di salvataggio\n");
            printf("Caricamento della partita dal file di salvataggio\n");
            s = importaSalvataggio(saveFile, &m, t, &nGiocatori, &currentP, &nCarte, log);
            g = (Giocatore *) malloc(sizeof(Giocatore) * (nGiocatori));
            //copio i giocatori appena letti dal file nel vettore a essi dedicato
            for (i = 0; i < nGiocatori; i++) {
                g[i] = s.g[i];
            }
            stampaGiocatori(g, nGiocatori, t);


        } else {
            fprintf(log, "%s", "Errore apertura salvataggio\n La partita iniziera' da capo\n");
            printf("Errore apertura salvataggio\n La partita iniziera' da capo\n");
            g = nuovaPartita(&nGiocatori, &m, t, log);
        }
    } else {
        fprintf(log, "%s", "Inizia la fase di preparazione\n");
        g = nuovaPartita(&nGiocatori, &m, t, log);
    }
    fclose(log);
    //fase di gioco
        while (fineGioco(nGiocatori) != true) {
            printf("Vuoi salvare?\n Premi s per continuare\n");
            getchar();
            scanf("%c", &scelta);
            if (scelta == 's' || scelta == 'S') {
                //apro il file di salvataggio
                saveFile = fopen("Salvataggio.rsk", "wb");
                //conto le carte presenti nel mazzo
                nCarte = contaCarte(&m);
                creaSalvataggio(saveFile, nGiocatori, 0, g, nCarte, m, t);
                fprintf(log, "%s", "Partita salvata\n");
                printf("Partita salvata\n");
                fclose(saveFile);
            }
            for (i = 0; i < nGiocatori; i++) {
                log = fopen(F_LOG, "a");
                fprintf(log, "%s %s %s", "Gioca il giocatore", g[i].nome, "\n Fase di rinforzo\n");
                rinforzo(&g[i], t, &m);
                attacco(&g[i], g, t, &idP, &m);
                //nel caso un giocatore abbia perso provedo a rimuoverlo
                if (idP != -1) {
                    g = rimuoviGiocatore(g, idP, nGiocatori, t);
                    nGiocatori--;
                    //riassegno  la sentinella a -1
                    idP = -1;
                }
                if (nGiocatori > 1) {
                    spostamentoStrategio(&g[i], t);
                    pulisciConsole();
                }
                stampaGiocatori(g, nGiocatori, t);
                fclose(log);
                pulisciConsole();

            }
        }
    //fine della fase di gioco
    log = fopen(F_LOG, "a");
    //apertura file statistiche
    statistiche = fopen("stat", "wb");
    //calcolo delle nuove statistiche di vittoria
    statisticheVittoria(&g[0], st);
    //scrittura sul file delle nuove statistiche
    scriviStatistiche(statistiche, st);
    //annuncio il vincitore
    finePartita(&g[0], log);
    fclose(log);
    fclose(statistiche);
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
 * Procedura per la stampa dei giocatori
 * @param g giocatori
 * @param nGiocatori
 * @param t  tabellone
 */
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

/**
 * Procedura per l'importazione delle carte dal file di testo
 * @param m mazzo
 */
void importaCarte(Mazzo *m) {
    FILE *f = fopen("carte.txt", "r");
    NodoC *it;
    int a, t;
    Carta c;
    int i = 0;
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
            //inserimento delle carte nel mazzo
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

/**
 * Funzione per l'inserimento di una carta in testa
 * @param c carta da inserire
 * @return indirizzo della nuova testa
 */
NodoC *inserimentoInTesta(Carta c) {
    NodoC *nuovo;
    nuovo = nuovoNodoC();
    nuovo->next = NULL;
    nuovo->prev = NULL;
    nuovo->c = c;
    return nuovo;
}

/**
 * Procedura per l'inserimento di una carta in coda
 * @param testa testa della lista
 * @param c carta da inserire
 */
void inserimentoInCoda(NodoC *testa, Carta c) {
    NodoC *nuovo, *it, *prev;
    prev = NULL;
        it = testa;
    //arrivo all'ultima posizione per salvarmi l'elemento precedente
        while (it != NULL) {
            prev = it;
            it = it->next;
        }
        it = prev;
    //cerco l'elemento con next==null per inserire in codda
        while (it->next != NULL) {
            it = it->next;
        }
    //creazione e assegnamento del nuovo nodo
        nuovo = nuovoNodoC();
        nuovo->c = c;
        nuovo->next = NULL;
        nuovo->prev = prev;
        it->next = nuovo;
}


/**
 * Procedura per rimuovere la prima carta dal mazzo
 * @param m mazzo
 */
void rimuoviCarta(Mazzo *m) {
    NodoC *vecchia;
    if (m->testa->next != NULL) {
        vecchia = m->testa->next;
        free(m->testa);
        m->testa = vecchia;
    } else {
        m->testa = NULL;
    }
}


/**
 * Funzione per la allocazione di un nuovo NodoC
 * @return indirizzo del nuovo nodo
 */
NodoC *nuovoNodoC() {
    NodoC *nuovoNodo = (NodoC *) malloc(sizeof(NodoC)); //Allocazione

    if (nuovoNodo == NULL) //Controllo errori
        exit(-1);

    return nuovoNodo;
}

/**
 * Procedura per stampare il nome e l'id di un territorio
 * @param id id territorio
 * @param t tabellone
 */
void stampaNomeIdTerritorio(int id, Tabellone t[]) {
    printf("%d)", t[id].t.id);
    stampaNomeTerritorio(id, t);
}

/**
 * Procedura per stampare il nome di un territorio
 * @param id id territorio
 * @param t tabellone
 */
void stampaNomeTerritorio(int id, Tabellone t[]) {
    printf("%s \n", t[id].t.nome);
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
            //stampa dei territori del giocatore
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
        //se il numero attuale di armate+ le nuove fa superare i 100 diminuisco il numero di armate
        if (g->nArmateinG + nA > 100) {
            nA = 100 - g->nArmateinG;
            t[idTer].nArmate += nA;
            g->nArmateinG += nA;
            g->nArmate -= nA;
        } else {
            t[idTer].nArmate += nA;
            g->nArmateinG += nA;
            g->nArmate -= nA;
        }
            i++;

    }
    pulisciConsole();
}
/**
 * Procedura che pulisce la console in base al sistema su cui viene eseguito il programma
 */
void pulisciConsole() {
#ifdef _WIN32
    system("cls");
#endif
#ifdef unix
    system("clear");
#endif
}
/**
 * Funzione che trova il valore massimo
 * @param v vettore contenente i risultati dei vari tiri di dado
 * @param n numero dei dadi tirati
 * @return valore massimo
 */
int trovaMax(int v[], int n) {
    int m, i, posM = 0;
    m = v[0];
    //cerco il valore massimo
    for (i = 1; i < n; i++) {
        if (m < v[i]) {
            m = v[i];
            posM = i;
        }
    }
    //dopo aver trovato il valore massimo do un valore ''nullo'' al tiro per ignorarlo alla prossima chiamata
    v[posM] = 0;
    return m;
}

/**
 * Procedura che restituisce la carta noto l'id
 * @param m mazzo
 * @param el id da cercare
 * @return Carta trovata
 */
Carta recuperaCarta(Mazzo *m, int el) {
    Carta c;
    NodoC *app, *prev;
    app = m->testa;
    //Cerco la carta da eliminare
    if (app->next == NULL) {
        c = app->c;
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
    return c;
}
/**
 * Funzione che conta i territori di un giocatore
 * @param t tabellone
 * @param id id giocatore
 * @return numero territori
 */
int contaTerritoriGiocatore(Tabellone t[], int id) {
    int i, c = 0;
    for (i = 0; i < N_TERRITORI; i++) {
        if (t[i].idPropietario == id)
            c++;
    }
    return c;
}

/**
 * Procedura che assegna al giocatore il numero di armate presenti nel tabellone
 * @param t tabellone
 * @param g giocatore
 */
void contaArmateG(Tabellone t[], Giocatore *g) {
    int i, c = 0;
    for (i = 0; i < N_TERRITORI; i++) {
        if (t[i].idPropietario == g->id) {
            c += t[i].nArmate;
        }
    }
    g->nArmateinG = c;
}
/**
 * Funzione per contare le carte presenti in un mazzo
 * @param m mazzo
 * @return ncarte
 */
int contaCarte(Mazzo *m) {
    NodoC *app;
    int c = 0;
    app = m->testa;
    while (app != NULL) {
        c++;
        app = app->next;
    }
    return c;
}
