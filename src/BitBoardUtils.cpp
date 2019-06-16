//
// Created by louis on 6/6/19.
//

//static unsigned long starMask[64];

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
unsigned long starMask[64];
unsigned long starMaskRook[64];
unsigned long starMaskBishop[64];
unsigned long starLR[64];
unsigned long starUD[64];
unsigned long starDLUR[64];
unsigned long starULDR[64];

void setupStarMask() {
    if (ready) {
        return;
    }
    for (int i = 0; i < 64; i++) {
        unsigned long piece = newPieceOnSquare(i);
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
unsigned long makeLR(unsigned long piece) {
    unsigned long star = 0;

    unsigned long pieceTemp = piece & ~L_BORDER;
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

unsigned long makeUD(unsigned long piece) {
    unsigned long star = 0;

    unsigned long pieceTemp = 0;

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



unsigned long makeDLUR(unsigned long piece) {
    unsigned long star = 0;
    unsigned long pieceTemp = 0;

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

unsigned long makeULDR(unsigned long piece) {
    unsigned long star = 0;
    unsigned long pieceTemp = 0;

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
