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

unsigned long getMoveForPiece(unsigned long piece, unsigned long friends, unsigned long enemies) {
    return 0;
}

std::vector<long> getMovesAsArray(unsigned long moves){
    std::vector<long> result;

    while (moves){
        result.push_back(moves & -moves);
        moves &= moves - 1;
    }

    return result;
}

std::vector<std::string> getMovesAsArrayString(unsigned long moves){
    std::vector<std::string> result = {};

    while (moves){
        result.push_back(getMoveStringFromMove(moves & -moves));
        moves &= moves - 1;
    }

    return result;
}