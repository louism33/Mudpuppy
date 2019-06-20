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


unsigned int getExtension(Board &board, unsigned long moves, int ply, bool extended);
unsigned int getSafeExtension(Board &board, unsigned long moves, int ply);

#endif //OTHELLOPROJECT_EXTENSIONS_H
