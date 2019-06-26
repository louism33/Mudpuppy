//
// Created by louis on 6/16/19.
//

#include <printf.h>
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <bitset>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <math.h>
#include <jmorecfg.h>
#include "string"
#include "../Art.h"
#include "../BitBoardUtils.h"
#include "../MoveUtils.h"
#include "../Board.h"
#include "../main.h"
#include "Uci.h"
#include "../searchUtils/TranspositionTable.h"
#include "../Move.h"
#include "../evaluators/EvalBase.h"

void sendInfoString(Board *board, int depth, int score, long nps, long time, long pv) {
    bool win = score >= EvalBase::CHECKMATE_SCORE;
    bool loss = score <= EvalBase::IN_CHECKMATE_SCORE;
    bool normal = !win && !loss;
    std::string pvString = getPV(board, nullptr);
    std::cout << "info depth " << depth
    << (win ? " win " : loss ? " loss " : " score ")
    << (normal ? std::to_string(score) : "")
    << " time " << time
    << " nps " << nps
    << " pv " << pvString
    << std::endl;
}

void sendBestMove(long bestMove) {
    std::string moveString = getMoveStringFromMove(bestMove);
    std::cout << "bestmove " << moveString << std::endl;
}

const uint32_t maxPvLength = 10;

std::string getPV(Board *board, TranspositionTable *tt) {
    uint64_t bw = board->whitePieces;
    uint64_t bb = board->blackPieces;

    std::string pv;
    int i = 0;
    for (i = 0; i < maxPvLength; i++) {
        Entry *entry = tt->retrieveFromTable(board);
        if (entry){
            uint64_t move = entry->bestMove;
            pv += getMoveStringFromMove(move) + " ";
            board->makeMoveLong(board->turn, move);
        } else {
            break;
        }
    }

    while (i){
        board->unMakeMove();
        i--;
    }

    assert(bw == board->whitePieces);
    assert(bb == board->blackPieces);
    return pv;
}