//
// Created by louis on 6/13/19.
//

#include "EngineMinimaxBetter.h"

#include <bitset>
#include <bits/stdc++.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include "../searchUtils/Extensions.h"
#include "../searchUtils/TimeManagement.h"
#include "../protocols/Uci.h"
#include "../Art.h"
#include "../main.h"
#include "../Board.h"

using namespace std;


unsigned long aiMoveLong;
int aiMoveScore = 0;
bool running = false;

EngineMinimaxBetter::EngineMinimaxBetter(int maxDepth, bool printInfo, EvalBase *evaluator, string name,
                                         TimePoint timeLimit) {
    this->maxDepth = maxDepth != 0 ? maxDepth : absoluteMaxDepth;
    this->printInfo = printInfo;
    this->printDebug = false;
    this->evaluator = evaluator;
    this->name = name;
    this->timeLimit = timeLimit;
}

unsigned int EngineMinimaxBetter::getBestMoveInt(Board &board) {
    return getIndexLowestBit(getBestMove(board));
}

unsigned int EngineMinimaxBetter::getDisplayScoreOfMove(Board &board) {
    return aiMoveScore;
}

unsigned long getNps(long nodes, long time) {
    if (time == 0) {
        return 0;
    }
    return (nodes / time) * 1000;
}


static unsigned long totalNodes = 0;

void reset() {
    totalNodes = 0;
}

unsigned long EngineMinimaxBetter::getBestMove(Board &board) {
    reset();

    aiMoveScore = 0;
    aiMoveLong = -1;

    this->startTime = now();

    this->stopTime = this->timeLimit + startTime;

    running = true;

    iterativeDeepeningSearch(board);

    running = false;

    if (this->printDebug) {
        TimePoint currentTime = now();

        long finalTime = currentTime - startTime;
        cout << "\nThe search lasted " << finalTime << " milliseconds." << endl;
        cout << "NPS: " << getNps(totalNodes, finalTime) << endl;
    }

    unsigned int moveIndex = getIndexLowestBit(aiMoveLong);

    assert(moveIndex >= 0);
    assert(moveIndex <= 63);
    assert(popCount(aiMoveLong) == 1);

    return aiMoveLong;
}

bool EngineMinimaxBetter::stopSearch() {
    if (!running) {
        return true;
    }
    bool b = now() > this->stopTime - 1;
    if (b) {
        running = false;
        cout << "time to stop" << endl;
    }
    return b;
}

unsigned long EngineMinimaxBetter::iterativeDeepeningSearch(Board &board) {
    unsigned long bestMove = 0;
    int bestScore, initialAlpha = EvalBase::SHORT_MIN, initialBeta = EvalBase::SHORT_MAX;

    int depth = 0;

    while (running){
        depth++;
        bestScore = principleVariationSearch(board, depth, 0, initialAlpha, initialBeta, false);

        if (printInfo) {
            TimePoint n = now();
            sendInfoString(depth, aiMoveScore, getNps(totalNodes, n - this->startTime), n,
                           aiMoveLong);
        }

        if (stopSearch()) {
            assert(!running);
            if (printDebug) {
                cout << "out of time ids" << endl;
            }
            break;
        }
    }

    if (printInfo) {
//        sendInfoString(depth, aiMoveScore, getNps(totalNodes, now() - this->startTime),
//                       now(), aiMoveLong);
        sendBestMove(aiMoveLong);
    }


    return bestMove;
}

unsigned long
EngineMinimaxBetter::principleVariationSearch(Board &board, int depth, int ply, int alpha, int beta, bool extended) {
    unsigned long moves = board.generateLegalMoves();

    unsigned int ex = extended ? 0 : getExtension(board, moves);
    if (ex) {
        extended = true;
    }
    depth += 2 * ex;

    if (depth <= 0) {
        return evaluator->eval(board, moves);
    }

    unsigned long move = 0;
    unsigned long bestMove = 0;
    int score = 0, bestScore = EvalBase::SHORT_MIN, movesMade = 0;

    if (moves == 0) {

        board.flipTurn();
        unsigned long opponentMoves = board.generateLegalMoves();
        board.flipTurn();

        if (opponentMoves == 0) {
            unsigned long m = board.getMyPieces();
            unsigned long u = board.getEnemyPieces();

            if (m == u) {
                return EvalBase::DRAW_SCORE;
            }
            if (m > u) {
                return EvalBase::CHECKMATE_SCORE;
            }

            return EvalBase::IN_CHECKMATE_SCORE;
        }

        board.flipTurn();
        score = -principleVariationSearch(board, depth - 1, ply + 1, -beta, -alpha, extended);
        board.flipTurn();

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
            if (ply == 0) {
                aiMoveLong = bestMove;
                aiMoveScore = bestScore;
            }
        }

    } else {
        while (moves) {
            move = moves & -moves;

            assert(move);

            unsigned int moveIndex = getIndexLowestBit(move);

            board.makeMove(moveIndex);

            movesMade++;
            totalNodes++;

            score = -principleVariationSearch(board, depth - 1, ply + 1, -beta, -alpha, extended);

            board.unMakeMove();

            if (!running || stopSearch()) {
                return 0;
            }

            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
                if (score > alpha) {
                    alpha = score;
                }
                if (ply == 0) {
                    aiMoveLong = bestMove;
                    aiMoveScore = bestScore;
                }
            }

            if (alpha >= beta) {
                break;
            }

            moves &= moves - 1;
        }
    }

    if (now() > this->stopTime) {
        return 0;
    }


    if (bestScore == EvalBase::SHORT_MIN) {
        printBoardWithIndexAndLegalMoves(board);
    }
    assert(bestScore != EvalBase::SHORT_MIN);

    return bestScore;
}

