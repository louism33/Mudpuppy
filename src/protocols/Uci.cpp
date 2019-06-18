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

void sendInfoString(Board *board, int depth, int score, long nps, long time, long pv) {
    std::string pvString = getPV(board);
    std::cout << "info depth " << depth
    << " score " << score
    << " time " << time
    << " nps " << nps
    << " pv " << pvString
    << std::endl;
}

void sendBestMove(long bestMove) {
    std::string moveString = getMoveStringFromMove(bestMove);
    std::cout << "bestmove " << moveString << std::endl;
}

const unsigned int maxPvLength = 10;

std::string getPV(Board* board){
    std::string pv;
    for (int i = 0; i < maxPvLength; i++) {
        Entry *entry = retrieveFromTable(board);
        if (entry){
            unsigned long move = entry->bestMove;
            pv += getMoveStringFromMove(move) + " ";
            board->makeMoveLong(board->turn, move);
        } else {
            while (i){
                board->unMakeMove();
                i--;
            }
            break;
        }

    }
    return pv;
}