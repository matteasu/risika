//
// Created by Matteo on 24/01/2019.
//

#ifndef RISIKA_LIBRISIKA_H
#define RISIKA_LIBRISIKA_H
#define NUM_GIOCATORI 3
#include <stdio.h>
#include "stdbool.h"
#define NUM_TERRITORI 26
typedef struct {
    int id;
    char nome[24];
}territorio;
void inizio();
_Bool isAdjacent(int idFirst, int idSecond);
#endif //RISIKA_LIBRISIKA_H
