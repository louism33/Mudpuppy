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

uint64_t getBoardHash(const Board *board);

const uint32_t bucketSize = 4;
const uint32_t totalEntries = 1u << 12u;
const uint32_t shiftAmount = getIndexLowestBit(totalEntries >> 2u);
const uint32_t ARRAY_SIZE = (totalEntries >> 2u) + bucketSize;
uint64_t keys[ARRAY_SIZE];
Entry entries[ARRAY_SIZE];

uint64_t hashPlayer(uint64_t x, bool whiteTurn) {
    x = ((x >> 32u) ^ x) * 0x45d9f3b45d9f3b;
    x = ((x >> 32u) ^ x) * 0x45d9f3b45d9f3b;
    x = (x >> 32u) ^ x;
    return whiteTurn ? x : ~x;
}

uint64_t getBoardHash(const Board *board) {
    bool w = board->turn == WHITE;
    uint64_t h = hashPlayer(board->whitePieces, w) ^hashPlayer(board->blackPieces, w);
    return w ? h : ~h;
}

uint32_t TranspositionTable::getIndex(Board *board, uint64_t hash) {
    uint64_t h = hash == 0 ? getBoardHash(board) : hash;
    return (h >> (64 - shiftAmount));
}

void TranspositionTable::addToTableReplaceByDepth(Board *board, uint64_t bestMove, int score, Flag flag, int depth, const int age) {
    uint64_t hash = getBoardHash(board);
    uint32_t index = getIndex(board, hash);

    int lowestDepth = 1000, replaceMeIndex = -1;
    for (int i = index; i < index + bucketSize; i++) {
        uint64_t k = keys[i];
        if (k == 0) {
            replaceMeIndex = i;
            break;
        }

        Entry alreadyThere = entries[i];

        int oldAge = alreadyThere.age;

        if (oldAge + 1 < age) {
            replaceMeIndex = i;
            break;
        }

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


Entry *TranspositionTable::retrieveFromTable(Board *board) {
    uint64_t hash = getBoardHash(board);
    uint32_t index = getIndex(board, hash);

    for (int i = index; i < index + bucketSize; i++) {
        uint64_t k = keys[i];

        if (k == hash) {
            return &entries[i];
        }

    }
    return nullptr;

}


void TranspositionTable::resetTT() {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        keys[i] = 0;
        entries[i] = {};
    }
}





