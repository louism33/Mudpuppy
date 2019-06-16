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
#include "Uci.h"
#include "../Art.h"
#include "../main.h"
#include "../Board.h"
#include "../BitBoardUtils.h"
#include "../MoveUtils.h"
#include "../Board.h"

void sendInfoString(int depth, int score, long nps, long time, long pv) {
    std::string pvString = getMoveStringFromMove(pv);
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
