//
// Created by louis on 6/8/19.
//

#ifndef OTHELLOPROJECT_EVALBASICHEURISTICS_H
#define OTHELLOPROJECT_EVALBASICHEURISTICS_H


#include "EvalBase.h"


class EvalBasicHeuristics : public EvalBase{

    int eval(const Board &board, unsigned long moves);

    int evalTurn(const Board &board);
};


#endif //OTHELLOPROJECT_EVALBASICHEURISTICS_H
