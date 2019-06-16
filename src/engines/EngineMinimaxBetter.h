//
// Created by louis on 6/13/19.
//

#ifndef OTHELLOPROJECT_ENGINEMINIMAXBETTER_H
#define OTHELLOPROJECT_ENGINEMINIMAXBETTER_H

#include <bitset>
#include "../main.h"
#include <sstream>
#include <iostream>
#include "../Art.h"
#include "../Board.h"
#include "EngineBase.h"
#include "../evaluators/EvalBase.h"
#include "../evaluators/EvalSimpleCount.h"

class EngineMinimaxBetter : public EngineBase {
public:

    explicit EngineMinimaxBetter(int maxDepth, bool print = false, EvalBase *evaluator = new EvalSimpleCount(),
                                 std::string name = "no name given");

    int maxDepth = 0;

    bool print = false;

    EvalBase *evaluator;

    unsigned long getBestMove(Board &board) override;

    unsigned int getBestMoveInt(Board &board);

    unsigned int getDisplayScoreOfMove(Board &board);

    unsigned long principleVariationSearch(Board &board, int depth, int ply, int alpha, int beta, bool extended);

    unsigned long iterativeDeepeningSearch(Board &board);
};


#endif //OTHELLOPROJECT_ENGINEMINIMAXBETTER_H
