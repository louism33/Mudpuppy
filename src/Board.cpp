//
// Created by louis on 6/5/19.
//

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
#include "MoveUtils.h"
#include "Move.h"

#include <cstdint>

using namespace std;


static uint64_t INITIAL_WHITE = 0x1008000000;
static uint64_t INITIAL_BLACK = 0x810000000;
static const string startPosStr = "---------------------------O*------*O---------------------------";
static Colour INITIAL_TURN = Colour::WHITE;

long *whiteStackP;
long *blackStackP;
int masterIndex = 0;

void Board::reset(){
    setup();
    this->blackPieces = INITIAL_BLACK;
    this->whitePieces = INITIAL_WHITE;
    this->turn = INITIAL_TURN;
    masterIndex = 0;
    this->numberOfRealMoves = 0;
}
Board::Board() {
    setup();
    this->blackPieces = INITIAL_BLACK;
    this->whitePieces = INITIAL_WHITE;
    this->turn = INITIAL_TURN;
    masterIndex = 0;
    this->numberOfRealMoves = 0;
}

Board::Board(const Board &b) {
    setup();
    cout << "copy constructor " << endl;
    this->blackPieces = b.blackPieces;
    this->whitePieces = b.whitePieces;
    this->turn = b.turn;
}

bool boardReady = false;

void setup() {
    setupStarMask();
    whiteStackP = new long[128];
    blackStackP = new long[128];
    boardReady = true;
}

int Board::calculateScore() {
    int w = popCount(this->whitePieces);
    int b = popCount(this->blackPieces);
    if (w > b) {
        cout << "Victory for White: " + to_string(w) + "-" + to_string(b) << endl;
    } else if (b > w) {
        cout << "Victory for Black: " + to_string(b) + "-" + to_string(w) << endl;
    } else {
        cout << "Draw!" << endl;
    }

    return w - b;
}

bool Board::isGameOver() {
    if (this->generateLegalMoves() == 0) {
        this->flipTurn();
        if (this->generateLegalMoves() == 0) {
            this->flipTurn();
            return true;
        }
    }
    return false;
}

const uint64_t Board::getMyPieces() const {
    if (this->turn == WHITE) {
        return this->whitePieces;
    }
    return this->blackPieces;
}

const uint64_t Board::getEnemyPieces() const {
    if (this->turn == BLACK) {
        return this->whitePieces;
    }
    return this->blackPieces;
}

void Board::makeMoveSB(string *moveString) {
    if (*moveString == "PA") {
        this->makeMoveLong(PASS_MOVE);
    } else {
        uint32_t index = getMoveFromMoveStringBig(moveString);
        this->makeMoveLong(this->turn, newPieceOnSquare(index));
    }
}

void Board::makeMoveS(string *moveString) {
    uint32_t index = getMoveFromMoveString(moveString);
    this->makeMoveLong(this->turn, newPieceOnSquare(index));
}

void Board::makeMoveLong(uint64_t move) {
    makeMoveLong(this->turn, move);
}

void Board::makeMoveLong(Colour t, uint64_t move) {
    assert(move == PASS_MOVE || popCount(move) == 1);
    assert(numberOfRealMoves >= 0);


    setupAttacksDatabase();
    this->flipTurn();
    whiteStackP[masterIndex] = this->whitePieces;
    blackStackP[masterIndex] = this->blackPieces;
    masterIndex++;

    this->numberOfRealMoves++;
    if (move == PASS_MOVE) {
        return;
    }

    bool b = (move & (this->whitePieces | this->blackPieces)) == 0;
    if (!b) {
        cout << "trying to make a move but someone already there" << endl;
        cout << getMoveStringFromMove(move) << endl;
        printBoard(*this);
        printLong(move);
    }
    assert(b);

    const bool w = t == WHITE;
    const uint64_t enemies = !w ? this->whitePieces : this->blackPieces;
    const uint64_t friends = w ? this->whitePieces : this->blackPieces;

    const uint32_t index = getIndexLowestBit(move);
    const uint64_t caps = ::getMoveCaptures(index, friends, enemies);

    if (!caps) {
        printBoard(*this);
        printLong(move);
        cout << (move == PASS_MOVE) << endl;
    }

    if (!caps) {
        cout << "ERROR, no caps found!" << endl;
        printBoardWithIndexAndLegalMoves(*this);
        cout << "trying to make move: " << endl;
        printLong(move);
    }
    assert(caps);

    if (w) {
        this->whitePieces |= caps;
        this->blackPieces ^= caps;
        this->whitePieces |= move;
    } else {
        this->blackPieces |= caps;
        this->whitePieces ^= caps;
        this->blackPieces |= move;
    }
}

void Board::unMakeMove() {
    assert(masterIndex > 0);
    this->numberOfRealMoves--;
    masterIndex--;
    this->whitePieces = whiteStackP[masterIndex];
    this->blackPieces = blackStackP[masterIndex];

    this->flipTurn();
}

const uint64_t Board::allPieces() const {
    return this->whitePieces | this->blackPieces;
}

void Board::flipTurn() {
    this->turn = this->turn == WHITE ? BLACK : WHITE;
}

const uint64_t Board::getEmptySquares() {
    return ~this->allPieces();
}

const uint64_t Board::getNeighbourSquares() {
    const uint64_t p = this->allPieces();
    // clockwise
    return ~p &
           (((p & ~U_BORDER) << 8u) | ((p & ~U_R_BORDER) << 7u) | ((p & ~R_BORDER) >> 1u) | ((p & ~D_R_BORDER) >> 9u)
            | ((p & ~D_BORDER) >> 8u) | ((p & ~D_L_BORDER) >> 7u) | ((p & ~L_BORDER) << 1u) |
            ((p & ~U_L_BORDER) << 9u));
}

const uint64_t Board::getBorderPieces() const {
    const uint64_t p = this->allPieces();
    // clockwise
    return p &
           (~((p & ~U_BORDER) << 8u) | ~((p & ~R_BORDER) >> 1u) | ~((p & ~D_BORDER) >> 8u) | ~((p & ~L_BORDER) << 1u));
}


uint64_t newPieceOnSquare(uint32_t index) {
    return (1ul << index);
}

uint32_t getIndexLowestBit(uint64_t b) {
    __asm__("bsfq %1,%0" : "=r" (b) : "rm" (b));
    return (int) b;
}

uint32_t getIndexHighestBit(uint64_t b) {
    __asm__("bsrq %1,%0" :"=r" (b) :"rm" (b));
    return b;
}

int popCount(uint64_t b) {
//    return __builtin_popcountll(b);
    __asm__("popcntq %1,%0" :"=r" (b) :"rm" (b));
    return (int) b;
}

uint32_t getMoveFromMoveString(string *moveString) {
    char &f = (*moveString)[0];
    int file = ('h' - f) % 8;

    char &r = (*moveString)[1];
    int row = (r - '0') - 1;

    return file + row * 8;
}

uint32_t getMoveFromMoveStringBig(string *moveString) {
    char &f = (*moveString)[0];
    int file = ('H' - f) % 8;

    char &r = (*moveString)[1];
    int row = (r - '0') - 1;

    return file + row * 8;
}

string Board::getMovesString() {
    uint64_t m = this->generateLegalMoves();
    if (!m) {
        return "No legal moves from this position";
    }
    string s = "Moves: ";
    while (m) {
        s += getMoveStringFromMove(m & -m);
        m = m & (m - 1);
        if (m) {
            s += ", ";
        }
    }
    return s;
}

string getMoveStringFromMoveCAP(uint64_t l) {
    if (l == PASS_MOVE) {
        return std::string("PA");
    }
    uint32_t index = getIndexLowestBit(l);
    int file = index % 8;
    char f = 'H' - file;

    int row = 1 + index / 8;

    return std::string(1, f) + "" + to_string(row);
}

string getMoveStringFromMove(uint64_t l) {
    if (l == PASS_MOVE) {
        return std::string("PA");
    }
    uint32_t index = getIndexLowestBit(l);
    int file = index % 8;
    char f = 'h' - file;

    int row = 1 + index / 8;

    return std::string(1, f) + "" + to_string(row);
}

uint64_t N(uint64_t l) {
    return ((l & ~U_BORDER) << 8u);
}

uint64_t E(uint64_t l) {
    return ((l & ~R_BORDER) >> 1u);
}

uint64_t W(uint64_t l) {
    return ((l & ~L_BORDER) << 1u);
}

uint64_t S(uint64_t l) {
    return ((l & ~D_BORDER) >> 8u);
}

uint64_t NW(uint64_t l) {
    return ((l & ~U_L_BORDER) << 9u);
}

uint64_t NE(uint64_t l) {
    return ((l & ~U_R_BORDER) << 7u);
}

uint64_t SW(uint64_t l) {
    return ((l & ~D_L_BORDER) >> 7u);
}

uint64_t SE(uint64_t l) {
    return ((l & ~D_R_BORDER) >> 9u);
}

uint64_t Board::generateLegalMoves() {
    const bool w = this->turn == WHITE;
    const uint64_t own = w ? this->whitePieces : this->blackPieces;
    const uint64_t enemy = !w ? this->whitePieces : this->blackPieces;
    uint64_t result = 0ull;
    int i;

    uint64_t empty = ~(own | enemy);
    uint64_t victims = N(own) & enemy;
    for (i = 0; i < 5; ++i)
        victims |= N(victims) & enemy;
    result |= N(victims);

    victims = S(own) & enemy;
    for (i = 0; i < 5; ++i)
        victims |= S(victims) & enemy;
    result |= S(victims);

    victims = W(own) & enemy;
    for (i = 0; i < 5; ++i)
        victims |= W(victims) & enemy;
    result |= W(victims);

    victims = E(own) & enemy;
    for (i = 0; i < 5; ++i)
        victims |= E(victims) & enemy;
    result |= E(victims);

    victims = NW(own) & enemy;
    for (i = 0; i < 5; ++i)
        victims |= NW(victims) & enemy;
    result |= NW(victims);

    victims = NE(own) & enemy;
    for (i = 0; i < 5; ++i)
        victims |= NE(victims) & enemy;
    result |= NE(victims);

    victims = SW(own) & enemy;
    for (i = 0; i < 5; ++i)
        victims |= SW(victims) & enemy;
    result |= SW(victims);

    victims = SE(own) & enemy;
    for (i = 0; i < 5; ++i)
        victims |= SE(victims) & enemy;
    result |= SE(victims);

    return result & empty;
}

Board::Board(std::string fen) {
    cout << fen << endl;
    uint64_t w = 0, b = 0;
    for (int i = 0; i < fen.length(); i++) {
        char &c = fen[i];
        if (c == '-') {
        } else if (c == 'O') {
            w |= newPieceOnSquare(i);
        } else if (c == '*') {
            b |= newPieceOnSquare(i);
        }
    }
    this->whitePieces = w;
    this->blackPieces = b;
}

