//
// Created by louis on 6/14/19.
//

#ifndef OTHELLOPROJECT_MOVEUTILS_H
#define OTHELLOPROJECT_MOVEUTILS_H

extern unsigned long fillUpAttacks[8][256][256];

extern unsigned long aFileAttacks[8][256][256];

void setupAttacksDatabase();

unsigned long flipAntiDiagonal(unsigned long x);

unsigned long flipDiagonal(unsigned long x);

unsigned long diagonalAttacks(unsigned int index, unsigned long friends, unsigned long enemies);

unsigned long antiDiagAttacks(unsigned int index, unsigned long friends, unsigned long enemies);

unsigned long rankAttacks(unsigned int index, unsigned long friends, unsigned long enemies);

unsigned long fileAttacks(unsigned int index, unsigned long friends, unsigned long enemies);

unsigned long getMoveCaptures(unsigned long piece, unsigned long friends, unsigned long enemies);

unsigned long *allLongInStar(unsigned long *indexes, unsigned long star);

#endif //OTHELLOPROJECT_MOVEUTILS_H
