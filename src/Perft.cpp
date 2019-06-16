//
// Created by louis on 6/9/19.
//
//

//
// Created by louis on 6/9/19.
//

#include "Perft.h"
#include <chrono>

using namespace std;

unsigned long Perft::getNps(unsigned long nodes, long time) {
    if (time == 0) {
        return 0;
    }
    return (nodes / time) * 1000;
}

unsigned long Perft::splitPerft(Board *board, int depth) {
    std::cout << "Calculating Perft to depth " << depth << std::endl;

    chrono::milliseconds startTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    unsigned long moves = board->generateLegalMoves();
    unsigned long total = 0;

    if (depth <= 1) {
        return popCount(moves) >= 1 ? popCount(moves) : 1;
    }

    while (moves) {
        unsigned long move = moves & -moves;
        unsigned int moveIndex = getIndexLowestBit(move);
        const std::string &s = getMoveStringFromMove(move);
        cout << s << ": ";
        board->makeMove(moveIndex);
        unsigned long p = perft(board, depth - 1, false);
        board->unMakeMove();
        cout << p << endl;
        total += p;
        moves &= moves - 1;
    }

    chrono::milliseconds currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    long i = currentTime.count();
    long finalTime = i - startTime.count();

    std::cout << "Depth: " << depth << ", nodes: " << total << " time: " << finalTime << " milliseconds" << std::endl;
    std::cout << "NPS: " << getNps(total, finalTime) << std::endl;

    return total;
}

unsigned long Perft::perft(Board *board, int depth) {
    std::cout << "Calculating Perft to depth " << depth << std::endl;

    chrono::milliseconds startTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    unsigned long total = perft(board, depth, false);

    chrono::milliseconds currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    long finalTime = currentTime.count() - startTime.count();

    std::cout << "Depth: " << depth << ", nodes: " << total << " time: " << finalTime << " milliseconds" << std::endl;
    std::cout << "NPS: " << getNps(total, finalTime) << std::endl;

    return total;
}

unsigned long Perft::perft(Board *board, int depth, bool passed) {

    if (depth == 0) {
        return 1;
    }
    if (depth == 1) { // do not ignore leaf nodes with no moves
        int i = popCount(board->generateLegalMoves());
        return i > 0 ? i : 1;
    }

    unsigned long nodes = 0;
    unsigned long moves = board->generateLegalMoves();

    if (moves == 0) {
        if (passed) {
            return 1;
        }

        board->flipTurn();
        nodes += perft(board, depth - 1, true);
        board->flipTurn();

    } else {
        while (moves) {
            unsigned long move = moves & -moves;
            unsigned long moveIndex = getIndexLowestBit(move);

//            assert(popCount(move) == 1);

//            unsigned long bw = board->whitePieces;
//            unsigned long bb = board->blackPieces;
//            Colour mover = board->turn;

//            board->makeMove(moveIndex);
            board->makeMove(board->turn, moveIndex);

//            unsigned long aw = board->whitePieces;
//            unsigned long ab = board->blackPieces;

//            assert(popCount(aw) + popCount(ab) - 1 == popCount(bw) + popCount(bb));
//            assert((bw | bb | move) == (aw | ab));
//
//            if (mover == WHITE) {
//                assert(popCount(aw) >= popCount(bw) + 2);
//                assert(popCount(ab) <= popCount(bb) - 1);
//            }
//
//            if (mover == BLACK) {
//                assert(popCount(ab) >= popCount(bb) + 2);
//                assert(popCount(aw) <= popCount(bw) - 1);
//            }

            nodes += perft(board, depth - 1, false);

            board->unMakeMove();

            moves &= moves - 1;
        }
    }

    return nodes;
}