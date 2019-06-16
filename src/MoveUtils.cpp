//
// Created by louis on 6/14/19.
//
#include <bitset>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <math.h>
#include <jmorecfg.h>
#include "Art.h"
#include "main.h"
#include "Board.h"
#include "BitBoardUtils.h"
#include "MoveUtils.h"

using namespace std;
unsigned long getMoveCapturesHelper(unsigned long piece, unsigned long friends, unsigned long enemies);

struct Permutation {
    unsigned long emptiesTreatAsDead = 0;
    unsigned long enemiesTreatAsEmpties = 0;
    unsigned long friendsTreatAsOcc = 0;
};

Permutation *allPermutationsFromLongs(Permutation *permutations, unsigned long *pieces, unsigned long size);

bool moveDatabaseReady = false;

void setupAttacksDatabase() {
    setupStarMask();
    if (moveDatabaseReady) {
        return;
    }

    unsigned long longsInStar[8];
    for (int x = 0; x < 8; x++) {
        unsigned long piece = newPieceOnSquare(x);
        unsigned long star = starLR[x];
        allLongInStar(longsInStar, star);
        const unsigned long size = pow(3ul, longsInStar[0]);

        Permutation permutations[size];
        allPermutationsFromLongs(permutations, longsInStar, size);
        for (int p = 0; p < size; p++) {
            Permutation &perm = permutations[p];
            unsigned long moves = getMoveCapturesHelper(piece, perm.friendsTreatAsOcc,
                                                        perm.enemiesTreatAsEmpties);

            unsigned long i = flipAntiDiagonal(moves);
            aFileAttacks[x][perm.friendsTreatAsOcc][perm.enemiesTreatAsEmpties] |= i;
            for (int y = 0; y < 8; y++) {
                unsigned long shiftedMoves = moves << (8u * y);
                fillUpAttacks[x][perm.friendsTreatAsOcc][perm.enemiesTreatAsEmpties] |= shiftedMoves;
            }
        }
    }

    moveDatabaseReady = true;
}


const unsigned long k1 = 0x5500550055005500;
const unsigned long k2 = 0x3333000033330000;
const unsigned long k4 = 0x0f0f0f0f00000000;

unsigned long flipAntiDiagonal(unsigned long x) {
    unsigned long t;
    t = k4 & (x ^ (x << 28u));
    x ^= t ^ (t >> 28u);
    t = k2 & (x ^ (x << 14u));
    x ^= t ^ (t >> 14u);
    t = k1 & (x ^ (x << 7u));
    x ^= t ^ (t >> 7u);
    return x;
}

const unsigned long kk1 = 0xaa00aa00aa00aa00;
const unsigned long kk2 = 0xcccc0000cccc0000;
const unsigned long kk4 = 0xf0f0f0f00f0f0f0f;

unsigned long flipDiagonal(unsigned long x) {
    unsigned long t;
    t = x ^ (x << 36u);
    x ^= kk4 & (t ^ (x >> 36u));
    t = kk2 & (x ^ (x << 18u));
    x ^= t ^ (t >> 18u);
    t = kk1 & (x ^ (x << 9u));
    x ^= t ^ (t >> 9u);
    return x;
}

unsigned long *allLongInStar(unsigned long *indexes, unsigned long star) {
    int index = 0;
    while (star) {
        unsigned long p = star & -star;
        indexes[++index] = p;
        star &= star - 1;
    }
    indexes[0] = index; // to store size;

    return indexes;
}

Permutation *allPermutationsFromLongs(Permutation *permutations, unsigned long *pieces, unsigned long size) {
    // we must consider three states:
    // square can be empty, friend, enemy

    for (unsigned int tt = 0; tt < size; tt++) {
        Permutation p = {};
        unsigned int t = tt;

        if ((t % 3) == 0)
            p.emptiesTreatAsDead |= pieces[1];
        if ((t % 3) == 1)
            p.friendsTreatAsOcc |= pieces[1];
        if ((t % 3) == 2)
            p.enemiesTreatAsEmpties |= pieces[1];

        t = t / 3;

        if ((t % 3) == 0)
            p.emptiesTreatAsDead |= pieces[2];
        if ((t % 3) == 1)
            p.friendsTreatAsOcc |= pieces[2];
        if ((t % 3) == 2)
            p.enemiesTreatAsEmpties |= pieces[2];

        t = t / 3;

        if ((t % 3) == 0)
            p.emptiesTreatAsDead |= pieces[3];
        if ((t % 3) == 1)
            p.friendsTreatAsOcc |= pieces[3];
        if ((t % 3) == 2)
            p.enemiesTreatAsEmpties |= pieces[3];

        t = t / 3;

        if ((t % 3) == 0)
            p.emptiesTreatAsDead |= pieces[4];
        if ((t % 3) == 1)
            p.friendsTreatAsOcc |= pieces[4];
        if ((t % 3) == 2)
            p.enemiesTreatAsEmpties |= pieces[4];

        t = t / 3;

        if ((t % 3) == 0)
            p.emptiesTreatAsDead |= pieces[5];
        if ((t % 3) == 1)
            p.friendsTreatAsOcc |= pieces[5];
        if ((t % 3) == 2)
            p.enemiesTreatAsEmpties |= pieces[5];

        t = t / 3;

        if ((t % 3) == 0)
            p.emptiesTreatAsDead |= pieces[6];
        if ((t % 3) == 1)
            p.friendsTreatAsOcc |= pieces[6];
        if ((t % 3) == 2)
            p.enemiesTreatAsEmpties |= pieces[6];

        t = t / 3;

        if ((t % 3) == 0) {
            p.emptiesTreatAsDead |= pieces[7];
        }
        if ((t % 3) == 1) {
            p.friendsTreatAsOcc |= pieces[7];
        }
        if ((t % 3) == 2) {
            p.enemiesTreatAsEmpties |= pieces[7];
        }

        t = t / 3;

        if (t != 0) {
            throw std::runtime_error("error in magic gen");
        }

        permutations[tt] = p;

        assert((p.enemiesTreatAsEmpties & p.friendsTreatAsOcc) == 0);
        bool b = (p.emptiesTreatAsDead & p.friendsTreatAsOcc) == 0;
        assert(b);
        assert((p.emptiesTreatAsDead & p.enemiesTreatAsEmpties) == 0);
    }

    return permutations;
}

unsigned long fillUpAttacks[8][256][256];

unsigned long aFileAttacks[8][256][256];

unsigned long diagonalAttacks(unsigned int index, unsigned long friends, unsigned long enemies) {
    unsigned int f = index & 7u;
    unsigned long d = DIAGONALS[index];
    friends = (d & friends) * H_FILE >> 56u;
    enemies = (d & enemies) * H_FILE >> 56u;
    return d & fillUpAttacks[f][friends][enemies];
}

unsigned long antiDiagAttacks(unsigned int index, unsigned long friends, unsigned long enemies) {
    unsigned int f = index & 7u;
    unsigned long d = ANTI_DIAGONALS[index];
    unsigned long i = d & friends;
    unsigned long i1 = i * H_FILE;
    friends = i1 >> 56u;
    enemies = (d & enemies) * H_FILE >> 56u;
    return d & fillUpAttacks[f][friends][enemies];
}

unsigned long rankAttacks(unsigned int index, unsigned long friends, unsigned long enemies) {
    unsigned int f = index & 7u;
    unsigned long r = RANKS[index];
    unsigned long i1 = r & friends;
    unsigned long i2 = i1 * H_FILE;
    friends = i2 >> 56u;
    enemies = (r & enemies) * H_FILE >> 56u;
    unsigned long i = r & fillUpAttacks[f][friends][enemies];
    return i;
}


const unsigned long hFile = 0x0101010101010101;
const unsigned long diaga1g8 = 0x0102040810204080ULL;

unsigned long fileAttacks(unsigned int index, unsigned long friends, unsigned long enemies) {
    unsigned int f = index & 7u;
    friends = hFile & (friends >> f);
    unsigned long i2 = diaga1g8 * friends;
    friends = i2 >> 56u;
    enemies = hFile & (enemies >> f);
    enemies = (diaga1g8 * enemies) >> 56u;
    unsigned long i = aFileAttacks[index >> 3u][friends][enemies];
    unsigned long i1 = i << f;
    return i1;
}


unsigned long getMoveCaptures(unsigned long index, unsigned long friends, unsigned long enemies) {
    unsigned long answers = 0;
    const unsigned int f = index & 7u;

    unsigned long friendsTemp = friends;
    unsigned long enemiesTemp = enemies;

    // file
    friendsTemp = hFile & (friendsTemp >> f);
    friendsTemp = (diaga1g8 * friendsTemp) >> 56u;
    enemiesTemp = hFile & (enemiesTemp >> f);
    enemiesTemp = (diaga1g8 * enemiesTemp) >> 56u;
    answers |= (aFileAttacks[index >> 3u][friendsTemp][enemiesTemp]) << f;

    // rank
    friendsTemp = friends;
    enemiesTemp = enemies;

    const unsigned long r = RANKS[index];
    friendsTemp = ((r & friendsTemp) * H_FILE) >> 56u;
    enemiesTemp = (r & enemiesTemp) * H_FILE >> 56u;
    answers |= r & fillUpAttacks[f][friendsTemp][enemiesTemp];

    // anti diag
    friendsTemp = friends;
    enemiesTemp = enemies;

    const unsigned long ad = ANTI_DIAGONALS[index];
    friendsTemp = ((ad & friendsTemp) * H_FILE) >> 56u;
    enemiesTemp = (ad & enemiesTemp) * H_FILE >> 56u;
    answers |= ad & fillUpAttacks[f][friendsTemp][enemiesTemp];

    // diag
    friendsTemp = friends;
    enemiesTemp = enemies;

    const unsigned long d = DIAGONALS[index];
    friendsTemp = (d & friendsTemp) * H_FILE >> 56u;
    enemiesTemp = (d & enemiesTemp) * H_FILE >> 56u;
    answers |= d & fillUpAttacks[f][friendsTemp][enemiesTemp];

    return answers;
}

unsigned long getMoveCapturesHelper(unsigned long piece, unsigned long friends, unsigned long enemies) {
    unsigned long empties = ~(friends | enemies);

    unsigned long captures = 0;

    //////////////////////////////////
    /// Rook Style
    //////////////////////////////////

    unsigned long pieceTemp = 0;
    unsigned long capTemp = 0;
    pieceTemp = piece & ~L_BORDER;
    if (((pieceTemp = pieceTemp << 1u) & enemies) != 0) {
        capTemp |= pieceTemp;
        while (((pieceTemp & L_BORDER) == 0)) {
            pieceTemp = pieceTemp << 1u;
            if (pieceTemp & friends) {
                captures |= capTemp;
                break;
            }

            if (pieceTemp & enemies) {
                capTemp |= pieceTemp;
            }

            if (pieceTemp & empties) {
                break;
            }

            if (pieceTemp & L_BORDER) {
                break;
            }
        }
    }

    pieceTemp = piece & ~U_BORDER;
    capTemp = 0;
    if (((pieceTemp = pieceTemp << 8u) & enemies) != 0) {
        capTemp |= pieceTemp;
        while (((pieceTemp & U_BORDER) == 0)) {
            pieceTemp = pieceTemp << 8u;
            if (pieceTemp & friends) {
                captures |= capTemp;
                break;
            }

            if (pieceTemp & enemies) {
                capTemp |= pieceTemp;
            }

            if (pieceTemp & empties) {
                break;
            }

            if (pieceTemp & U_BORDER) {
                break;
            }
        }
    }

    pieceTemp = piece & ~R_BORDER;
    capTemp = 0;
    if (((pieceTemp = pieceTemp >> 1u) & enemies) != 0) {
        capTemp |= pieceTemp;
        while (((pieceTemp & R_BORDER) == 0)) {
            pieceTemp = pieceTemp >> 1u;
            if (pieceTemp & friends) {
                captures |= capTemp;
                break;
            }

            if (pieceTemp & enemies) {
                capTemp |= pieceTemp;
            }

            if (pieceTemp & empties) {
                break;
            }

            if (pieceTemp & R_BORDER) {
                break;
            }
        }
    }

    pieceTemp = piece & ~D_BORDER;
    capTemp = 0;
    if (((pieceTemp = pieceTemp >> 8u) & enemies) != 0) {
        capTemp |= pieceTemp;
        while (((pieceTemp & D_BORDER) == 0)) {
            pieceTemp = pieceTemp >> 8u;
            if (pieceTemp & friends) {
                captures |= capTemp;
                break;
            }

            if (pieceTemp & enemies) {
                capTemp |= pieceTemp;
            }

            if (pieceTemp & empties) {
                break;
            }

            if (pieceTemp & D_BORDER) {
                break;
            }
        }
    }

    //////////////////////////////////
    /// Bishop style
    //////////////////////////////////

    // top right
    pieceTemp = piece & ~U_R_BORDER;
    capTemp = 0;
    if (((pieceTemp = pieceTemp << 7u) & enemies) != 0) {
        capTemp |= pieceTemp;
        while (((pieceTemp & U_R_BORDER) == 0)) {
            pieceTemp = pieceTemp << 7u;
            if (pieceTemp & friends) {
                captures |= capTemp;
                break;
            }

            if (pieceTemp & enemies) {
                capTemp |= pieceTemp;
            }

            if (pieceTemp & empties) {
                break;
            }

            if (pieceTemp & U_R_BORDER) {
                break;
            }
        }
    }

    // top left
    pieceTemp = piece & ~U_L_BORDER;
    capTemp = 0;
    if (((pieceTemp = pieceTemp << 9u) & enemies) != 0) {
        capTemp |= pieceTemp;
        while (((pieceTemp & U_L_BORDER) == 0)) {
            pieceTemp = pieceTemp << 9u;
            if (pieceTemp & friends) {
                captures |= capTemp;
                break;
            }

            if (pieceTemp & enemies) {
                capTemp |= pieceTemp;
            }

            if (pieceTemp & empties) {
                break;
            }

            if (pieceTemp & U_L_BORDER) {
                break;
            }
        }
    }

    // bottom right
    pieceTemp = piece & ~D_R_BORDER;
    capTemp = 0;
    if (((pieceTemp = pieceTemp >> 9u) & enemies) != 0) {
        capTemp |= pieceTemp;
        while (((pieceTemp & D_R_BORDER) == 0)) {
            pieceTemp = pieceTemp >> 9u;
            if (pieceTemp & friends) {
                captures |= capTemp;
                break;
            }

            if (pieceTemp & enemies) {
                capTemp |= pieceTemp;
            }

            if (pieceTemp & empties) {
                break;
            }

            if (pieceTemp & D_R_BORDER) {
                break;
            }
        }
    }

    // bottom left
    pieceTemp = piece & ~D_L_BORDER;
    capTemp = 0;
    if (((pieceTemp = pieceTemp >> 7u) & enemies) != 0) {
        capTemp |= pieceTemp;
        while (((pieceTemp & D_L_BORDER) == 0)) {
            pieceTemp = pieceTemp >> 7u;
            if (pieceTemp & friends) {
                captures |= capTemp;
                break;
            }

            if (pieceTemp & enemies) {
                capTemp |= pieceTemp;
            }

            if (pieceTemp & empties) {
                break;
            }

            if (pieceTemp & D_L_BORDER) {
                break;
            }
        }
    }

    return captures;
}
