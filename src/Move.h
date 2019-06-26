//
// Created by louis on 6/10/19.
//

#ifndef OTHELLOPROJECT_MOVE_H
#define OTHELLOPROJECT_MOVE_H


#include <string>
#include <vector>

const uint64_t PASS_MOVE = 0xffffffffffffffff;

uint64_t getMoveForPiece(uint64_t piece, uint64_t friends, uint64_t enemies);

void getMovesAsArray(uint64_t *array, uint64_t moves);

std::vector<std::string> getMovesAsArrayString(uint64_t moves);

#endif //OTHELLOPROJECT_MOVE_H
