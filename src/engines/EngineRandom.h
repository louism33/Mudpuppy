//
// Created by louis on 6/5/19.
//

#ifndef OTHELLOPROJECT_ENGINERANDOM_H
#define OTHELLOPROJECT_ENGINERANDOM_H





//
// Created by louis on 6/5/19.
//

#include <bitset>
#include "../main.h"
#include <sstream>
#include <iostream>
#include "../Art.h"
#include "../Board.h"
#include "EngineBase.h"

class EngineRandom: public EngineBase{
public:

    EngineRandom(std::string name);

    unsigned long getBestMove(Board &board);

    unsigned int getBestMoveInt(Board &board);

    unsigned int getDisplayScoreOfMove(Board &board);
};


#endif //OTHELLOPROJECT_ENGINERANDOM_H
