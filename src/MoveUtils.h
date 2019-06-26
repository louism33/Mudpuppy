//
// Created by louis on 6/14/19.
//

#ifndef OTHELLOPROJECT_MOVEUTILS_H
#define OTHELLOPROJECT_MOVEUTILS_H

#include <cstdint>

extern uint64_t fillUpAttacks[8][256][256];

extern uint64_t aFileAttacks[8][256][256];

void setupAttacksDatabase();

uint64_t flipAntiDiagonal(uint64_t x);

uint64_t flipDiagonal(uint64_t x);

uint64_t diagonalAttacks(uint32_t index, uint64_t friends, uint64_t enemies);

uint64_t antiDiagAttacks(uint32_t index, uint64_t friends, uint64_t enemies);

uint64_t rankAttacks(uint32_t index, uint64_t friends, uint64_t enemies);

uint64_t fileAttacks(uint32_t index, uint64_t friends, uint64_t enemies);

uint64_t getMoveCaptures(uint64_t piece, uint64_t friends, uint64_t enemies);

uint64_t *allLongInStar(uint64_t *indexes, uint64_t star);

#endif //OTHELLOPROJECT_MOVEUTILS_H
