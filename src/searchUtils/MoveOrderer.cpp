//
// Created by louis on 6/16/19.
//

#include <assert.h>
#include "MoveOrderer.h"
#include <bitset>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <math.h>
#include <jmorecfg.h>
#include "../Art.h"
#include "../main.h"
#include "../Board.h"
#include "../BitBoardUtils.h"
#include "../MoveUtils.h"
#include "../Move.h"

// consider changing to dealing with int, and storing score in high bits
void getOrderedMovesAsArray(unsigned long *array, unsigned long moves, unsigned long hashMove = 0) {
    int index = 1;
    int c = 1;

    bool noMoves = moves == 0;
    if (hashMove == PASS_MOVE) {
        if (!noMoves) {
            printLong(moves);
        }
        assert(noMoves);
    }

    if (noMoves) {
        assert(hashMove == 0 || hashMove == PASS_MOVE);
    }

    if (noMoves) {
        array[1] = PASS_MOVE;
        index++;
    } else {
        c = popCount(moves);

        if (hashMove & moves){
            array[index++] = hashMove;
            moves ^= hashMove;
        }

        unsigned long cornerMoves = moves & CORNERS; // 2
        unsigned long bigSpoonMoves = moves & BIG_SPOON_SQUARES; // 3

        unsigned long smallSpoonMoves = moves & LITTLE_SPOON_SQUARES; // 7
        moves = moves & ~(CORNERS | BIG_SPOON_SQUARES | LITTLE_SPOON_SQUARES); // 6

        while (cornerMoves) {
            array[index++] = (cornerMoves & -cornerMoves);
            cornerMoves &= cornerMoves - 1;
        }
        while (bigSpoonMoves) {
            array[index++] = (bigSpoonMoves & -bigSpoonMoves);
            bigSpoonMoves &= bigSpoonMoves - 1;
        }
        while (smallSpoonMoves) {
            array[index++] = (smallSpoonMoves & -smallSpoonMoves);
            smallSpoonMoves &= smallSpoonMoves - 1;
        }
        while (moves) {
            array[index++] = (moves & -moves);
            moves &= moves - 1;
        }
    }
    array[0] = index;
    bool b = c + 1 == index;
    if (!b) {
        printLong(hashMove);
    }
    assert(b);
}








