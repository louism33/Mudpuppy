//
// Created by louis on 6/13/19.
//

#ifndef OTHELLOPROJECT_ENGINEMINIMAXBETTER_H
#define OTHELLOPROJECT_ENGINEMINIMAXBETTER_H

#include <bitset>
#include "../main.h"
#include <sstream>
#include <iostream>
#include <chrono>
#include "../Art.h"
#include "../Board.h"
#include "EngineBase.h"
#include "../evaluators/EvalBase.h"
#include "../evaluators/EvalSimpleCount.h"
#include "../searchUtils/TimeManagement.h"

class EngineMinimaxBetter : public EngineBase {
private:
    TimePoint timeLimit;
    TimePoint startTime;
    TimePoint stopTime;

public:

    explicit EngineMinimaxBetter(int maxDepth, bool printInfo = false, EvalBase *evaluator = new EvalSimpleCount(),
                                 std::string name = "no name given",
                                 TimePoint timeLimit = 1000l);

    int maxDepth = 0;
    const int absoluteMaxDepth = 64;

    bool printDebug = false;
    bool printInfo = false;

    EvalBase *evaluator;

    unsigned long getBestMove(Board &board) override;

    unsigned int getBestMoveInt(Board &board) override;

    unsigned int getDisplayScoreOfMove(Board &board) override;

    unsigned long principleVariationSearch(Board &board, int depth, int ply, int alpha, int beta, bool extended);

    unsigned long iterativeDeepeningSearch(Board &board);

    bool stopSearch();
};


#endif //OTHELLOPROJECT_ENGINEMINIMAXBETTER_H
