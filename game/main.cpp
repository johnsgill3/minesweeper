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
    cout << error << endl;
    // TODO: Full "Help"
    exit(1);
}

int main (int argc, char **argv)
{
    bool debug;
    int nRow, nCol, nBomb, x, y;
    char c, action;
    nRow = nCol = nBomb = -1;
    Board *b;
    boardState_t gState = INPROGRESS;
    stringstream ss;
    string line;

    opterr = 0;
    while ((c = getopt (argc, argv, "r:c:b:d")) != -1)
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
        default:
            usage("Unknown Arguement");
        }
    }
    if(nRow == -1 || nCol == -1 || nBomb == -1)
        usage("Missing Arguemnt");

    cout << "nRow(" << nRow << ") nCol(" << nCol << ") nBomb(" << nBomb << ")\n";
    b = new Board(nRow, nCol, nBomb);

    do
    {
        // Reset Input Variables
        action = ' ';
        x = y = -1;

        // Print the current state of board
        cout << *b << endl;
        
        // Get user input
        getline(cin, line);
        ss.clear();
        ss.str(line);
        ss >> action;
        ss >> x;
        ss >> y;

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

    cout << *b << endl;
    cout << (gState == WON ? "WIN" : "LOSE") << endl;

    return 0;
}
