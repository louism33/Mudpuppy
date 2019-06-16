//
// Created by louis on 6/5/19.
//

#include <bitset>
#include <bits/stdc++.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include "../main.h"
#include "../Art.h"
#include "../Board.h"
#include "EngineMinimax.h"

using namespace std;

static unsigned long aiMoveLong;
static int aiMoveScore;

EngineMinimax::EngineMinimax(int maxDepth, bool print, EvalBase *evaluator, string name) {
    this->maxDepth = maxDepth;
    this->print = print;
    this->evaluator = evaluator;
    this->name = name;
}

unsigned int EngineMinimax::getBestMoveInt(Board &board) {
    return getIndexLowestBit(getBestMove(board));
}

unsigned int EngineMinimax::getDisplayScoreOfMove(Board &board){
    return aiMoveScore;
}

unsigned long getNpsMinimaxBetter(long nodes, long time){
    if (time == 0) {
        return 0;
    }
    return (nodes / time) * 1000;
}

static unsigned long totalNodes = 0;

void resetMinimaxBetter(){
    totalNodes = 0;
}

unsigned long EngineMinimax::getBestMove(Board &board) {
    resetMinimaxBetter();

    aiMoveScore = 0;
    aiMoveLong = -1;

    chrono::milliseconds startTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    iterativeDeepeningSearch(board);

    if (this->print){
        chrono::milliseconds currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
        );

        long finalTime = currentTime.count() - startTime.count();
        cout << "\nThe search lasted " << finalTime << " milliseconds." << endl;
        cout << "NPS: " << getNpsMinimaxBetter(totalNodes, finalTime) << endl;
    }

    unsigned int moveIndex = getIndexLowestBit(aiMoveLong);

    if (moveIndex < 0 || moveIndex > 63) {
        printBoardWithIndexAndLegalMoves(board);
        printLong(aiMoveLong);
        cout << aiMoveScore << endl;
    }
    assert(moveIndex >= 0);
    assert(moveIndex <= 63);
    assert(popCount(aiMoveLong) == 1);

    return aiMoveLong;
}

unsigned long EngineMinimax::iterativeDeepeningSearch(Board &board) {

    unsigned long bestMove = 0;
    int bestScore, initialAlpha = EvalBase::SHORT_MIN, initialBeta = EvalBase::SHORT_MAX;

    bestScore = principleVariationSearch(board, maxDepth, 0, initialAlpha, initialBeta);

    if (print){
        cout << "best move found: " + getMoveStringFromMove(aiMoveLong) << endl;
        cout << "best score: " + to_string(aiMoveScore) << endl;
    }

    return bestMove;
}

unsigned long EngineMinimax::principleVariationSearch(Board &board, int depth, int ply, int alpha, int beta) {
    unsigned long moves = board.generateLegalMoves();

    if (depth <= 0) {
        return evaluator->eval(board, moves);
    }

    unsigned long move = 0;
    unsigned long bestMove = 0;
    int score = 0, bestScore = EvalBase::SHORT_MIN, movesMade = 0;

    if (moves == 0){

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
        score = -principleVariationSearch(board, depth - 1, ply + 1, -beta, -alpha);
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
            score = -principleVariationSearch(board, depth - 1, ply + 1, -beta, -alpha);
            board.unMakeMove();
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
                if (score > alpha){
                    alpha = score;
                }
                if (ply == 0) {
                    aiMoveLong = bestMove;
                    aiMoveScore = bestScore;
                }
            }

            if (alpha >= beta){
                break;
            }

            moves &= moves - 1;
        }

    }

    if (bestScore == EvalBase::SHORT_MIN) {
        printBoardWithIndexAndLegalMoves(board);
    }
    assert(bestScore != EvalBase::SHORT_MIN);

    return bestScore;
}

