// Copyright 2018 John Gill
#include <string>
#include <iostream>
#include <sstream>
#include "../objects/utils.h"
#include "../objects/board.h"

int main(int argc, char **argv) {
    std::cout.setf(std::ios::unitbuf);
    std::cerr.setf(std::ios::unitbuf);
    signal(SIGSEGV, sig_handler);

    int x, y;
    char action;

    boardState_t gState = INPROGRESS;
    std::stringstream ss;
    std::string line;

    my_getargs(argc, argv);
    if (args["nRow"] == -1 ||
       args["nCol"] == -1 ||
       args["nBomb"] == -1)
        usage("Missing Arguemnt");

    if (args["debug"])
        std::cerr << "game-" << "nRow(" << args["nRow"] << ") "
             << "nCol(" << args["nCol"] << ") "
             << "nBomb(" << args["nBomb"] << ")\n";
    Board b(args["nRow"], args["nCol"], args["nBomb"]);

    // args["hardcode"] = 1;
    // ss.clear(); ss.str("232\n***\n232");
    // ss >> *b;

    do {
        do {
            // Reset Input Variables
            action = ' ';
            x = y = -1;

            // Print the current state of board
            if (args["interactive"])
                std::cout << "CONTINUE" << std::endl << b;
            else
                std::cout << b;

            // Get user input
            getline(std::cin, line);
            if (args["debug"])
                std::cerr << "game-" << "line = " << line << std::endl;
            ss.clear();
            ss.str(line);
            ss >> action;
            ss >> x;
            ss >> y;

            if (args["debug"])
                std::cerr << "game-" << "Action = " << action <<
                             " x = " << x << " y = " << y << std::endl;

            // Perform the action
            switch (action) {
                case 'c':
                    gState = b.clickSquare(x, y);
                    break;
                case 'f':
                    b.flagSquare(x, y);
                    break;
                default:
                    std::cout << "Action can only be one of 'c' or 'f'" << std::endl;
            }
        } while (gState == INPROGRESS);

        std::cout << (gState == WON ? "WIN" : "LOSE") << std::endl << b;
        if (args["debug"])
            std::cerr << (gState == WON ? "WIN" : "LOSE") << std::endl;

        getline(std::cin, line);
        ss.clear();
        ss.str(line);
        ss >> action;
        if (action == 'y')
            b.reset();
    } while (action == 'y');

    return 0;
}
