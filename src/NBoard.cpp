//
// Created by louis on 6/26/19.
//

#include <iostream>
#include <cstring>
#include <string>

#include <boost/tokenizer.hpp>
#include <ncurses.h>
#include <regex>
#include <bitset>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <math.h>
#include <jmorecfg.h>
#include "Art.h"
#include "BitBoardUtils.h"
#include "NBoard.h"
#include "MoveUtils.h"
#include "Move.h"
#include "main.h"
#include "Board.h"

#include <cstdint>

using namespace boost;

using namespace std;

void parseBoardCommand(string token, Board *board);

vector<string> split(const string &str, const string &delim) {
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

void mainLoopNBoard() {
    cout << "starting main nBoard loop " << endl;

    string cmd, c;
    Board board;
    parseBoardCommand(
            "set game (;GM[Othello]PC[NBoard]DT[2019-06-27 15:34:50 GMT]PB[Ntest4]PW[louis]RE[?]TI[5:00]TY[8]BO[8 ---------------------------O*------*O--------------------------- *]B[D3/-1.34]W[C5//2.029]B[E6/-1.30]W[F5//5.09]B[F6/-0.51]W[D7//14.009]B[F4/1.68]W[E3//0.75]B[F3/3.11]W[E2//1.177]B[C4/4.08]W[G6//2.2]B[B6/4.79]W[A7//2.104]B[F1/12.66]W[C3//2.207]B[D6/15.17]W[G3//4.644]B[C6/19.94]W[D1//1.127]B[E7/24.15]W[D8//7.094]B[F7/26.12]W[G8//2.797]B[F2/28.44]W[C7//2.949]B[H3/30.83]W[G4//2.528]B[G5/29.58]W[H5//2.6550000000000002]B[H6/34.26]W[H7//1.151]B[E1/33.43]W[G1//1.608]B[H4/31.11]W[H2//2.125]B[C2/33.32]W[C1//2.235]B[D2/51.85]W[G7//3.251]B[F8/57.82]W[E8//1.7910000000000001]B[H8/59.42]W[PA//3.142]B[H1/61.54]W[PA//1.162]B[B1/59.85]W[B2//1.611]B[A1/64.00]W[B3//1.879]B[G2/62.00/0.01]W[PA//1.61]B[A3/62.00]W[A2//1.387]B[B4/62.00]W[A4//3.108]B[B5/62.00]W[PA//0.886]B[A6/62.00]W[A5//0.405]B[B7/62.00]W[C8//1.089]B[A8/62.00]W[PA//0.417];)",
            &board);

//    printBoard(board);
    return;

    while (true) {
        std::getline(cin, cmd);

        cout << "read cmd: " << cmd << endl;

        const vector<string> &tokens = split(cmd, " ");


        for (int i = 0; i < tokens.size(); i++) {
            cout << tokens[i] << endl;
        }

        c = tokens[0];

        if (c == "nboard") {

        } else if (c == "hint") {
            cout << "not implemented yet " << endl;
        } else if (c == "learn") {
            cout << "not implemented yet " << endl;

        } else if (c == "analyse") {
            cout << "not implemented yet " << endl;

        } else if (c == "go") {
            cout << "not implemented yet " << endl;

        } else if (c == "ping") {
            cout << "pong " << tokens[1] << endl;
        } else if (c == "move") {
            cout << "not implemented yet " << endl;

        } else if (c == "set game") {
            parseBoardCommand(tokens[1], &board);
            cout << "not implemented yet " << endl;

        } else if (c == "set contempt") {

            cout << "not implemented yet " << endl;
        } else if (c == "set depth") {

            cout << "not implemented yet " << endl;
        } else if (c == "quit") {
            cout << "received quit command, quitting" << endl;
            return;
        }
    }

}
void parseBoardCommand(string token, Board *board){
    cout << token << endl <<endl<<endl;
    /*
set game (;GM[Othello]PC[NBoard]DT[2019-06-27 15:34:50 GMT]PB[Ntest4]PW[louis]RE[?]TI[5:00]TY[8]BO[8 ---------------------------O*------*O--------------------------- *]B[D3/-1.34]W[C5//2.029]B[E6/-1.30]W[F5//5.09]B[F6/-0.51]W[D7//14.009]B[F4/1.68]W[E3//0.75]B[F3/3.11]W[E2//1.177]B[C4/4.08]W[G6//2.2]B[B6/4.79]W[A7//2.104]B[F1/12.66]W[C3//2.207]B[D6/15.17]W[G3//4.644]B[C6/19.94]W[D1//1.127]B[E7/24.15]W[D8//7.094]B[F7/26.12]W[G8//2.797]B[F2/28.44]W[C7//2.949]B[H3/30.83]W[G4//2.528]B[G5/29.58]W[H5//2.6550000000000002]B[H6/34.26]W[H7//1.151]B[E1/33.43]W[G1//1.608]B[H4/31.11]W[H2//2.125]B[C2/33.32]W[C1//2.235]B[D2/51.85]W[G7//3.251]B[F8/57.82]W[E8//1.7910000000000001]B[H8/59.42]W[PA//3.142]B[H1/61.54]W[PA//1.162]B[B1/59.85]W[B2//1.611]B[A1/64.00]W[B3//1.879]B[G2/62.00/0.01]W[PA//1.61]B[A3/62.00]W[A2//1.387]B[B4/62.00]W[A4//3.108]B[B5/62.00]W[PA//0.886]B[A6/62.00]W[A5//0.405]B[B7/62.00]W[C8//1.089]B[A8/62.00]W[PA//0.417];)
*/
    std::string subject = token;
    std::string result;
    try {
        std::regex re("\\s\\*\\](([BW]\\[[ABCDEFGHP][12345678A]/(?:\\d+\\.\\d+)?/?-?\\d+\\.\\d+\\])+)");
        std::smatch match;
        if (std::regex_search(subject, match, re) && match.size() > 1) {
            result = match.str(1);
            cout << result << endl;
        } else {
            result = std::string("");
        }
    } catch (std::regex_error& e) {
        // Syntax error in the regular expression
    }
}