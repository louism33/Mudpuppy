//
// Created by louis on 6/5/19.
//

#include <bitset>
#include <bits/stdc++.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include "main.h"
#include "Art.h"
#include "Board.h"

std::__cxx11::string getNiceLong(long l);

char whiteSymbol = 'o';
char blackSymbol = 'x';
char moveSymbol = '.';


// red vs blue, no background
//std::string whiteSymbolColour = "\x1B[31m o \033[0m";
//std::string blackSymbolColour = "\x1B[36m x \033[0m";

// green vs red background lite
static std::string whiteSymbolColour = "\x1B[1;42m   \033[0m";
static std::string blackSymbolColour = "\x1B[1;41m   \033[0m";

// white vs black background
//std::string whiteSymbolColour = "\x1B[0;47m   \033[0m";
//std::string blackSymbolColour = "\x1B[0;107m   \033[0m";

static std::string moveSymbolColour = " . ";

using namespace std;

void printBoard(long white, long black, int turn);

void printLong(long l) {
    std::cout << getNiceLong(l) << std::endl;
    std::cout << "-----" << std::endl;
}

std::__cxx11::string getNiceLong(long l) {
    std::string out_string;
    std::string ss;

    std::__cxx11::string s = std::bitset<64>(l).to_string();

    for (int y = 0; y < 64; ++y) {
        ss += s[y];
        if (y % 8 == 7) {
            ss += "\n";
        }
    }
    return ss;
}


void printBoard(Board &board) {
    std::cout << getNiceBoard(board.whitePieces, board.blackPieces, board.turn) << std::endl;
    std::cout << "-----" << std::endl;
}

void printBoardWithIndex(Board &board) {
    std::cout << getNiceBoardWithIndex(board.whitePieces, board.blackPieces, board.turn) << std::endl;
    std::cout << "-----" << std::endl;
}

void printBoardWithIndexAndLegalMovesColour(Board &board) {
    std::cout << getNiceBoardWithIndexAndLegalMovesColour(board, board.whitePieces, board.blackPieces, board.turn)
              << std::endl;

    int w = popCount(board.whitePieces);
    int b = popCount(board.blackPieces);
    std::cout << "Current score: " << w << "-" << b << endl;
    std::cout << "-----" << std::endl;
}

void printBoardWithIndexAndLegalMoves(Board &board) {
    std::cout << getNiceBoardWithIndexAndLegalMoves(board, board.whitePieces, board.blackPieces, board.turn)
              << std::endl;

    int w = popCount(board.whitePieces);
    int b = popCount(board.blackPieces);
    std::cout << "Current score: " << w << "-" << b << endl;
    std::cout << "-----" << std::endl;
}

void printBoardHelper(long white, long black, int turn) {
    std::cout << getNiceBoard(white, black, turn) << std::endl;
    std::cout << "-----" << std::endl;
}

std::__cxx11::string getNiceBoard(long white, long black, int turn) {

    std::string out_string;
    std::string ss;

    std::__cxx11::string whiteString = std::bitset<64>(white).to_string();
    std::__cxx11::string blackString = std::bitset<64>(black).to_string();


    ss += "\n+---+---+---+---+---+---+---+---+ \n";
    for (int y = 0; y < 8; ++y) {
        ss += "| ";
        for (int i = 0; i < 8; ++i) {
            int index = (y * 8 + i);
            char &ww = whiteString[index];
            char &bb = blackString[index];
            if (ww == '1') {
                ss += whiteSymbol;
            } else if (bb == '1') {
                ss += blackSymbol;
            } else {
                ss += ' ';
            }

            ss += " | ";

        }
        ss += "\n+---+---+---+---+---+---+---+---+ \n";
    }

    if (turn == WHITE) {
        ss += "\nWhite (";
        ss += whiteSymbol;
    } else if (turn == BLACK) {
        ss += "\nBlack (";
        ss += blackSymbol;
    }

    ss += ") to play\n";

    return ss;
}

std::__cxx11::string getNiceBoardWithIndex(long white, long black, int turn) {

    std::string out_string;
    std::string ss;

    std::__cxx11::string whiteString = std::bitset<64>(white).to_string();
    std::__cxx11::string blackString = std::bitset<64>(black).to_string();


    ss += "\n   +---+---+---+---+---+---+---+---+ \n";
//    ss += "---+---+---+---+---+---+---+---+---+ \n";
    for (int y = 0; y < 8; ++y) {
        ss += to_string(8 - y);
        ss += "  | ";
        for (int i = 0; i < 8; ++i) {
            int index = (y * 8 + i);
            char &ww = whiteString[index];
            char &bb = blackString[index];
            if (ww == '1') {
                ss += whiteSymbol;
            } else if (bb == '1') {
                ss += blackSymbol;
            } else {
                ss += ' ';
            }

            ss += " | ";

        }
        ss += "\n   +---+---+---+---+---+---+---+---+ \n";
//        ss += "\n---+---+---+---+---+---+---+---+---+ \n";
    }
//    ss += "   | a | b | c | d | e | f | g | h | \n";
    ss += "     a   b   c   d   e   f   g   h   \n";

    if (turn == WHITE) {
        ss += "\nWhite (";
        ss += whiteSymbol;
    } else if (turn == BLACK) {
        ss += "\nBlack (";
        ss += blackSymbol;
    }

    ss += ") to play\n";

    return ss;
}

std::__cxx11::string getNiceBoardWithIndexAndLegalMoves(Board &board, long white, long black, int turn) {
    unsigned long moves = board.generateLegalMoves();

    std::string out_string;
    std::string ss;

    std::__cxx11::string whiteString = std::bitset<64>(white).to_string();
    std::__cxx11::string blackString = std::bitset<64>(black).to_string();

    std::__cxx11::string movesString = std::bitset<64>(moves).to_string();


    ss += "\n   +---+---+---+---+---+---+---+---+ \n";
    for (int y = 0; y < 8; ++y) {
        ss += to_string(8 - y);
        ss += "  | ";
        for (int i = 0; i < 8; ++i) {
            int index = (y * 8 + i);
            char &ww = whiteString[index];
            char &bb = blackString[index];
            char &mm = movesString[index];
            if (ww == '1') {
                ss += whiteSymbol;
            } else if (bb == '1') {
                ss += blackSymbol;
            } else if (mm == '1') {
                ss += moveSymbol;
            } else {
                ss += ' ';
            }

            ss += " | ";

        }
        ss += "\n   +---+---+---+---+---+---+---+---+ \n";
    }
    ss += "     a   b   c   d   e   f   g   h   \n";

    if (turn == WHITE) {
        ss += "\nWhite (";
        ss += whiteSymbol;
    } else if (turn == BLACK) {
        ss += "\nBlack (";
        ss += blackSymbol;
    }
    ss += ") to play\n";

    const string &basicString = board.getMovesString();
    ss += basicString;

    return ss;
}

std::__cxx11::string getNiceBoardWithIndexAndLegalMovesColour(Board &board, long white, long black, int turn) {
    unsigned long moves = board.generateLegalMoves();

    std::string out_string;
    std::string ss;

    std::__cxx11::string whiteString = std::bitset<64>(white).to_string();
    std::__cxx11::string blackString = std::bitset<64>(black).to_string();

    std::__cxx11::string movesString = std::bitset<64>(moves).to_string();


    ss += "\n   +---+---+---+---+---+---+---+---+ \n";
    for (int y = 0; y < 8; ++y) {
        ss += to_string(8 - y);
        ss += "  |";
        for (int i = 0; i < 8; ++i) {
            int index = (y * 8 + i);
            char &ww = whiteString[index];
            char &bb = blackString[index];
            char &mm = movesString[index];
            if (ww == '1') {
                ss += whiteSymbolColour;
            } else if (bb == '1') {
                ss += blackSymbolColour;
            } else if (mm == '1') {
                ss += moveSymbolColour;
            } else {
                ss += ' ';
                ss += ' ';
                ss += ' ';
            }

            ss += "|";

        }
        ss += "\n   +---+---+---+---+---+---+---+---+ \n";
    }
    ss += "     a   b   c   d   e   f   g   h   \n";

    if (turn == WHITE) {
        ss += "\nWhite (";
        ss += whiteSymbolColour;
    } else if (turn == BLACK) {
        ss += "\nBlack (";
        ss += blackSymbolColour;
    }
    ss += ") to play\n";

    const string &basicString = board.getMovesString();
    ss += basicString;

    return ss;
}