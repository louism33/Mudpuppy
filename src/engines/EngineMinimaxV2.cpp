//
// Created by louis on 6/13/19.
//

#include "EngineMinimaxV2.h"

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

EngineMinimaxV2::EngineMinimaxV2(int maxDepth, bool printInfo, EvalBase *evaluator, string name,
                                         TimePoint timeLimit) {
    bool goToDepth = maxDepth != 0;
    this->maxDepth = goToDepth ? maxDepth : absoluteMaxDepth;
    this->printInfo = printInfo;
    this->printDebug = false;
    this->evaluator = evaluator;
    this->name = name;
    this->timeLimit = !goToDepth ? timeLimit : absoluteMaxLimit;
    this->tt = TranspositionTable();
    fullReset();
}

uint32_t EngineMinimaxV2::getBestMoveInt(Board &board) {
    return getIndexLowestBit(getBestMove(board));
}

int EngineMinimaxV2::getDisplayScoreOfMove(Board &board) {
    return aiMoveScore;
}

uint64_t EngineMinimaxV2::getNps(long nodes, long time) {
    if (time == 0) {
        return 0;
    }
    return (nodes / time) * 1000;
}


static uint64_t totalNodes = 0;

void EngineMinimaxV2::reset() {
    totalNodes = 0;
    age++;
}

void EngineMinimaxV2::fullReset() {
    reset();
    age = 0;
    tt.resetTT();
}

uint64_t EngineMinimaxV2::getBestMove(Board &board) {
    reset();

    aiMoveScore = 0;
    aiMoveLong = -1;

    this->startTime = now();

    this->stopTime = this->timeLimit + startTime;

    running = true;

    uint64_t rootMoves = board.generateLegalMoves();

    if (popCount(rootMoves) == 0) {
        return PASS_MOVE;
    }
    if (popCount(rootMoves) == 1) {
        return rootMoves;
    }

    iterativeDeepeningSearch(board);

    running = false;

    if (this->printDebug) {
        TimePoint currentTime = now();

        long finalTime = currentTime - startTime;
        cout << "\nThe search lasted " << finalTime << " milliseconds." << endl;
        cout << "NPS: " << getNps(totalNodes, finalTime) << endl;
    }

    uint32_t moveIndex = getIndexLowestBit(aiMoveLong);

    if (debug){
        assert(moveIndex >= 0);
        assert(moveIndex <= 63);
        assert((rootMoves & aiMoveLong) != 0);
        assert(popCount(aiMoveLong) == 1);
    }

    return aiMoveLong;
}

bool EngineMinimaxV2::stopSearch() {
    if (!running) {
        return true;
    }
    bool b = now() > this->stopTime - 1;
    if (b) {
        running = false;
    }
    return b;
}


uint64_t EngineMinimaxV2::iterativeDeepeningSearch(Board &board) {
    uint64_t bestMove = 0;
    int aspirationAlphaIndex = 0, aspirationBetaIndex = 0;
    int aspirationValues[]{10, 25, 100}, maxAspirationTries = 3;

    int score = 0,
            alpha = EvalBase::SHORT_MIN,
            beta = EvalBase::SHORT_MAX;

    int depth = 0;
    bool lastPrint = false;
    while (running) {
        depth++;

        assert(depth > 0);


        if (depth > maxDepth) {
            break;
        }

        if (depth >= 5) {
            aspirationAlphaIndex = 0;
            aspirationBetaIndex = 0;
            alpha = score - aspirationValues[aspirationAlphaIndex];
            beta = score + aspirationValues[aspirationBetaIndex];
        }

        while (running) {

            score = principleVariationSearch(board, depth, 0, alpha, beta, false, false);

            assert(depth <= maxDepth);

            if (score >= EvalBase::CHECKMATE_SCORE) {
                assert(aiMoveScore >= EvalBase::CHECKMATE_SCORE);
                lastPrint = true;
                goto end;
            }

            if (!running) {
                goto end;
            }

            if (score <= alpha) {
                aspirationAlphaIndex++;
                if (aspirationAlphaIndex >= maxAspirationTries) {
                    alpha = EvalBase::SHORT_MIN;
                } else {
                    alpha = score - aspirationValues[aspirationAlphaIndex];
                }
            } else if (score >= beta) {
                aspirationBetaIndex++;
                if (aspirationBetaIndex >= maxAspirationTries) {
                    beta = EvalBase::SHORT_MAX;
                } else {
                    beta = score + aspirationValues[aspirationBetaIndex];
                }
            } else {
                break;
            }
        }

        if (printInfo) {
            TimePoint t = now() - this->startTime;
            sendInfoString(&board, depth, aiMoveScore, getNps(totalNodes, t), t,
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

    end:
    if (printInfo) {
        if (lastPrint) {
            TimePoint t = now() - this->startTime;
            sendInfoString(&board, depth, aiMoveScore, getNps(totalNodes, t),
                           t, aiMoveLong);
        }
        sendBestMove(aiMoveLong);
    }


    return bestMove;
}

uint64_t
EngineMinimaxV2::principleVariationSearch(Board &board, int depth, int ply, int alpha, int beta, bool extended,
                                              bool passed) {
    uint64_t moves = board.generateLegalMoves();
    const uint64_t originalMovesForDebug = moves;
    uint32_t movesMadeInGame = board.numberOfRealMoves;
    const int originalAlpha = alpha;

    // have not found any extension to increase strength
//    uint32_t safeEx = getSafeExtension(board, moves, ply);
//    uint32_t ex = safeEx != 0 ? 0 : getExtension(board, moves, ply, extended);
//    extended = ex > 0; // if we extended, do not extend next turn. Else, allow extension next turn.
//    depth += safeEx + ex;

    if (depth <= 0) {
        return evaluator->eval(board, moves);
    }

    const bool PV_NODE = (beta - alpha) != 1;

    Entry *entry = tt.retrieveFromTable(&board);

    uint64_t bestMove = 0, hashMove = 0;
    int m = 0, score = 0, bestScore = EvalBase::SHORT_MIN, movesMade = 0;

    if (entry) {
        score = entry->score;
        hashMove = entry->bestMove;

        if (entry->depth >= depth) {
            Flag flag = entry->flag;
            if (flag == EXACT) {
                if (ply == 0) {
                    assert(popCount(hashMove) == 1);
                    aiMoveLong = hashMove;
                    aiMoveScore = score;
                }
                return score;
            } else if (flag == LOWERBOUND) {
                if (score >= beta) {
                    if (ply == 0) {
                        assert(popCount(hashMove) == 1);
                        aiMoveLong = hashMove;
                        aiMoveScore = score;
                    }
                    return score;
                }
            } else if (flag == UPPERBOUND) {
                if (score <= alpha) {
                    if (ply == 0) {
                        assert(popCount(hashMove) == 1);
                        aiMoveLong = hashMove;
                        aiMoveScore = score;
                    }
                    return score;
                }
            }
        }
    }

    if (debug) {
        if (hashMove == 0) {
            assert(!entry);
        }
        if (!entry) {
            assert(hashMove == 0);
        }

        if (hashMove == PASS_MOVE) {
            assert(moves == 0);
        }
    }

    getOrderedMovesAsArray(movesArray[movesMadeInGame], moves, hashMove);
    uint64_t move = 0;
    for (m = 1; m < movesArray[movesMadeInGame][0]; m++) {
        move = movesArray[movesMadeInGame][m];
        if (move == 0) {
            break;
        }

        bool passingMove = move == PASS_MOVE;

        if (passingMove && passed) {
            uint64_t me = board.getMyPieces();
            uint64_t u = board.getEnemyPieces();

            if (debug) {
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
        bool dangerousAreaMove = (move & GENERAL_CORNER_AREA) != 0;
        uint32_t moveIndex = getIndexLowestBit(move);

        bool b = (move == PASS_MOVE) || (originalMovesForDebug & move) != 0;
        if (!b) {
            printBoardWithIndexAndLegalMoves(board);
            printLong(originalMovesForDebug);
            printLong(moves);
            printLong(move);
        }

        if (debug) {
            assert(b);
            assert(originalMovesForDebug == moves);
        }

        board.makeMoveLong(board.turn, move);
        movesMade++;

        if (debug) {
            if (!passingMove) {
                assert(movesMade <= popCount(moves));
            } else {
                assert(movesMade == 1);
            }
        }

        totalNodes++;

        score = alpha + 1;

        // LMR
        if (movesMade > 2 && !dangerousAreaMove && !passingMove) {
            assert(move != PASS_MOVE);
            assert(move != hashMove);
            int R = 3;
            R += (depth / 4);

            int allPiecesCount = popCount(board.allPieces());

            // try less reduction in end game
            if (allPiecesCount >= 54) {
                R--;
            }
            if (!PV_NODE) {
                R++;
            }
            score = -principleVariationSearch(board, depth - R > 0 ? depth - R : 0, ply + 1, -alpha - 1, -alpha,
                                              extended, passingMove);

        }
        if (movesMade > 1 && score > alpha) {
            assert(move != PASS_MOVE);
            assert(move != hashMove);
            score = -principleVariationSearch(board, depth - 1, ply + 1, -alpha - 1, -alpha, extended, passingMove);
        }
        if (score > alpha) {
            if (debug) {
                if (moves == 0) {
                    assert(move == PASS_MOVE);
                }
                if (movesMade == 0 && hashMove != 0) {
                    assert(move == hashMove);
                }
            }
            score = -principleVariationSearch(board, depth - 1, ply + 1, -beta, -alpha, extended, passingMove);
        }

        board.unMakeMove();

        if (!running || stopSearch()) {
            return 0;
        }

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
            if (bestMove != PASS_MOVE) {
                assert(popCount(bestMove) == 1);
                assert((bestMove & moves) != 0);
            }
            if (score > alpha) {
                alpha = score;
            }
            if (ply == 0) {
                if (bestMove != PASS_MOVE) {
                    assert(popCount(bestMove) == 1);
                    assert((bestMove & moves) != 0);
                }
                assert(popCount(bestMove) == 1);
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

    tt.addToTableReplaceByDepth(&board, bestMove, bestScore, flag, depth, age);

    return bestScore;
}

void EngineMinimaxV2::setTimeLimitMillis(TimePoint limit) {
    this->maxDepth = absoluteMaxDepth;
    this->timeLimit = limit;
}

void EngineMinimaxV2::setDepthLimit(int depth) {
    this->maxDepth = depth;
    this->timeLimit = absoluteMaxLimit;
}


