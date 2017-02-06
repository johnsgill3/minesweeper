#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include "board.h"
#include "solver.h"

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
    int nRow, nCol, nBomb, nGames;
    char c;
    nRow = nCol = nBomb = nGames = -1;
    Solver *s;
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

    s = new Solver(nRow, nCol, nBomb);

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
            cin >> *s;
            if(action.compare(contStr) == 0)
                cout << s->makeGuess() << endl;
        } while(action.compare(contStr) == 0);

        if(g < nGames) {
            cout << "y" << endl;
            s->reset();
        }
    }

    return 0;
}
