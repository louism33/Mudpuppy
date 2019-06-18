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
#include "../Move.h"
#include "../searchUtils/MoveOrderer.h"
#include "../searchUtils/TranspositionTable.h"
#include "../BitBoardUtils.h"

using namespace std;

unsigned long aiMoveLong;
int aiMoveScore = 0;
bool running = false;
unsigned long movesArray[64][32];
unsigned int age = 0;

EngineMinimaxBetter::EngineMinimaxBetter(int maxDepth, bool printInfo, EvalBase *evaluator, string name,
                                         TimePoint timeLimit) {
    this->maxDepth = maxDepth != 0 ? maxDepth : absoluteMaxDepth;
    this->printInfo = printInfo;
    this->printDebug = false;
    this->evaluator = evaluator;
    this->name = name;
    this->timeLimit = timeLimit;
//    this->movesArray = new unsigned long[64][32];
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
    age++;
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
//        cout << "time to stop" << endl;
    }
    return b;
}


unsigned long EngineMinimaxBetter::iterativeDeepeningSearch(Board &board) {
    unsigned long bestMove = 0;
    int bestScore, initialAlpha = EvalBase::SHORT_MIN, initialBeta = EvalBase::SHORT_MAX;

    int depth = 0;

    while (running) {
        depth++;
        bestScore = principleVariationSearch(board, depth, 0, initialAlpha, initialBeta, false, false);

        if (printInfo) {
            TimePoint n = now();
            sendInfoString(&board, depth, aiMoveScore, getNps(totalNodes, n - this->startTime), n - this->startTime,
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
//        sendInfoString(&board, depth, aiMoveScore, getNps(totalNodes, now() - this->startTime),
//                       now(), aiMoveLong);
        sendBestMove(aiMoveLong);
    }


    return bestMove;
}

unsigned long
EngineMinimaxBetter::principleVariationSearch(Board &board, int depth, int ply, int alpha, int beta, bool extended,
                                              bool passed) {
    unsigned long moves = board.generateLegalMoves();
    unsigned int movesMadeInGame = board.numberOfMoves;
    const int originalAlpha = alpha;

    // only seek extension if we did not extend last turn. Allow extensions every other turn
    unsigned int ex = extended ? 0 : getExtension(board, moves);
    if (ex) {
        extended = true;
    } else {
        // todo seems to create massive slow down around depth 8 9
//        extended = false;
    }
    depth += ex;

    if (depth <= 0) {
        return evaluator->eval(board, moves);
    }

    Entry *entry = retrieveFromTable(&board);

    unsigned long bestMove = 0, hashMove = 0;
    int m = 0, score = 0, bestScore = EvalBase::SHORT_MIN, movesMade = 0;

    if (entry) {
        score = entry->score;
        hashMove = entry->bestMove;

        if (entry->depth >= depth) {
            Flag flag = entry->flag;
            if (flag == EXACT) {
                if (ply == 0) {
                    aiMoveLong = hashMove;
                    aiMoveScore = score;
                    }
                return score;
            } else if (flag == LOWERBOUND) {
                if (score >= beta) {
                    if (ply == 0) {
                        aiMoveLong = hashMove;
                        aiMoveScore = score;
                    }
                    return score;
                }
            } else if (flag == UPPERBOUND) {
                if (score <= alpha) {
                    if (ply == 0) {
                        aiMoveLong = hashMove;
                        aiMoveScore = score;
                    }
                    return score;
                }
            }
        }

    }

    if (hashMove == 0) {
        assert(!entry);
    }
    if (!entry) {
        assert(hashMove == 0);
    }

    if (hashMove == PASS_MOVE) {
        assert(moves == 0);
    }

    getOrderedMovesAsArray(movesArray[movesMadeInGame], moves, hashMove);
    unsigned long move = 0;
    for (m = 1; m < movesArray[movesMadeInGame][0]; m++) {
        move = movesArray[movesMadeInGame][m];
        if (move == 0) {
            break;
        }

        if (hashMove != 0 && m == 1) {
            assert(move == hashMove);
        }

        bool passingMove = move == PASS_MOVE;

        if (passingMove && passed) {
            unsigned long me = board.getMyPieces();
            unsigned long u = board.getEnemyPieces();

            if (true){
                assert(board.generateLegalMoves() == 0);
                board.flipTurn();
                assert(board.generateLegalMoves() == 0);
                board.flipTurn();
            }

            if (me == u) {
                return EvalBase::DRAW_SCORE;
            }
            if (me > u) {
                return EvalBase::CHECKMATE_SCORE;
            }
            return EvalBase::IN_CHECKMATE_SCORE;
        }

        bool cornerMove = (move & CORNERS) != 0;
        bool dangerousAreaMove = (move & GENERAL_EDGE_OF_BOARD) != 0;
        unsigned int moveIndex = getIndexLowestBit(move);

        board.makeMoveLong(board.turn, move);
        movesMade++;
        totalNodes++;

        score = alpha + 1;

        if (movesMade > 2 && !dangerousAreaMove) {
            int R = (depth - 3) - (movesMade / 5) - (depth / 5);
            score = -principleVariationSearch(board, R > 0 ? R : 0, ply + 1, -alpha - 1, -alpha, extended, passingMove);

        }
        if (movesMade > 1 && score > alpha) {
            score = -principleVariationSearch(board, depth - 1, ply + 1, -alpha - 1, -alpha, extended, passingMove);
        }
        if (score > alpha){
            score = -principleVariationSearch(board, depth - 1, ply + 1, -beta, -alpha, extended, passingMove);
        }

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

    }

    if (now() > this->stopTime) {
        return 0;
    }


    assert(bestMove != 0);
    assert(bestScore != EvalBase::SHORT_MIN);

    Flag flag;
    if (bestScore <= originalAlpha) {
        flag = UPPERBOUND;
    } else if (bestScore >= beta) {
        flag = LOWERBOUND;
    } else {
        flag = EXACT;
    }

    addToTableReplaceByDepth(&board, bestMove, bestScore, flag, depth, age);

    return bestScore;
}





