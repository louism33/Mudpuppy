//
// Created by louis on 6/13/19.
//

#include "Extensions.h"
#include "../BitBoardUtils.h"

unsigned int getExtension(Board &board, unsigned long moves) {

    // extend if can go to corners
    if (moves & CORNERS) {
        return 1;
    }

    // extend if the move may allow a corner capture
    if (moves & LITTLE_SPOON_SQUARES) {
        return 1;
    }

    // singular move extension
    if (popCount(moves) == 1) {
        return 1;
    }

    // end of game extension
    if (popCount(board.whitePieces | board.blackPieces) >= 60) {
        return 1;
    }

    return 0;
}
