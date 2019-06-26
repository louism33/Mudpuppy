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
    std::string name;

    virtual int getDisplayScoreOfMove(Board &board) = 0;

    virtual uint32_t getBestMoveInt(Board &board) = 0;

    virtual uint64_t getBestMove(Board &board) = 0;
};

#endif //OTHELLOPROJECT_ENGINEBASE_H
