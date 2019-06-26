//
// Created by louis on 6/8/19.
//

#ifndef OTHELLOPROJECT_EVALBASICHEURISTICS_H
#define OTHELLOPROJECT_EVALBASICHEURISTICS_H


#include "EvalBase.h"


class EvalBasicHeuristics : public EvalBase{
public:
    int eval(const Board &board, uint64_t moves);

};


#endif //OTHELLOPROJECT_EVALBASICHEURISTICS_H
