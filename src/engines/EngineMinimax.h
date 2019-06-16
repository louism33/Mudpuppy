//
// Created by louis on 6/5/19.
//

#ifndef OTHELLOPROJECT_ENGINEMINIMAX_H
#define OTHELLOPROJECT_ENGINEMINIMAX_H

#include <bitset>
#include "../main.h"
#include <sstream>
#include <iostream>
#include "../Art.h"
#include "../Board.h"
#include "EngineBase.h"
#include "../evaluators/EvalBase.h"
#include "../evaluators/EvalSimpleCount.h"

class EngineMinimax: public EngineBase{
public:

    explicit EngineMinimax(int maxDepth, bool print = false, EvalBase *evaluator = new EvalSimpleCount(), std::string name = "no name given");

    int maxDepth = 0;

    bool print = false;

    EvalBase *evaluator;

    unsigned long getBestMove(Board &board) override;

    unsigned int getBestMoveInt(Board &board) override;

    unsigned int getDisplayScoreOfMove(Board &board) override;

    unsigned long principleVariationSearch(Board &board, int depth, int ply, int alpha, int beta);

    unsigned long iterativeDeepeningSearch(Board &board);
};

void resetMinimaxBetter();

#endif //OTHELLOPROJECT_ENGINEMINIMAX_H
