//
// Created by louis on 6/5/19.
//

#ifndef OTHELLOPROJECT_ART_H
#define OTHELLOPROJECT_ART_H

#include <bitset>
#include "main.h"
#include <sstream>
#include <iostream>
#include "Art.h"
#include "Board.h"

void printLong(long l);

void printBoardHelper(long white, long black, int turn);

void printBoard(Board &board);
void printBoardWithIndex(Board &board);
void printBoardWithIndexAndLegalMoves(Board &board);
void printBoardWithIndexAndLegalMovesColour(Board &board);

std::__cxx11::string getNiceBoard(long white, long black, int turn);
std::__cxx11::string getNiceBoardWithIndex(long white, long black, int turn);
std::__cxx11::string getNiceBoardWithIndexAndLegalMoves(Board &board, long white, long black, int turn);
std::__cxx11::string getNiceBoardWithIndexAndLegalMovesColour(Board &board, long white, long black, int turn);

std::__cxx11::string getNiceBoard(long white, long black, int turn);


#endif //OTHELLOPROJECT_ART_H
