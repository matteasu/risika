//
// Created by Matteo on 15/03/2019.
//

#include "libFile.h"


/**
 * Procedura per leggere e stampare le statistiche sulle vittorie dal file
 * @param f file da cui leggere
 * @param s struttura su cui verranno memorizzate le statistiche
 */
void leggiStatistiche(FILE *f, Stat s[]) {
    int i;
    for (i = 0; i < 6; i++) {
        fread(&s[i].colore, sizeof(char) * 10, 1, f);
        fread(&s[i].nP, sizeof(int), 1, f);
        printf("Partite vinte dalle armate %s: %d\n", s[i].colore, s[i].nP);
    }
}

/**
 * Procedura per scrivere le statistiche sulle vittorie sul file
 * @param f file su cui scrivere
 * @param s struttura contenente le statistiche
 */
void scriviStatistiche(FILE *f, Stat s[]) {
    int i;
    for (i = 0; i < 6; i++) {
        fwrite(&s[i].colore, sizeof(char) * 10, 1, f);
        fwrite(&s[i].nP, sizeof(int), 1, f);
    }
}

/**
 * Procedura per l'aggiornamento delle statistiche di vittoria
 * @param g giocatore
 * @param s vettore con le statistiche
 */
void statisticheVittoria(Giocatore *g, Stat s[]) {
    switch (g->c.id) {
        case Rosso:
            s[Rosso].nP++;
            break;
        case Nero:
            s[Nero].nP++;
            break;
        case Viola:
            s[Viola].nP++;
            break;
        case Verde:
            s[Verde].nP++;
            break;
        case Giallo:
            s[Giallo].nP++;
            break;
        case Blu:
            s[Blu].nP++;
            break;
    }
}

/**
 * Funzione per l'importazione dei dati dal salvataggio
 * @param f salvataggio
 * @param m mazzo
 * @param t tabellone
 * @param nGioc numero giocatori
 * @param currentP giocatore corrente
 * @param nCarte numero carte
 * @param log log
 * @return Struttura salvataggio
 */
Salvataggio importaSalvataggio(FILE *f, Mazzo *m, Tabellone t[], int *nGioc, int *currentP, int *nCarte, FILE *log) {
    Salvataggio s;
    NodoC *app, *it;
    Carta c;
    int i, j, idC;
    //lettura del numero di giocatori
    fread(nGioc, sizeof(int), 1, f);
    fprintf(log, "%s %d\n", "Numero giocatori: ", *nGioc);
    //lettura del giocatore corrente
    fread(currentP, sizeof(int), 1, f);
    //copia dei giocatori
    for (i = 0; i < *nGioc; i++) {
        s.g[i].id = i;
        fread(&s.g[i].nome, sizeof(char) * 24, 1, f);
        s.g[i].cg.testa = NULL;
        //assegnamento colore
        fread(&s.g[i].c.id, sizeof(int), 1, f);
        s.g[i].c = assegnaColore(s.g[i].c.id);
        //copia delle carte possedute dal giocatore
        fread(&s.g[i].nCarte, sizeof(int), 1, f);
        for (j = 0; j < s.g[i].nCarte; j++) {
            fread(&idC, sizeof(int), 1, f);
            //rimozione della carta dal mazzo
            c = recuperaCarta(m, idC);
            if (s.g[i].cg.testa == NULL) {
                s.g[i].cg.testa = inserimentoInTesta(c);
            } else {
                inserimentoInCoda(s.g[i].cg.testa, c);
            }
        }
        fprintf(log, "%s %s\n%s %s\n", "Inserito giocatore: ", s.g[i].nome, "Il giocatore usa le armate: ",
                s.g[i].c.nome);
    }
    //lettura dello stato del tabellone
    for (i = 0; i < N_TERRITORI; i++) {
        //ignoro l'id del territorio
        fseek(f, sizeof(int), SEEK_CUR);
        fread(&t[i].idPropietario, sizeof(int), 1, f);
        fread(&t[i].nArmate, sizeof(int), 1, f);
    }
    //conto il numero di armate in gioco per ogni giocatore
    for (i = 0; i < *nGioc; i++) {
        contaArmateG(t, &s.g[i]);
        fprintf(log, "%s %s %d %s\n", s.g[i].nome, "ha ", s.g[i].nArmateinG, "armate nel tabellone");
    }
    //copia delle carte rimaste nel mazzo
    fread(nCarte, sizeof(int), 1, f);
    for (i = 0; i < *nCarte; i++) {
        fread(&s.carte[i], sizeof(int), 1, f);
    }
    //ordinamento del mazzo
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

/**
 * Funzione che assegna al giocatore il colore da lui scelto
 * @param id id colore
 * @return variabile di tipo colore corrispondente
 */
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

/**
 * Procedura che scrive il salvataggio
 * @param f file di salvataggio
 * @param nGiocatori numero di giocatori
 * @param currentP giocatore corrente
 * @param g vettore coi giocatori
 * @param nC numero carte
 * @param m mazzo di carte
 * @param t tabellone
 */
void creaSalvataggio(FILE *f, int nGiocatori, int currentP, Giocatore *g, int nC, Mazzo m, Tabellone t[]) {

    int i = 0, j;
    NodoC *app;
    //scrittura del numero di giocatori
    fwrite(&nGiocatori, sizeof(int), 1, f);
    //scrittura del giocatore corrente
    fwrite(&currentP, sizeof(int), 1, f);
    for (i = 0; i < nGiocatori; i++) {
        //scrittura dati giocatore
        fwrite(&g[i].nome, sizeof(char) * 24, 1, f);
        fwrite(&g[i].id, sizeof(int), 1, f);
        fwrite(&g[i].nCarte, sizeof(int), 1, f);
        //scrittura carte del singolo giocatore
        if (g[i].nCarte > 0) {
            app = g[i].cg.testa;
            for (j = 0; j < g[i].nCarte; j++) {
                fwrite(&app->c.idCarta, sizeof(int), 1, f);
                app = app->next;
            }
        }
    }
    //scrittura del tabellone, territorio,propietario e numero armate
    for (i = 0; i < N_TERRITORI; i++) {
        fwrite(&i, sizeof(int), 1, f);
        fwrite(&t[i].idPropietario, sizeof(int), 1, f);
        fwrite(&t[i].nArmate, sizeof(int), 1, f);
    }
    //scrittura delle carte presenti nel mazzo
    fwrite(&nC, sizeof(int), 1, f);
    app = m.testa;
    for (i = 0; i < nC; i++) {
        fwrite(&app->c.idCarta, sizeof(int), 1, f);
        app = app->next;
    }

}
