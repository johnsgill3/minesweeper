#include <string>
#include <iostream>
#include <sstream>
#include "utils.h"
#include "board.h"

using namespace std;

int main (int argc, char **argv)
{
    cout.setf(ios::unitbuf);
    cerr.setf(ios::unitbuf);

    int x, y;
    char action;
    Board *b;
    boardState_t gState = INPROGRESS;
    stringstream ss;
    string line;

    my_getargs(argc, argv);
    if(args["nRow"] == -1 ||
       args["nCol"] == -1 ||
       args["nBomb"] == -1)
        usage("Missing Arguemnt");

    if(args["debug"])
        cerr << "game-" << "nRow(" << args["nRow"] << ") "
             << "nCol(" << args["nCol"] << ") "
             << "nBomb(" << args["nBomb"] << ")\n";
    b = new Board(args["nRow"], args["nCol"], args["nBomb"]);

    do
    {
        do
        {
            // Reset Input Variables
            action = ' ';
            x = y = -1;

            // Print the current state of board
            if(args["interactive"])
                cout << "CONTINUE" << endl << *b;
            else
                cout << *b;

            // Get user input
            getline(cin, line);
            if(args["debug"])
                cerr << "game-" << "line = " << line << endl;
            ss.clear();
            ss.str(line);
            ss >> action;
            ss >> x;
            ss >> y;

            if(args["debug"])
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

        cout << (gState == WON ? "WIN" : "LOSE") << endl << *b;

        getline(cin, line);
        ss.clear();
        ss.str(line);
        ss >> action;
        if(action == 'y')
            b->reset();
    } while(action == 'y');

    return 0;
}
