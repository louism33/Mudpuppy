//
// Created by louis on 6/16/19.
//

#ifndef MUDPUPPY_UCI_H
#define MUDPUPPY_UCI_H

/*
 * a copy of the uci chess protocol
 */
void sendInfoString(int depth, int score, long nps, long time, long pv);

void sendBestMove(long bestMove);


#endif //MUDPUPPY_UCI_H
