#include <stdio.h>
#include "libRisika.h"

int main() {
    int i=0;
    Giocatore *g;
    caricaGiocatori(4,g);
    ordinaVettore(g,4);
    return 0;
}