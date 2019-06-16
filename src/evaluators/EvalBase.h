//
// Created by louis on 6/8/19.
//

#ifndef OTHELLOPROJECT_EVALBASE_H
#define OTHELLOPROJECT_EVALBASE_H

#include <iostream>
#include <bitset>
#include <sstream>
#include "../main.h"
#include "../Art.h"
#include "../Board.h"

class EvalBase {
public:

    const static int CHECKMATE_SCORE = 31000;
    const static int IN_CHECKMATE_SCORE = -31000;
    const static int DRAW_SCORE = 0;
    const static int SHORT_MIN = -32000;
    const static int SHORT_MAX = 32000;

    virtual int eval(const Board &board, unsigned long moves) = 0;
};
#endif //OTHELLOPROJECT_EVALBASE_H
