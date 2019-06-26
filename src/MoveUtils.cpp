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
uint64_t getMoveCapturesHelper(uint64_t piece, uint64_t friends, uint64_t enemies);

struct Permutation {
    uint64_t emptiesTreatAsDead = 0;
    uint64_t enemiesTreatAsEmpties = 0;
    uint64_t friendsTreatAsOcc = 0;
};

Permutation *allPermutationsFromLongs(Permutation *permutations, uint64_t *pieces, uint64_t size);

bool moveDatabaseReady = false;

void setupAttacksDatabase() {
    setupStarMask();
    if (moveDatabaseReady) {
        return;
    }

    uint64_t longsInStar[8];
    for (int x = 0; x < 8; x++) {
        uint64_t piece = newPieceOnSquare(x);
        uint64_t star = starLR[x];
        allLongInStar(longsInStar, star);
        const uint64_t size = pow(3ul, longsInStar[0]);

        Permutation permutations[size];
        allPermutationsFromLongs(permutations, longsInStar, size);
        for (int p = 0; p < size; p++) {
            Permutation &perm = permutations[p];
            uint64_t moves = getMoveCapturesHelper(piece, perm.friendsTreatAsOcc,
                                                        perm.enemiesTreatAsEmpties);

            uint64_t i = flipAntiDiagonal(moves);
            aFileAttacks[x][perm.friendsTreatAsOcc][perm.enemiesTreatAsEmpties] |= i;
            for (int y = 0; y < 8; y++) {
                uint64_t shiftedMoves = moves << (8u * y);
                fillUpAttacks[x][perm.friendsTreatAsOcc][perm.enemiesTreatAsEmpties] |= shiftedMoves;
            }
        }
    }

    moveDatabaseReady = true;
}


const uint64_t k1 = 0x5500550055005500;
const uint64_t k2 = 0x3333000033330000;
const uint64_t k4 = 0x0f0f0f0f00000000;

uint64_t flipAntiDiagonal(uint64_t x) {
    uint64_t t;
    t = k4 & (x ^ (x << 28u));
    x ^= t ^ (t >> 28u);
    t = k2 & (x ^ (x << 14u));
    x ^= t ^ (t >> 14u);
    t = k1 & (x ^ (x << 7u));
    x ^= t ^ (t >> 7u);
    return x;
}

const uint64_t kk1 = 0xaa00aa00aa00aa00;
const uint64_t kk2 = 0xcccc0000cccc0000;
const uint64_t kk4 = 0xf0f0f0f00f0f0f0f;

uint64_t flipDiagonal(uint64_t x) {
    uint64_t t;
    t = x ^ (x << 36u);
    x ^= kk4 & (t ^ (x >> 36u));
    t = kk2 & (x ^ (x << 18u));
    x ^= t ^ (t >> 18u);
    t = kk1 & (x ^ (x << 9u));
    x ^= t ^ (t >> 9u);
    return x;
}

uint64_t *allLongInStar(uint64_t *indexes, uint64_t star) {
    int index = 0;
    while (star) {
        uint64_t p = star & -star;
        indexes[++index] = p;
        star &= star - 1;
    }
    indexes[0] = index; // to store size;

    return indexes;
}

Permutation *allPermutationsFromLongs(Permutation *permutations, uint64_t *pieces, uint64_t size) {
    // we must consider three states:
    // square can be empty, friend, enemy

    for (uint32_t tt = 0; tt < size; tt++) {
        Permutation p = {};
        uint32_t t = tt;

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

uint64_t fillUpAttacks[8][256][256];

uint64_t aFileAttacks[8][256][256];

uint64_t diagonalAttacks(uint32_t index, uint64_t friends, uint64_t enemies) {
    uint32_t f = index & 7u;
    uint64_t d = DIAGONALS[index];
    friends = (d & friends) * H_FILE >> 56u;
    enemies = (d & enemies) * H_FILE >> 56u;
    return d & fillUpAttacks[f][friends][enemies];
}

uint64_t antiDiagAttacks(uint32_t index, uint64_t friends, uint64_t enemies) {
    uint32_t f = index & 7u;
    uint64_t d = ANTI_DIAGONALS[index];
    uint64_t i = d & friends;
    uint64_t i1 = i * H_FILE;
    friends = i1 >> 56u;
    enemies = (d & enemies) * H_FILE >> 56u;
    return d & fillUpAttacks[f][friends][enemies];
}

uint64_t rankAttacks(uint32_t index, uint64_t friends, uint64_t enemies) {
    uint32_t f = index & 7u;
    uint64_t r = RANKS[index];
    uint64_t i1 = r & friends;
    uint64_t i2 = i1 * H_FILE;
    friends = i2 >> 56u;
    enemies = (r & enemies) * H_FILE >> 56u;
    uint64_t i = r & fillUpAttacks[f][friends][enemies];
    return i;
}


const uint64_t hFile = 0x0101010101010101;
const uint64_t diaga1g8 = 0x0102040810204080ULL;

uint64_t fileAttacks(uint32_t index, uint64_t friends, uint64_t enemies) {
    uint32_t f = index & 7u;
    friends = hFile & (friends >> f);
    uint64_t i2 = diaga1g8 * friends;
    friends = i2 >> 56u;
    enemies = hFile & (enemies >> f);
    enemies = (diaga1g8 * enemies) >> 56u;
    uint64_t i = aFileAttacks[index >> 3u][friends][enemies];
    uint64_t i1 = i << f;
    return i1;
}


uint64_t getMoveCaptures(uint64_t index, uint64_t friends, uint64_t enemies) {
    uint64_t answers = 0;
    const uint32_t f = index & 7u;

    uint64_t friendsTemp = friends;
    uint64_t enemiesTemp = enemies;

    // file
    friendsTemp = hFile & (friendsTemp >> f);
    friendsTemp = (diaga1g8 * friendsTemp) >> 56u;
    enemiesTemp = hFile & (enemiesTemp >> f);
    enemiesTemp = (diaga1g8 * enemiesTemp) >> 56u;
    answers |= (aFileAttacks[index >> 3u][friendsTemp][enemiesTemp]) << f;

    // rank
    friendsTemp = friends;
    enemiesTemp = enemies;

    const uint64_t r = RANKS[index];
    friendsTemp = ((r & friendsTemp) * H_FILE) >> 56u;
    enemiesTemp = (r & enemiesTemp) * H_FILE >> 56u;
    answers |= r & fillUpAttacks[f][friendsTemp][enemiesTemp];

    // anti diag
    friendsTemp = friends;
    enemiesTemp = enemies;

    const uint64_t ad = ANTI_DIAGONALS[index];
    friendsTemp = ((ad & friendsTemp) * H_FILE) >> 56u;
    enemiesTemp = (ad & enemiesTemp) * H_FILE >> 56u;
    answers |= ad & fillUpAttacks[f][friendsTemp][enemiesTemp];

    // diag
    friendsTemp = friends;
    enemiesTemp = enemies;

    const uint64_t d = DIAGONALS[index];
    friendsTemp = (d & friendsTemp) * H_FILE >> 56u;
    enemiesTemp = (d & enemiesTemp) * H_FILE >> 56u;
    answers |= d & fillUpAttacks[f][friendsTemp][enemiesTemp];

    return answers;
}

uint64_t getMoveCapturesHelper(uint64_t piece, uint64_t friends, uint64_t enemies) {
    uint64_t empties = ~(friends | enemies);

    uint64_t captures = 0;

    //////////////////////////////////
    /// Rook Style
    //////////////////////////////////

    uint64_t pieceTemp = 0;
    uint64_t capTemp = 0;
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
