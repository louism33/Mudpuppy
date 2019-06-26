//
// Created by louis on 6/13/19.
//

#include <assert.h>
#include "Extensions.h"
#include "../BitBoardUtils.h"

uint32_t getExtension(Board &board, uint64_t moves, int ply, bool extended) {
    //don't extend root
    if (ply < 1) {
        return 0;
    }

//    assert(popCount(moves) > 1);

    // extend on passes
    if (extended) {
        return 0;
    }

    // extend if can go to corners
    if ((moves & CORNERS) != 0) {
        return 1;
    }

    // extend if the move may allow a corner capture
    if (moves & LITTLE_SPOON_SQUARES) {
        return 1;
    }

    return 0;
}

// extensions which can be safely applied recursively without causing explosions
uint32_t getSafeExtension(Board &board, uint64_t moves, int ply) {

    //don't extend root
    if (ply < 1) {
        return 0;
    }

    // extend on passes
    int c = popCount(moves);
    if (c == 1) {
        return 1;
    }
//    // end of game extension
    if (popCount(board.whitePieces | board.blackPieces) >= 62) {
        return 1;
    }

    return 0;
}