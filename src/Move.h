//
// Created by louis on 6/10/19.
//

#ifndef OTHELLOPROJECT_MOVE_H
#define OTHELLOPROJECT_MOVE_H


#include <string>
#include <vector>

unsigned long getMoveForPiece(unsigned long piece, unsigned long friends, unsigned long enemies);

std::vector<long> getMovesAsArray(unsigned long moves);

std::vector<std::string> getMovesAsArrayString(unsigned long moves);

#endif //OTHELLOPROJECT_MOVE_H
