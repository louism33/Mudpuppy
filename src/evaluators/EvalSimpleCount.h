//
// Created by louis on 6/7/19.
//

#ifndef OTHELLOPROJECT_EVALSIMPLECOUNT_H
#define OTHELLOPROJECT_EVALSIMPLECOUNT_H

#include <bitset>
#include "../main.h"
#include <sstream>
#include <iostream>
#include "../Art.h"
#include "../Board.h"
#include "EvalBase.h"

class EvalSimpleCount: public EvalBase {
    int eval(const Board &board, unsigned long moves);
};



#endif //OTHELLOPROJECT_EVALSIMPLECOUNT_H
