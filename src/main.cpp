#include <iostream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cstring>
#include "main.h"
#include "Art.h"
#include "Board.h"
#include "engines/EngineRandom.h"
#include "engines/EngineMinimaxV1.h"
#include "evaluators/EvalBasicHeuristics.h"
#include "Perft.h"
#include "engines/EngineMinimaxV2.h"
#include "Move.h"
#include "MoveUtils.h"
#include "Tests.h"
#include "searchUtils/TranspositionTable.h"
#include "engines/EngineMinimaxV3.h"
#include "NBoard.h"

using namespace std;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCSimplifyInspection"

int main(int argc, char **argv) {
    // todo quiescence for corners

    if (argc <= 1) {
//        playEngineGames();
        playPlayerGames();

    } else if (strncmp(argv[1], "tests", 5) == 0) {
        cout << "----- running debug tests -----" << endl;
        masterTest();
        cout << "----- running speed tests -----" << endl;
        speedTests();
    } else if (strncmp(argv[1], "nboard", 6) == 0) {
        cout << "----- preparing mudpuppy to follow nboard protocol  -----" << endl;

        EngineMinimaxV3 *engine = new EngineMinimaxV3(0, false, new EvalBasicHeuristics,
                                                      "Engine Minimax Better with BasicHeuristics",
                                                      1000);
        mainLoopNBoard(engine);
    }


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
    long initialTime = 50, maxTime, absoluteMaxTime = 200, timeInc = 50;
    bool increaseTimeE1 = true, increaseTimeE2 = true;

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
        } else {
            cout << "engine1 is black" << endl;
        }
        Colour engine1playOnTurn = engine1White ? WHITE : BLACK;


        maxDepth = initialDepth;
        maxTime = initialTime;

        while (true) {
            Board board;
            assert(board.numberOfRealMoves == 0);
//        engine1 = new EngineRandom("Engine Random");
//        engine1 = new EngineMinimaxV1(maxDepth, false, new EvalSimpleCount, "Engine Minimax with Simple Count");
//        engine1 = new EngineMinimaxV1(maxDepth, enginePrint, new EvalBasicHeuristics, "Engine Minimax with BasicHeuristics");
//            engine1 = new EngineMinimaxV2(0 * maxDepth, enginePrint, new EvalBasicHeuristics,
//                                              "Engine Minimax Better with BasicHeuristics",
//                                              100);
            engine1 = new EngineMinimaxV3(0 * maxDepth, enginePrint, new EvalBasicHeuristics,
                                          "Engine Minimax Better with BasicHeuristics",
                                          increaseTimeE1 ? maxTime : initialTime);

//        engine2 = new EngineRandom("Engine Random");
//        engine2 = new EngineMinimaxV1(maxDepth, enginePrint, new EvalSimpleCount, "Engine Minimax with Simple Count");
//            engine2 = new EngineMinimaxV1(maxDepth, enginePrint, new EvalBasicHeuristics,
//                                        "Engine Minimax with BasicHeuristics");
            engine2 = new EngineMinimaxV2(0 * maxDepth, enginePrint, new EvalBasicHeuristics,
                                          "Engine Minimax Better with BasicHeuristics",
                                          increaseTimeE2 ? maxTime : initialTime);


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
                    cout << "depth: " << maxDepth << ", time: " << maxTime << ", "
                         << engine1Win << "|" << engine2Win << "|" << draw
                         << " | " << (engine1Win + engine2Win + draw) << endl;
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

                uint64_t moves = board.generateLegalMoves();
                if (moves == 0) {
                    cout << "Because there are no legal moves from this position, your turn is skipped" << endl;
                    board.flipTurn();
                    continue;
                }


                if (board.turn == engine1playOnTurn) {
                    uint64_t move = engine1->getBestMove(board);
                    if (printMove) {
                        cout << "Engine1 makes move: " + getMoveStringFromMove((move)) << endl;
                    }
                    board.makeMoveLong(board.turn, move);
                } else {
                    uint64_t move = engine2->getBestMove(board);
                    if (printMove) {
                        cout << "Engine2 makes move: " + getMoveStringFromMove((move)) << endl;
                    }
                    board.makeMoveLong(board.turn, move);
                }


            }
            if (maxDepth >= absoluteMaxDepth) {
                break;
            }
            if (maxTime > absoluteMaxTime) {
                break;
            }
            maxDepth = maxDepth + 1;
            maxTime = maxTime + timeInc;

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

    cout << "time taken for engine vs engine games: "
         << (e - s) / 60000 << " min "
         << ((e - s) / 1000) % 60 << " sec "
         << ((e - s) % 1000) << " mil"
         << endl;

    return 0;
}

#pragma clang diagnostic pop

int playPlayerGames() {

    EngineBase *engine;

    while (true) {
        Board board;
//        engine = new EngineRandom("Engine Random");
//        engine = new EngineMinimaxV1(8, true, new EvalSimpleCount, "Engine Minimax with Simple Count");
//        engine = new EngineMinimaxV1(x, false, new EvalSimpleCount, "Engine Minimax with Simple Count");
//        engine = new EngineMinimaxV1(12, true, new EvalBasicHeuristics, "Engine Minimax with BasicHeuristics");
//        engine = new EngineMinimaxV2(0, true, new EvalBasicHeuristics, "Engine Minimax with BasicHeuristics", 1000);
        engine = new EngineMinimaxV3(0, true, new EvalBasicHeuristics, "Engine Minimax with BasicHeuristics", 1000);

        cout << "mudpuppy v0.2 by Louis James Mackenzie-Smith" << endl;
        cout << "id name mudpuppy v0.2" << endl;
        cout << "id author Louis James Mackenzie-Smith" << endl;


        string tl, debug;
        bool enginePrint = false;

        cout << "Engine config:"
             << endl
             << "Time Limit per move in milliseconds (write '0' if you want the engine to search to a specific depth):"
             << endl;
        std::getline(cin, tl);
        if (tl == "0") {
            string dl;
            cout << "Since we play with no time limit, please specify Depth Limit (1-18ish):" << endl;
            std::getline(cin, dl);
            cout << "Setting Depth limit to " << stoi(dl) << endl;
            engine->setDepthLimit(stoi(dl));
        } else {
            cout << "Setting Time limit to " << stoi(tl) << endl;
            engine->setTimeLimitMillis(stoi(tl));
        }

        cout << "Do you want the engine to print what it is thinking? y/n:"
             << endl;
        std::getline(cin, debug);
        if (debug == "y" || debug == "Y" || debug == "yes" || debug == "Yes") {
            enginePrint = true;
        } else {
            enginePrint = false;
        }

        engine->setPrint(enginePrint);

        while (true) {

            if (board.isGameOver()) {
                cout << "Game over!";
                printBoardWithIndexAndLegalMovesColour(board);
                board.calculateScore();
                break;
            }

            printBoardWithIndexAndLegalMovesColour(board);

            uint64_t moves = board.generateLegalMoves();
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
                    uint64_t move = engine->getBestMove(board);
                    cout << "Engine makes move: " + getMoveStringFromMove((move)) << endl;
                    board.makeMoveLong(board.turn, move);
                    break;
                } else if (userMove == "x") {
                    engine->setPrint(true);
                    uint64_t move = engine->getBestMove(board);
                    const int aiMoveSore = engine->getDisplayScoreOfMove(board);
                    engine->setPrint(enginePrint);
                    cout << "Engine suggests move: " + getMoveStringFromMove((move))
                         << ", and suggests this board is worth: " << aiMoveSore
                         << ", the bigger the better (for you) "
                         << endl;

                } else if (userMove == "r") {
                    EngineRandom engineRandom("random engine");
                    uint64_t move = engineRandom.getBestMove(board);
                    cout << "Random Engine makes move: " + getMoveStringFromMove((move)) << endl;
                    board.makeMoveLong(board.turn, move);
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

