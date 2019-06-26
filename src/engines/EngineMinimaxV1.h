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

class EngineMinimaxV1: public EngineBase{
public:

    explicit EngineMinimaxV1(int maxDepth, bool print = false, EvalBase *evaluator = new EvalSimpleCount(), std::string name = "no name given");

    int maxDepth = 0;

    bool print = false;

    EvalBase *evaluator;

    uint64_t getBestMove(Board &board) override;

    uint32_t getBestMoveInt(Board &board) override;

    int getDisplayScoreOfMove(Board &board) override;

    uint64_t principleVariationSearch(Board &board, int depth, int ply, int alpha, int beta);

    uint64_t iterativeDeepeningSearch(Board &board);
};

void resetMinimaxBetter();

#endif //OTHELLOPROJECT_ENGINEMINIMAX_H
