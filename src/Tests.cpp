//
// Created by louis on 6/16/19.
//

#include "MoveUtils.h"
#include "Move.h"
#include "engines/EngineMinimaxBetter.h"
#include "Perft.h"
#include "evaluators/EvalBasicHeuristics.h"
#include "engines/EngineMinimax.h"
#include "engines/EngineRandom.h"
#include "Board.h"
#include "Art.h"
#include "main.h"
#include <algorithm>
#include <bitset>
#include <sstream>
#include <iostream>
#include "Tests.h"

void speedTests() {

    auto *board = new Board;
    unsigned long p2 = Perft::perft(board, 12);
    if (p2 != 1939886636) {
        throw std::runtime_error(
                "Perft* failed on depth 11, expected " + std::__cxx11::to_string(1939886636) + ", got " +
                std::__cxx11::to_string(p2));
    }

    delete board;
}

void tests() {
    // thanks to http://www.aartbik.com/MISC/reversi.html
    auto *board = new Board;
    unsigned long p1 = Perft::perft(board, 1);
    if (p1 != 4) {
        throw std::runtime_error("Perft failed on depth 1, expected " + std::__cxx11::to_string(4) + ", got " +
                                 std::__cxx11::to_string(p1));
    }

    unsigned long p2 = Perft::perft(board, 2);
    if (p2 != 12) {
        throw std::runtime_error("Perft failed on depth 2, expected " + std::__cxx11::to_string(12) + ", got " +
                                 std::__cxx11::to_string(p2));
    }


    unsigned long p3 = Perft::splitPerft(board, 3);
//    unsigned long p3 = Perft::perftP(board, 3);
    if (p3 != 56) {
        throw std::runtime_error("Perft failed on depth 3, expected " + std::__cxx11::to_string(56) + ", got " +
                                 std::__cxx11::to_string(p3));
    }

    unsigned long p4 = Perft::perft(board, 4);
    if (p4 != 244) {
        throw std::runtime_error("Perft failed on depth 4, expected " + std::__cxx11::to_string(244) + ", got " +
                                 std::__cxx11::to_string(p4));
    }

    unsigned long p5 = Perft::perft(board, 5);
    if (p5 != 1396) {
        throw std::runtime_error("Perft failed on depth 5, expected " + std::__cxx11::to_string(1396) + ", got " +
                                 std::__cxx11::to_string(p5));
    }

    unsigned long p6 = Perft::perft(board, 6);
    if (p6 != 8200) {
        throw std::runtime_error("Perft failed on depth 6, expected " + std::__cxx11::to_string(8200) + ", got " +
                                 std::__cxx11::to_string(p6));
    }

//    unsigned long p7 = Perft::perftP(board, 7);
    unsigned long p7 = Perft::splitPerft(board, 7);
    if (p7 != 55092) {
        throw std::runtime_error("Perft failed on depth 7, expected " + std::__cxx11::to_string(55092) + ", got " +
                                 std::__cxx11::to_string(p7));
    }

//    unsigned long p8 = Perft::perftP(board, 8);
    unsigned long p8 = Perft::splitPerft(board, 8);
    if (p8 != 390216) {
        throw std::runtime_error("Perft failed on depth 8, expected " + std::__cxx11::to_string(390216) + ", got " +
                                 std::__cxx11::to_string(p8));
    }

    unsigned long p9 = Perft::perft(board, 9);
    if (p9 != 3005288) {
        throw std::runtime_error("Perft failed on depth 9, expected " + std::__cxx11::to_string(3005288) + ", got " +
                                 std::__cxx11::to_string(p9));
    }


    unsigned long p10 = Perft::perft(board, 10);
    if (p10 != 24571284) {
        throw std::runtime_error("Perft failed on depth 10, expected " + std::__cxx11::to_string(24571284) + ", got " +
                                 std::__cxx11::to_string(p10));
    }

//    unsigned long p11 = Perft::perftP(board, 11);
//    if (p11 != 212258800) {
//        throw runtime_error("Perft failed on depth 11, expected " + to_string(212258800) + ", got " + to_string(p11));
//    }

//    unsigned long p12 = Perft::perftP(board, 12);
//    if (p12 != 1939886636) {
//        throw runtime_error("Perft failed on depth 11, expected " + to_string(1939886636) + ", got " + to_string(p12));
//    }

    delete board;
}
