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
    cerr << "solver-" << error << endl;
    // TODO: Full "Help"
    exit(1);
}

int main (int argc, char **argv)
{
    cout.setf(ios::unitbuf);
    cerr.setf(ios::unitbuf);

    bool debug;
    int nRow, nCol, nBomb, nGames, sNum = 0;
    char c;
    nRow = nCol = nBomb = nGames = -1;
    Board *b;
    boardState_t gState = INPROGRESS;
    stringstream ss;
    string action, line, contStr = "CONTINUE";

    opterr = 0;
    while ((c = getopt (argc, argv, "r:c:b:dg:")) != -1)
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
        case 'g':
            nGames = atoi(optarg);
            break;
        default:
            usage("Unknown Arguement");
        }
    }
    if(nRow == -1 || nCol == -1 || nBomb == -1 || nGames == -1)
        usage("Missing Arguemnt");

    b = new Board(nRow, nCol, nBomb, false);

    for(int g = 1; g <= nGames; g++)
    {
        do
        {
            // Reset Input Variables
            action = ' ';

            // Get board input
            getline(cin, line);
            if(debug)
                cerr << "solver-" << "line = " << line << endl;
            ss.clear();
            ss.str(line);
            ss >> action;
            if(debug)
                cerr << "solver-" << "action = " << action << endl;
            cin >> *b;
            if(action.compare(contStr) == 0)
            {
                cout << "c " << (sNum / nRow) << " " << (sNum % nCol) << endl;
                sNum++;
            }
        } while(action.compare(contStr) == 0);

        if(g < nGames) {
            cout << "y" << endl;
            sNum = 0;
            b->reset();
        }
    }

    return 0;
}
