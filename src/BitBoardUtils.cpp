//
// Created by louis on 6/6/19.
//

//static uint64_t starMask[64];

#include <iostream>
#include <bitset>
#include <iostream>
#include <assert.h>
#include <math.h>
#include <jmorecfg.h>
#include "Art.h"
#include "main.h"
#include "BitBoardUtils.h"

using namespace std;
bool ready = false;
uint64_t starMask[64];
uint64_t starMaskRook[64];
uint64_t starMaskBishop[64];
uint64_t starLR[64];
uint64_t starUD[64];
uint64_t starDLUR[64];
uint64_t starULDR[64];

void setupStarMask() {
    if (ready) {
        return;
    }
    for (int i = 0; i < 64; i++) {
        uint64_t piece = newPieceOnSquare(i);
        starLR[i] = makeLR(piece);
        starUD[i] = makeUD(piece);
        starDLUR[i] = makeDLUR(piece);
        starULDR[i] = makeULDR(piece);

        starMaskRook[i] = starLR[i] | starUD[i];
        starMaskBishop[i] = starDLUR[i] | starULDR[i];
        starMask[i] = starMaskRook[i] | starMaskBishop[i];
    }

    ready = true;
}


// exclude piece, include border
uint64_t makeLR(uint64_t piece) {
    uint64_t star = 0;

    uint64_t pieceTemp = piece & ~L_BORDER;
    if (pieceTemp != 0) {
        while (((pieceTemp & L_BORDER) == 0)) {
            pieceTemp = pieceTemp << 1u;
            star |= pieceTemp;
            if (pieceTemp & L_BORDER) {
                break;
            }
        }
    }

    pieceTemp = piece & ~R_BORDER;
    if (pieceTemp != 0) {
        while (((pieceTemp & R_BORDER) == 0)) {
            pieceTemp = pieceTemp >> 1u;
            star |= pieceTemp;
            if (pieceTemp & R_BORDER) {
                break;
            }
        }
    }

    assert(star);

    return star;
}

uint64_t makeUD(uint64_t piece) {
    uint64_t star = 0;

    uint64_t pieceTemp = 0;

    pieceTemp = piece & ~U_BORDER;
    if (pieceTemp != 0) {
        while (((pieceTemp & U_BORDER) == 0)) {
            pieceTemp = pieceTemp << 8u;
            star |= pieceTemp;
            if (pieceTemp & U_BORDER) {
                break;
            }
        }
    }

    pieceTemp = piece & ~D_BORDER;
    if (pieceTemp != 0) {
        while (((pieceTemp & D_BORDER) == 0)) {
            pieceTemp = pieceTemp >> 8u;
            star |= pieceTemp;
            if (pieceTemp & D_BORDER) {
                break;
            }
        }
    }

    assert(star);

    return star;
}



uint64_t makeDLUR(uint64_t piece) {
    uint64_t star = 0;
    uint64_t pieceTemp = 0;

    // top right
    pieceTemp = piece & ~U_R_BORDER;
    if (pieceTemp != 0) {
        while (((pieceTemp & U_R_BORDER) == 0)) {
            pieceTemp = pieceTemp << 7u;
            star |= pieceTemp;
            if (pieceTemp & U_R_BORDER) {
                break;
            }
        }
    }

    // bottom left
    pieceTemp = piece & ~D_L_BORDER;
    if (pieceTemp != 0) {
        while (((pieceTemp & D_L_BORDER) == 0)) {
            pieceTemp = pieceTemp >> 7u;
            star |= pieceTemp;
            if (pieceTemp & D_L_BORDER) {
                break;
            }
        }
    }

    return star;
}

uint64_t makeULDR(uint64_t piece) {
    uint64_t star = 0;
    uint64_t pieceTemp = 0;

    // top left
    pieceTemp = piece & ~U_L_BORDER;
    if (pieceTemp != 0) {
        while (((pieceTemp & U_L_BORDER) == 0)) {
            pieceTemp = pieceTemp << 9u;
            star |= pieceTemp;
            if (pieceTemp & U_L_BORDER) {
                break;
            }
        }
    }

    // bottom right
    pieceTemp = piece & ~D_R_BORDER;
    if (pieceTemp != 0) {
        while (((pieceTemp & D_R_BORDER) == 0)) {
            pieceTemp = pieceTemp >> 9u;
            star |= pieceTemp;
            if (pieceTemp & D_R_BORDER) {
                break;
            }
        }
    }

    return star;
}
