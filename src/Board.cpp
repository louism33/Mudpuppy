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

using namespace std;


static unsigned long INITIAL_WHITE = 0x1008000000;
static unsigned long INITIAL_BLACK = 0x810000000;

static Colour INITIAL_TURN = Colour::WHITE;

long *whiteStackP;
long *blackStackP;
int masterIndex = 0;

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
//    if (boardReady) {
//        cout << "board already ready" << endl;
//        return;
//    }
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

const unsigned long Board::getMyPieces() const {
    if (this->turn == WHITE) {
        return this->whitePieces;
    }
    return this->blackPieces;
}

const unsigned long Board::getEnemyPieces() const {
    if (this->turn == BLACK) {
        return this->whitePieces;
    }
    return this->blackPieces;
}

void Board::makeMoveS(string *moveString) {
    unsigned int index = getMoveFromMoveString(moveString);
    this->makeMove(this->turn, index);
}

void Board::makeMove(unsigned int index) {
    this->makeMove(this->turn, index);
}

void Board::makeMove(Colour t, int index) {
    setupAttacksDatabase();
    assert(index >= 0);
    assert(index <= 63);

    throw runtime_error("");

    this->numberOfRealMoves++;
    const bool w = t == WHITE;
    this->flipTurn();

    whiteStackP[masterIndex] = this->whitePieces;
    blackStackP[masterIndex] = this->blackPieces;

    masterIndex++;
    unsigned long enemies = !w ? this->whitePieces : this->blackPieces;
    unsigned long friends = w ? this->whitePieces : this->blackPieces;

    unsigned long piece = newPieceOnSquare(index);
    unsigned long caps = ::getMoveCaptures(index, friends, enemies);

    assert(caps);

//    unsigned long bw = this->whitePieces;
//    unsigned long bb = this->blackPieces;

    if (w) {
//        assert(!(caps & this->whitePieces));
//        assert((caps | this->blackPieces) == this->blackPieces);

        this->whitePieces |= caps;
        this->blackPieces ^= caps;
        this->whitePieces |= piece;
    } else {
//        assert(!(caps & this->blackPieces));
//        assert((caps | this->whitePieces) == this->whitePieces);

        this->blackPieces |= caps;
        this->whitePieces ^= caps;
        this->blackPieces |= piece;
    }
//    this->flipTurn();

    // check that no pieces have (dis)appeared
//    unsigned long mw = this->whitePieces;
//    unsigned long mb = this->blackPieces;
//    assert(popCount(bw) + popCount(bb) == popCount(mw) + popCount(mb));


//    unsigned long aw = this->whitePieces;
//    unsigned long ab = this->blackPieces;
//    assert(popCount(bw) + popCount(bb) == popCount(aw) + popCount(ab) - 1);
//    assert((bw | bb | piece) == (aw | ab));
//    if (t == WHITE) {
//        assert(popCount(aw) >= popCount(bw) + 2);
//        assert(popCount(ab) <= popCount(bb) - 1);
//    }
//    if (t == BLACK) {
//        assert(popCount(ab) >= popCount(bb) + 2);
//        assert(popCount(aw) <= popCount(bw) - 1);
//    }

}

void Board::makeMoveLong(Colour t, unsigned long move) {
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

    const bool w = t == WHITE;
    unsigned long enemies = !w ? this->whitePieces : this->blackPieces;
    unsigned long friends = w ? this->whitePieces : this->blackPieces;

    unsigned int index = getIndexLowestBit(move);
    unsigned long caps = ::getMoveCaptures(index, friends, enemies);

    if (!caps) {
        printBoard(*this);
        printLong(move);
        cout << (move == PASS_MOVE) <<endl;
    }

    if (!caps){
        printBoardWithIndexAndLegalMoves(*this);
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

const unsigned long Board::allPieces() const {
    return this->whitePieces | this->blackPieces;
}

void Board::flipTurn() {
    this->turn = this->turn == WHITE ? this->turn = BLACK : this->turn = WHITE;
}

unsigned long Board::getEmptySquares() {
    return ~this->allPieces();
}

unsigned long Board::getNeighbourSquares() {
    const unsigned long p = this->allPieces();
    // clockwise
    return ~p &
           (((p & ~U_BORDER) << 8u) | ((p & ~U_R_BORDER) << 7u) | ((p & ~R_BORDER) >> 1u) | ((p & ~D_R_BORDER) >> 9u)
            | ((p & ~D_BORDER) >> 8u) | ((p & ~D_L_BORDER) >> 7u) | ((p & ~L_BORDER) << 1u) |
            ((p & ~U_L_BORDER) << 9u));
}

const unsigned long Board::getBorderPieces() const {
    const unsigned long p = this->allPieces();
    // clockwise
    return p & (~((p & ~U_BORDER) << 8u) | ~((p & ~R_BORDER) >> 1u) | ~((p & ~D_BORDER) >> 8u) | ~((p & ~L_BORDER) << 1u));
}


unsigned long newPieceOnSquare(unsigned int index) {
    return (1ul << index);
}

unsigned int getIndexLowestBit(unsigned long b) {
    __asm__("bsfq %1,%0" : "=r" (b) : "rm" (b));
    return (int) b;
}

unsigned int getIndexHighestBit(unsigned long b) {
    __asm__("bsrq %1,%0" :"=r" (b) :"rm" (b));
    return b;
}


//int popCount(unsigned long x) {
//    int count = 0;
//    while (x) {
//        count++;
//        x &= x - 1;
//    }
//    return count;
//}

int popCount(unsigned long b) {
//    return __builtin_popcountll(b);
    __asm__("popcntq %1,%0" :"=r" (b) :"rm" (b));
    return (int) b;
}

unsigned int getMoveFromMoveString(string *moveString) {
    char &f = (*moveString)[0];
    int file = (int) (('h' - f) % 8);

    char &r = (*moveString)[1];
    int row = (r - '0') - 1;

    return file + row * 8;
}

string Board::getMovesString() {
    unsigned long m = this->generateLegalMoves();
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

//todo, should not be part of Board::
string getMoveStringFromMove(unsigned long l) {
    unsigned int index = getIndexLowestBit(l);
    int file = index % 8;
    char f = 'h' - file;

    int row = 1 + index / 8;

    return std::string(1, f) + "" + to_string(row);
}

unsigned long N(unsigned long l) {
    return ((l & ~U_BORDER) << 8u);
}

unsigned long E(unsigned long l) {
    return ((l & ~R_BORDER) >> 1u);
}

unsigned long W(unsigned long l) {
    return ((l & ~L_BORDER) << 1u);
}

unsigned long S(unsigned long l) {
    return ((l & ~D_BORDER) >> 8u);
}

unsigned long NW(unsigned long l) {
    return ((l & ~U_L_BORDER) << 9u);
}

unsigned long NE(unsigned long l) {
    return ((l & ~U_R_BORDER) << 7u);
}

unsigned long SW(unsigned long l) {
    return ((l & ~D_L_BORDER) >> 7u);
}

unsigned long SE(unsigned long l) {
    return ((l & ~D_R_BORDER) >> 9u);
}

unsigned long Board::generateLegalMoves() {
    const bool w = this->turn == WHITE;
    const unsigned long own = w ? this->whitePieces : this->blackPieces;
    const unsigned long enemy = !w ? this->whitePieces : this->blackPieces;
    unsigned long result = 0ull;
    int i;

    unsigned long empty = ~(own | enemy);
    unsigned long victims = N(own) & enemy;
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

    // todo
}

