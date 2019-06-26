//
// Created by louis on 6/7/19.
//

#include <bitset>
#include <bits/stdc++.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include "../main.h"
#include "../Art.h"
#include "../Board.h"
#include "../engines/EngineMinimaxV1.h"
#include "EvalSimpleCount.h"


int EvalSimpleCount::eval(const Board &board, uint64_t moves) {
    const int num = popCount(board.whitePieces) - popCount(board.blackPieces);
    return board.turn == WHITE ? num : -num;
}