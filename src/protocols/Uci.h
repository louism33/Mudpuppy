//
// Created by louis on 6/16/19.
//

#ifndef MUDPUPPY_UCI_H
#define MUDPUPPY_UCI_H

#include "../Board.h"
#include "../searchUtils/TranspositionTable.h"

/*
 * a copy of the uci chess protocol
 */
void sendInfoString(Board *board, int depth, int score, long nps, long time, long pv);

std::string getPV(Board *board, TranspositionTable *tt);

void sendBestMove(long bestMove);


#endif //MUDPUPPY_UCI_H
