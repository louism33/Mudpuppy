//
// Created by louis on 6/10/19.
//

#ifndef OTHELLOPROJECT_MOVE_H
#define OTHELLOPROJECT_MOVE_H


#include <string>
#include <vector>

const unsigned long PASS_MOVE = 0xffffffffffffffff;

unsigned long getMoveForPiece(unsigned long piece, unsigned long friends, unsigned long enemies);

void getMovesAsArray(unsigned long *array, unsigned long moves);

std::vector<std::string> getMovesAsArrayString(unsigned long moves);

#endif //OTHELLOPROJECT_MOVE_H
