#include "docente.h"

//This global constant variable contains the adjacencies of the territories (true if a territory is adjacent with another).
const _Bool t[N_TERRITORI][N_TERRITORI] = {
        {false, false, true,  false, false, false, false, false, false, false, false, false, true,  false, false, false, false, true,  false, false, false, false, false, false, false, false},
        {false, false, true,  true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
        {true,  true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
        {false, true,  false, false, true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,  false, false, false, false, false, false},
        {false, false, false, true,  false, true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, true,  false, false, true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, true,  true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, true,  true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, true,  false, false, false, true,  false, false, false, false, false, true,  false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, true,  false, false, false, false, false, false, false, false, false, true,  false, false, false, false, false},
        {false, false, false, false, false, false, false, false, true,  true,  false, true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
        {true,  false, false, false, false, false, false, false, false, false, false, false, false, true,  true,  false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false, false, true,  false, true,  false, true,  false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false, false, true,  true,  false, true,  false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,  false, false, true,  false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, true,  false, false, false, false, true,  false, false, false, false, false, false, false, false, false, false, false, false},
        {true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,  false, false, true,  false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,  false, true,  false, true,  false, false, false, false},
        {false, false, false, true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,  false, true,  true,  true,  false, false, false},
        {false, false, false, false, false, false, false, false, false, true,  false, false, false, false, false, false, false, false, false, true,  false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,  true,  false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,  false, false, false, true,  false, true},
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,  false, true,  false},
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,  false, true},
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,  false, true,  false}
};

/**
 * This function checks if two territories are adjacent (reachable from each other).
 * @param idFirst  the id of the first territory
 * @param idSecond the id of the second territory
 * @return it returns true if they are adjacent, false otherwise
 */
_Bool isAdjacent(int idFirst, int idSecond) {
    return t[idFirst][idSecond];
}

