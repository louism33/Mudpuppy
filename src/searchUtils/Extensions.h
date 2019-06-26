//
// Created by louis on 6/13/19.
//

#ifndef OTHELLOPROJECT_EXTENSIONS_H
#define OTHELLOPROJECT_EXTENSIONS_H

#include <bitset>
#include <sstream>
#include <iostream>
#include "../main.h"
#include "../Art.h"
#include "../Board.h"


uint32_t getExtension(Board &board, uint64_t moves, int ply, bool extended);
uint32_t getSafeExtension(Board &board, uint64_t moves, int ply);

#endif //OTHELLOPROJECT_EXTENSIONS_H
