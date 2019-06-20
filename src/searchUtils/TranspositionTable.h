//
// Created by louis on 6/16/19.
//

#ifndef MUDPUPPY_TRANSPOSITIONTABLE_H
#define MUDPUPPY_TRANSPOSITIONTABLE_H

#include "../Board.h"

enum Flag {
    EXACT = 0, LOWERBOUND = 1, UPPERBOUND = 2
};

struct Entry {
    unsigned long bestMove; // todo consider index
    int score;
    int age;
    Flag flag;
    int depth;
};

unsigned int getIndex(Board *board, unsigned long hash = 0);

unsigned long getBoardHash(const Board *board);

void addToTableReplaceByDepth(Board *board, unsigned long bestMove, int score, Flag flag, int depth, int age);

Entry *retrieveFromTable(Board *board);

unsigned long hashPlayer(unsigned long x, bool whiteTurn);

void resetTT();

#endif //MUDPUPPY_TRANSPOSITIONTABLE_H
