//
// Created by louis on 6/24/19.
//

#ifndef MUDPUPPY_ENGINEMINIMAXV3_H
#define MUDPUPPY_ENGINEMINIMAXV3_H

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
#include "../searchUtils/TranspositionTable.h"


class EngineMinimaxV3 : public EngineBase {
private:
    TimePoint timeLimit;
    TimePoint startTime;
    TimePoint stopTime;
    uint64_t aiMoveLong;
    int aiMoveScore = 0;
    bool running = false, debug = true;
    uint64_t movesArray[128][32];
    uint32_t age = 0;
    TranspositionTable tt;

public:

    explicit EngineMinimaxV3(int maxDepth, bool printInfo = false, EvalBase *evaluator = new EvalSimpleCount(),
                             std::string name = "no name given",
                             TimePoint timeLimit = 1000l);

    int maxDepth = 0;
    const int absoluteMaxDepth = 128;
    const TimePoint absoluteMaxLimit = TimePoint(1'000'000);

    bool printDebug = false;
    bool printInfo = false;

    EvalBase *evaluator;

    void fullReset();

    uint64_t getBestMove(Board &board) override;

    uint32_t getBestMoveInt(Board &board) override;

    int getDisplayScoreOfMove(Board &board) override;

    uint64_t principleVariationSearch(Board &board, int depth, int ply, int alpha, int beta, bool extended,
                                           bool passed);

    uint64_t iterativeDeepeningSearch(Board &board);

    bool stopSearch();

    void reset();

    static uint64_t getNps(long nodes, long time);

    void setTimeLimitMillis(TimePoint limit);

    void setDepthLimit(int depth);

    void setPrint(bool b);
};



#endif //MUDPUPPY_ENGINEMINIMAXV3_H
