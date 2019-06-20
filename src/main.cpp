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
#include "searchUtils/TranspositionTable.h"

using namespace std;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCSimplifyInspection"

int main() {
//    masterTest();

    Board board;
//    printBoardWithIndexAndLegalMoves(board);

    if (false) {
        EvalBasicHeuristics evaluator;
        unsigned long moves = board.generateLegalMoves();
//        printBoard(board);
        int eval = evaluator.eval(board, moves);
        cout << eval << endl;
    }
    if (false) {
        string m = "d3";
        board.makeMoveS(&m);
//        printBoard(board);
        EvalBasicHeuristics evaluator;
        unsigned long moves = board.generateLegalMoves();
        int eval = evaluator.eval(board, moves);
        cout << eval << endl;
    }

    if (false) {
        EngineBase *engine;

        printBoardWithIndexAndLegalMoves(board);
        engine = new EngineMinimaxBetter(0, true, new EvalBasicHeuristics,
                                         "Engine Minimax Better with BasicHeuristics",
                                         100);


        unsigned int i = engine->getBestMove(board);
    }


    playEngineGames();
//    playPlayerGames();

    return 0;
}

#pragma clang diagnostic pop


#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"

#pragma clang diagnostic ignored "-Wmissing-noreturn"

int playEngineGames() {

    TimePoint s = now();

    int numberOfFlippedIterations = 4;

    int initialDepth = 3, maxDepth, absoluteMaxDepth = 9;
    int engine1Win = 0, engine2Win = 0, draw = 0;

    EngineBase *engine1;
    EngineBase *engine2;

    bool engine1White, printMove = false, printB = false, enginePrint = false;

    for (int i = 0; i < numberOfFlippedIterations * 2; i++) {
        engine1White = i % 2 != 0;
        cout << "\n" << endl;
        cout << "----- Iteration " << (i + 1) << " -----" << endl;
        if (engine1White) {
            cout << "engine1 is white" << endl;
        }else {
            cout << "engine1 is black" << endl;
        }
        Colour engine1playOnTurn = engine1White ? WHITE : BLACK;

        maxDepth = initialDepth;
        while (true) {
            Board board;
            assert(board.numberOfRealMoves == 0);
//        engine1 = new EngineRandom("Engine Random");
//        engine1 = new EngineMinimax(maxDepth, false, new EvalSimpleCount, "Engine Minimax with Simple Count");
//        engine1 = new EngineMinimax(maxDepth, enginePrint, new EvalBasicHeuristics, "Engine Minimax with BasicHeuristics");
            engine1 = new EngineMinimaxBetter(0 * maxDepth, enginePrint, new EvalBasicHeuristics,
                                              "Engine Minimax Better with BasicHeuristics",
                                              100);

//        engine2 = new EngineRandom("Engine Random");
//        engine2 = new EngineMinimax(maxDepth, enginePrint, new EvalSimpleCount, "Engine Minimax with Simple Count");
            engine2 = new EngineMinimax(maxDepth, enginePrint, new EvalBasicHeuristics,
                                        "Engine Minimax with BasicHeuristics");
//        engine2 = new EngineMinimaxBetter(maxDepth, enginePrint, new EvalBasicHeuristics, "Engine Minimax Better with BasicHeuristics");


//        cout << "Starting new game between '" << (engine1White ? engine1->name : engine2->name)
//             << "' and '" << (!engine1White ? engine1->name : engine2->name) << "'" << endl;

            while (true) {

                if (board.isGameOver()) {
//                cout << "Game over!" << endl;
//                printBoardWithIndexAndLegalMoves(board);

                    int wnb = board.calculateScore();
                    if (engine1White) {
                        if (wnb > 0) {
                            engine1Win++;
                        }
                        if (wnb < 0) {
                            engine2Win++;
                        }

                    } else {
                        if (wnb > 0) {
                            engine2Win++;
                        }
                        if (wnb < 0) {
                            engine1Win++;
                        }
                    }
                    if (wnb == 0) {
                        draw++;
                    }
                    cout << "depth: " << maxDepth << ", "
                         << engine1Win << "|" << engine2Win << "|" << draw
                         << " | " << (engine1Win+engine2Win+draw)<< endl;
                    if (engine1White) {
//                    cout << "'" << engine1->name << "' is white." << endl;
//                    cout << "'" << engine2->name << "' is black." << endl;
                    } else {
//                    cout << "'" << engine2->name << "' is white." << endl;
//                    cout << "'" << engine1->name << "' is black." << endl;
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


                if (board.turn == engine1playOnTurn) {
                    unsigned long move = engine1->getBestMove(board);
                    if (printMove) {
                        cout << "Engine1 makes move: " + getMoveStringFromMove((move)) << endl;
                    }
                    board.makeMoveLong(board.turn, move);
                } else {
                    unsigned long move = engine2->getBestMove(board);
                    if (printMove) {
                        cout << "Engine2 makes move: " + getMoveStringFromMove((move)) << endl;
                    }
                    board.makeMoveLong(board.turn, move);
                }


            }
            if (maxDepth >= absoluteMaxDepth) {
                break;
            }
            maxDepth = maxDepth + 1;
//        cout << "The game has ended." << endl;
//        break;
//        if (x == 3) {
//            break;
//        }
//        break;

        }

    }

//    delete engine1, engine2;

    TimePoint e = now();

    cout << "time taken for engine vs engine games: " << (e - s) << endl;

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

