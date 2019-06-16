//
// Created by louis on 6/8/19.
//

#include "EvalBasicHeuristics.h"
#include "../BitBoardUtils.h"

int materialWeight = 2;
int cornerWeight = 10;
int moveWeight = 2;
int turnWeight = 10;

int EvalBasicHeuristics::eval(const Board &board, unsigned long moves) {
    int score = 0;

    const int material = popCount(board.whitePieces) - popCount(board.blackPieces);
    const int weightedMaterial = (board.turn == WHITE ? material : -material) * materialWeight;

    score += weightedMaterial;

    const int myCorners = popCount(board.getMyPieces() & CORNERS);
    score += myCorners * cornerWeight;

    const int enemyCorners = popCount(board.getEnemyPieces() & CORNERS);
    score += enemyCorners * cornerWeight;

    score += popCount(moves) * moveWeight;

    score += turnWeight;

    return score;
}


int EvalBasicHeuristics::evalTurn(const Board &board) {

}