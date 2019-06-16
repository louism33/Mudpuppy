//
// Created by louis on 6/5/19.
//

#ifndef OTHELLOPROJECT_BOARD_H
#define OTHELLOPROJECT_BOARD_H

class Board {
public:
    unsigned long whitePieces, blackPieces;
    Colour turn;


public:
    Board();

    Board(const Board &b);

    explicit Board(std::string fen);

    void makeMove(Colour t, int index);

    void makeMove(unsigned int index);

    void makeMoveS(std::string *moveString);

    void flipTurn();

    unsigned long allPieces();

    unsigned long getEmptySquares();

    unsigned long getNeighbourSquares();

    unsigned long getBorderPieces();

    unsigned long generateLegalMoves();

    std::string getMovesString();

    bool isGameOver();

    int calculateScore();

    const unsigned long getMyPieces() const;

    const unsigned long getEnemyPieces() const;

    void unMakeMove();

};

std::string getMoveStringFromMove(unsigned long l);

unsigned long newPieceOnSquare(unsigned int index);

unsigned int getIndexLowestBit(unsigned long l);

unsigned int getIndexHighestBit(unsigned long b);

int first_bit(unsigned long long b);

unsigned int getMoveFromMoveString(std::string *moveString);

int popCount(unsigned long l);

void setup();

#endif //OTHELLOPROJECT_BOARD_H
