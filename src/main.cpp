#include <iostream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include "main.h"
#include "Art.h"
#include "Board.h"
#include "engines/EngineRandom.h"
#include "engines/EngineMinimax.h"
#include "evaluators/EvalBasicHeuristics.h"
#include "Perft.h"
#include "engines/EngineMinimaxBetter.h"
#include "Move.h"
#include "MoveUtils.h"
#include "Tests.h"

using namespace std;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCSimplifyInspection"

int main() {
//    tests();
//    speedTests();
    Board board;


    if (false){
        EvalBasicHeuristics evaluator;
        unsigned long moves = board.generateLegalMoves();
//        printBoard(board);
        int eval = evaluator.eval(board, moves);
        cout << eval << endl;
    }
    if (false){
        string m = "d3";
        board.makeMoveS(&m);
//        printBoard(board);
        EvalBasicHeuristics evaluator;
        unsigned long moves = board.generateLegalMoves();
        int eval = evaluator.eval(board, moves);
        cout << eval << endl;
    }

    if (true) {
        EngineBase *engine;

        engine = new EngineMinimaxBetter(0, true, new EvalBasicHeuristics,
                                         "Engine Minimax Better with BasicHeuristics",
                                         10000000);


        unsigned int i = engine->getBestMove(board);
    }

//    unsigned long moves = board.generateLegalMoves();
//    printBoard(board);
//    board.makeMoveLong(board.turn, moves & -moves);
//    printBoard(board);

//    playEngineGames();
//    playPlayerGames();

    return 0;
}

#pragma clang diagnostic pop


#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"

#pragma clang diagnostic ignored "-Wmissing-noreturn"

int playEngineGames() {

    int x = 1;

    EngineBase *engine1;
    EngineBase *engine2;

    bool engine1White = true, printMove = false, printB = false, enginePrint = false;
    while (true) {
        Board board;
//        engine1 = new EngineRandom("Engine Random");
//        engine1 = new EngineMinimax(x, false, new EvalSimpleCount, "Engine Minimax with Simple Count");
//        engine1 = new EngineMinimax(x, enginePrint, new EvalBasicHeuristics, "Engine Minimax with BasicHeuristics");
        engine1 = new EngineMinimaxBetter(x, enginePrint, new EvalBasicHeuristics,
                                          "Engine Minimax Better with BasicHeuristics",
                                          1000);

        engine2 = new EngineRandom("Engine Random");
//        engine2 = new EngineMinimax(x, enginePrint, new EvalSimpleCount, "Engine Minimax with Simple Count");
//        engine2 = new EngineMinimax(x, false, new EvalBasicHeuristics, "Engine Minimax with BasicHeuristics");
//        engine2 = new EngineMinimaxBetter(x, enginePrint, new EvalBasicHeuristics, "Engine Minimax Better with BasicHeuristics");


        cout << "Starting new game between '" << (engine1White ? engine1->name : engine2->name)
             << "' and '" << (!engine1White ? engine1->name : engine2->name) << "'" << endl;

        while (true) {

            if (board.isGameOver()) {
                cout << "Game over!";
                printBoardWithIndexAndLegalMoves(board);
                board.calculateScore();
                if (engine1White) {
                    cout << "'" << engine1->name << "' is white." << endl;
                    cout << "'" << engine2->name << "' is black." << endl;
                } else {
                    cout << "'" << engine2->name << "' is white." << endl;
                    cout << "'" << engine1->name << "' is black." << endl;
                }
                break;
            }

            if (printB) {
                printBoardWithIndexAndLegalMoves(board);
            }

            unsigned long moves = board.generateLegalMoves();
            if (moves == 0) {
                cout << "Because there are no legal moves from this position, your turn is skipped" << endl;
                board.flipTurn();
                continue;
            }


            if (engine1White && board.turn == WHITE) {
                unsigned int move = engine1->getBestMoveInt(board);
                if (printMove) {
                    cout << "Engine1 makes move: " + getMoveStringFromMove(newPieceOnSquare(move)) << endl;
                }
                board.makeMove(move);
            } else {
                unsigned int move = engine2->getBestMoveInt(board);
                if (printMove) {
                    cout << "Engine2 makes move: " + getMoveStringFromMove(newPieceOnSquare(move)) << endl;
                }
                board.makeMove(move);
            }

        }
        x = x + 1;
        cout << "The game has ended." << endl;
//        if (x == 3) {
//            break;
//        }
//        break;

    }

    return 0;
}

#pragma clang diagnostic pop

int playPlayerGames() {

    EngineBase *engine;

    while (true) {
        Board board;
//        engine = new EngineRandom("Engine Random");
//        engine = new EngineMinimax(8, true, new EvalSimpleCount, "Engine Minimax with Simple Count");
//        engine = new EngineMinimax(x, false, new EvalSimpleCount, "Engine Minimax with Simple Count");
//        engine = new EngineMinimax(12, true, new EvalBasicHeuristics, "Engine Minimax with BasicHeuristics");
        engine = new EngineMinimaxBetter(0, true, new EvalBasicHeuristics, "Engine Minimax with BasicHeuristics", 5000);

        cout << "mudpuppy v0.1 by Louis James Mackenzie-Smith" << endl;
        cout << "id name mudpuppy v0.1" << endl;
        cout << "id author Louis James Mackenzie-Smith" << endl;

        while (true) {

            if (board.isGameOver()) {
                cout << "Game over!";
//                printBoardWithIndexAndLegalMoves(board);
                printBoardWithIndexAndLegalMovesColour(board);
                board.calculateScore();
                break;
            }

//            printBoardWithIndexAndLegalMoves(board);
            printBoardWithIndexAndLegalMovesColour(board);

            unsigned long moves = board.generateLegalMoves();
            if (moves == 0) {
                cout << "Because there are no legal moves from this position, your turn is skipped" << endl;
                board.flipTurn();
                continue;
            }

            const vector<string> &moveVec = getMovesAsArrayString(moves);

            string userMove;

            while (true) {
                cout
                        << "Please enter your move (in format 'a1'), type 'x' for analysis, 'r' for random move, or type 'go' for an engine move: "
                        << endl;
                std::getline(cin, userMove);
                if (userMove == "go") {
                    unsigned int move = engine->getBestMoveInt(board);
                    cout << "Engine makes move: " + getMoveStringFromMove(newPieceOnSquare(move)) << endl;
                    board.makeMove(move);
                    break;
                } else if (userMove == "x") {
                    unsigned int move = engine->getBestMoveInt(board);
                    cout << "Engine suggests move: " + getMoveStringFromMove(newPieceOnSquare(move))
                         << ", and suggests this board is worth: " << engine->getDisplayScoreOfMove(board)
                         << " on a scale of -100 to +100 " << endl;

                } else if (userMove == "r") {
                    EngineRandom engineRandom("random engine");
                    unsigned int move = engineRandom.getBestMoveInt(board);
                    cout << "Random Engine makes move: " + getMoveStringFromMove(newPieceOnSquare(move)) << endl;
                    board.makeMove(move);
                    break;
                } else if (std::find(moveVec.begin(), moveVec.end(), userMove) != moveVec.end()) {
                    cout << "Making user move: " + userMove << endl;
                    board.makeMoveS(&userMove);
                    break;
                } else {
                    cout << "That looked weird!" << endl;
                    cout << board.getMovesString() << endl;
                }
            }

        }

        cout << "The game has ended. Would you like to play again? y/n" << endl;

        string ans;

        std::getline(cin, ans);

        if (ans == "y" || ans == "Y") {
            continue;
        }
        break;
    }


    return 0;
}

