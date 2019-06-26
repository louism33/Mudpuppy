//
// Created by louis on 6/10/19.
//
#include <bitset>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <math.h>
#include <jmorecfg.h>
#include "Art.h"
#include "main.h"
#include "Board.h"
#include "BitBoardUtils.h"
#include "Move.h"

uint64_t getMoveForPiece(uint64_t piece, uint64_t friends, uint64_t enemies) {
    return 0;
}

void getMovesAsArray(uint64_t *array, uint64_t moves) {
    // store number of moves in array[0]
    int index = 1;
    int c = 1;
    if (moves == 0) {
        array[1] = PASS_MOVE;
        index++;
    }else {
        c = popCount(moves);
        while (moves) {
            array[index++] = (moves & -moves);
            moves &= moves - 1;
        }
    }
    array[0] = index;
    assert(c +1== index);
}

std::vector<std::string> getMovesAsArrayString(uint64_t moves) {
    std::vector<std::string> result = {};

    while (moves) {
        result.push_back(getMoveStringFromMove(moves & -moves));
        moves &= moves - 1;
    }

    return result;
}