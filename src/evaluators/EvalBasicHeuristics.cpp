//
// Created by louis on 6/8/19.
//

#include "EvalBasicHeuristics.h"
#include "../BitBoardUtils.h"

const int materialWeight = 1;
const int cornerWeight = 14;
const int moveWeight = 2;
const int turnWeight = 10;
const int smallSpoonWeightCorner = 5;
const int smallSpoonWeightNoCorner = -25;
const int bigSpoonWeightNoCorner = 15;
const int enemiesHasMoreBorderPiecesThanMe = 10;
const int iWillHaveLastMove = 15;

int EvalBasicHeuristics::eval(const Board &board, uint64_t moves) {
    int finalScore = 0;
    const bool w = board.turn == WHITE;

    int colourScore = 0;
    for (int i = WHITE; i <= BLACK; i++) {
        colourScore = 0;
        const uint64_t enemies = i == BLACK ? board.whitePieces : board.blackPieces;
        const uint64_t friends = i == WHITE ? board.whitePieces : board.blackPieces;

        const uint64_t allPieces = enemies | friends;
        const uint64_t allBorderPieces = board.getBorderPieces();
        const uint64_t myBorderPieces = allBorderPieces & friends;
        const uint64_t enemyBorderPieces = allBorderPieces & enemies;

        if (popCount(allPieces) & 1) {
            colourScore += iWillHaveLastMove;
        }
        if (popCount(enemyBorderPieces) > popCount(myBorderPieces)) {
            colourScore += enemiesHasMoreBorderPiecesThanMe;
        }

        const int myCorners = popCount(friends & CORNERS);
        colourScore += myCorners * cornerWeight;

        colourScore += popCount(friends) * materialWeight;


        for (int c = 0; c < 4; c++) {
            if (friends & CORNER_ARRAY[c]) {
                // todo, code safe rectangles
                colourScore += popCount(friends & LITTLE_SPOON_ARRAY[c]) * smallSpoonWeightCorner;
            } else {
                colourScore -= popCount(friends & LITTLE_SPOON_ARRAY[c]) * smallSpoonWeightNoCorner;
            }
            colourScore += popCount(friends & BIG_SPOON_ARRAY[c]) * bigSpoonWeightNoCorner;
        }

        if (i == board.turn) {
            finalScore += colourScore;
        } else {
            finalScore -= colourScore;
        }
    }

    finalScore += popCount(moves) * moveWeight;

    finalScore += turnWeight;

    return finalScore;
}


