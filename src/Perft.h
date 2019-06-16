//
// Created by louis on 6/9/19.
//

#ifndef OTHELLOPROJECT_PERFT_H
#define OTHELLOPROJECT_PERFT_H

#include <bitset>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <math.h>
#include <jmorecfg.h>
#include "Art.h"
#include "main.h"
#include "Board.h"
#include "BitBoardUtils.h"
#include "Board.h"

class Perft {

public:
    static unsigned long getNps(unsigned long nodes, long time);

    static unsigned long perft(Board *board, int depth, bool passed);

    static unsigned long perft(Board *board, int depth);

    static unsigned long splitPerft(Board *board, int depth);
};


#endif //OTHELLOPROJECT_PERFT_H
