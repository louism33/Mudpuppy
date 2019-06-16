//
// Created by louis on 6/8/19.
//

#ifndef OTHELLOPROJECT_ENGINEBASE_H
#define OTHELLOPROJECT_ENGINEBASE_H

#include <iostream>
#include <bitset>
#include <sstream>
#include "../main.h"
#include "../Art.h"
#include "../Board.h"
#include "EngineBase.h"

class EngineBase{
public:
    std::string *name;

    virtual unsigned int getDisplayScoreOfMove(Board &board) = 0;

    virtual unsigned int getBestMoveInt(Board &board) = 0;

    virtual unsigned long getBestMove(Board &board) = 0;
};

#endif //OTHELLOPROJECT_ENGINEBASE_H
