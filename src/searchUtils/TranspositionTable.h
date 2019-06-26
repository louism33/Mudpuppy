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
    uint64_t bestMove; // todo consider index
    int score;
    int age;
    Flag flag;
    int depth;
};

uint64_t getBoardHash(const Board *board);

uint64_t hashPlayer(uint64_t x, bool whiteTurn);

class TranspositionTable {

public:

    static uint32_t getIndex(Board *board, uint64_t hash = 0);


    void addToTableReplaceByDepth(Board *board, uint64_t bestMove, int score, Flag flag, int depth, int age);

    Entry *retrieveFromTable(Board *board);

    static void resetTT();
};

#endif //MUDPUPPY_TRANSPOSITIONTABLE_H
