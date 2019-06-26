//
// Created by louis on 6/5/19.
//

#ifndef OTHELLOPROJECT_BOARD_H
#define OTHELLOPROJECT_BOARD_H

class Board {
public:
    uint64_t whitePieces, blackPieces;
    Colour turn;
    uint32_t numberOfRealMoves = 0;

public:
    Board();

    Board(const Board &b);

    explicit Board(std::string fen);

    void makeMoveS(std::string *moveString);

    void flipTurn();

    const uint64_t allPieces() const;

    const uint64_t getEmptySquares();

    const uint64_t getNeighbourSquares();

    const uint64_t getBorderPieces() const;

    uint64_t generateLegalMoves();

    std::string getMovesString();

    bool isGameOver();

    int calculateScore();

    const uint64_t getMyPieces() const;

    const uint64_t getEnemyPieces() const;

    void unMakeMove();

    void makeMoveLong(Colour t, uint64_t move);

    void makeMoveLong(uint64_t move);
};

std::string getMoveStringFromMove(uint64_t l);

uint64_t newPieceOnSquare(uint32_t index);

uint32_t getIndexLowestBit(uint64_t l);

uint32_t getIndexHighestBit(uint64_t b);

int first_bit(uint64_t b);

uint32_t getMoveFromMoveString(std::string *moveString);

int popCount(uint64_t l);

void setup();

#endif //OTHELLOPROJECT_BOARD_H
