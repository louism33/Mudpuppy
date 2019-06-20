//
// Created by louis on 6/16/19.
//

#include <bitset>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <math.h>
#include <jmorecfg.h>
#include "../Art.h"
#include "../BitBoardUtils.h"
#include "../MoveUtils.h"
#include "../Move.h"
#include "../main.h"
#include "../Board.h"
#include "TranspositionTable.h"

// replace by depth, age of 1, score, bestmove, flag


unsigned long getBoardHash(const Board *board);

const unsigned int bucketSize = 4;
const unsigned int totalEntries = 1u << 12u;
const unsigned int shiftAmount = getIndexLowestBit(totalEntries >> 2u);
const unsigned int ARRAY_SIZE = (totalEntries >> 2u) + bucketSize;
unsigned long keys[ARRAY_SIZE];
Entry entries[ARRAY_SIZE];

unsigned long hashPlayer(unsigned long x, bool whiteTurn) {
    x = ((x >> 32u) ^ x) * 0x45d9f3b45d9f3b;
    x = ((x >> 32u) ^ x) * 0x45d9f3b45d9f3b;
    x = (x >> 32u) ^ x;
    return whiteTurn ? x : ~x;
}

unsigned int getIndex(Board *board, unsigned long hash) {
    unsigned long h = hash == 0 ? getBoardHash(board) : hash;
    return (h >> (64 - shiftAmount));
}

unsigned long getBoardHash(const Board *board) {
    bool w = board->turn == WHITE;
    unsigned long h = hashPlayer(board->whitePieces, w) ^hashPlayer(board->blackPieces, w);
    return w ? h : ~h;
}

void addToTableReplaceByDepth(Board *board, unsigned long bestMove, int score, Flag flag, int depth, const int age) {
    unsigned long hash = getBoardHash(board);
    unsigned int index = getIndex(board, hash);

    int lowestDepth = 1000, replaceMeIndex = -1;
    for (int i = index; i < index + bucketSize; i++) {
        unsigned long k = keys[i];
        if (k == 0) {
            replaceMeIndex = i;
            break;
        }

        Entry alreadyThere = entries[i];

        int d = alreadyThere.depth;
        if (k == hash) {
            if (d > depth) {
                return;
            }
            replaceMeIndex = i;
            break;
        }

        assert(k != hash);
        assert(k != 0);

        if (d < lowestDepth) {
            lowestDepth = d;
            replaceMeIndex = i;
        }

    }

    assert(replaceMeIndex != -1);
    keys[replaceMeIndex] = hash;
    const Entry &entry = Entry{bestMove, score, age, flag, depth};
    entries[replaceMeIndex] = entry;
}


Entry *retrieveFromTable(Board *board) {
    unsigned long hash = getBoardHash(board);
    unsigned int index = getIndex(board, hash);

    for (int i = index; i < index + bucketSize; i++) {
        unsigned long k = keys[i];

        if (k == hash) {
            return &entries[i];
        }

    }
    return nullptr;

}


void resetTT() {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        keys[i] = 0;
        entries[i] = {};
    }
}





