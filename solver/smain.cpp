#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include "utils.h"
#include "board.h"
#include "solver.h"

using namespace std;

int main (int argc, char **argv)
{
    cout.setf(ios::unitbuf);
    cerr.setf(ios::unitbuf);
    signal(SIGSEGV, sig_handler);

    Solver *s;
    stringstream ss;
    string action, line, contStr = "CONTINUE";

    my_getargs(argc, argv);
    if(args["nRow"] == -1 ||
       args["nCol"] == -1 ||
       args["nBomb"] == -1 ||
       args["nGames"] == -1)
        usage("Missing Arguemnt");

    if(args["debug"])
        cerr << "solver-" << "nRow(" << args["nRow"] << ") "
             << "nCol(" << args["nCol"] << ") "
             << "nBomb(" << args["nBomb"] << ") "
             << "nGame(" << args["nGame"] << ")\n";

    s = new Solver(args["nRow"], args["nCol"], args["nBomb"]);

    for(int g = 1; g <= args["nGames"]; g++)
    {
        do
        {
            // Reset Input Variables
            action = ' ';

            // Get board input
            getline(cin, line);
            if(args["debug"])
                cerr << "solver-" << "line = " << line << endl;
            ss.clear();
            ss.str(line);
            ss >> action;
            if(args["debug"])
                cerr << "solver-" << "action = " << action << endl;
            cin >> *s;
            if(action.compare(contStr) == 0)
                cout << s->makeGuess() << endl;
        } while(action.compare(contStr) == 0);

        if(g < args["nGames"]) {
            cout << "y" << endl;
            s->reset();
        }
    }

    return 0;
}
