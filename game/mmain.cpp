#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include "board.h"

using namespace std;

void usage(string error)
{
    cerr << "game-" << error << endl;
    // TODO: Full "Help"
    exit(1);
}

int main (int argc, char **argv)
{
    cout.setf(ios::unitbuf);
    cerr.setf(ios::unitbuf);

    bool debug, interactive;
    int nRow, nCol, nBomb, x, y;
    char c, action;
    nRow = nCol = nBomb = -1;
    Board *b;
    boardState_t gState = INPROGRESS;
    stringstream ss;
    string line;

    opterr = 0;
    while ((c = getopt (argc, argv, "r:c:b:di")) != -1)
    {
        switch (c)
        {
        case 'r':
            nRow = atoi(optarg);
            break;
        case 'c':
            nCol = atoi(optarg);
            break;
        case 'b':
            nBomb = atoi(optarg);
            break;
        case 'd':
            debug = true;
            break;
        case 'i':
            interactive = true;
            break;
        default:
            usage("Unknown Arguement");
        }
    }
    if(nRow == -1 || nCol == -1 || nBomb == -1)
        usage("Missing Arguemnt");

    if(debug)
        cerr << "game-" << "nRow(" << nRow << ") nCol(" << nCol << ") nBomb(" << nBomb << ")\n";
    b = new Board(nRow, nCol, nBomb);

    do
    {
        do
        {
            // Reset Input Variables
            action = ' ';
            x = y = -1;

            // Print the current state of board
            if(interactive)
                cout << "CONTINUE" << endl << *b;
            else
                cout << *b;

            // Get user input
            getline(cin, line);
            if(debug)
                cerr << "game-" << "line = " << line << endl;
            ss.clear();
            ss.str(line);
            ss >> action;
            ss >> x;
            ss >> y;

            if(debug)
                cerr << "game-" << "Action = " << action << " x = " << x << " y = " << y << endl;

            // Perform the action
            switch(action)
            {
                case 'c':
                    gState = b->clickSquare(x, y);
                    break;
                case 'f':
                    b->flagSquare(x, y);
                    break;
                default:
                    cout << "Action can only be one of 'c' or 'f'" << endl;
            }
        } while(gState == INPROGRESS);

        cout << (gState == WON ? "WIN" : "LOSE") << endl << *b << endl;

        getline(cin, line);
        ss.clear();
        ss.str(line);
        ss >> action;
        if(action == 'y')
            b->reset();
    } while(action == 'y');

    return 0;
}
