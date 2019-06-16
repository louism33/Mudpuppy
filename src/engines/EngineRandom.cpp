//
// Created by louis on 6/5/19.
//

#include <bitset>
#include <bits/stdc++.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include "../main.h"
#include "../Art.h"
#include "../Board.h"
#include "EngineRandom.h"


using namespace std;

std::random_device dev;
std::mt19937 rng(dev());

EngineRandom::EngineRandom(std::string name) {
    this->name = &name;
}

unsigned int EngineRandom::getBestMoveInt(Board &board) {
    return getIndexLowestBit(getBestMove(board));
}

unsigned int EngineRandom::getDisplayScoreOfMove(Board &board){
    return 0;
}

unsigned long EngineRandom::getBestMove(Board &board) {
    unsigned long moves = board.generateLegalMoves();
    int total = popCount(moves);

    std::uniform_int_distribution<std::mt19937::result_type> dist(0,total-1);

    int rand = dist(rng);

    while (rand) {
        rand --;
        moves &= moves - 1;
    }

    assert(moves);

    unsigned long i1 = popCount(moves) == 1 ? moves : moves & -moves;

    assert(i1);

    return i1;
}

