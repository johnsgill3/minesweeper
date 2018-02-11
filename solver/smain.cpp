// Copyright 2018 John Gill
#include <string>
#include <iostream>
#include <sstream>
#include "../objects/utils.h"
#include "../objects/board.h"
#include "../objects/solver.h"

int main(int argc, char **argv) {
    std::cout.setf(std::ios::unitbuf);
    std::cerr.setf(std::ios::unitbuf);
    signal(SIGSEGV, sig_handler);


    std::stringstream ss;
    std::string action, line, contStr = "CONTINUE";

    my_getargs(argc, argv);
    if (args["nRow"] == -1 ||
       args["nCol"] == -1 ||
       args["nBomb"] == -1 ||
       args["nGames"] == -1)
        usage("Missing Arguemnt");

    if (args["debug"])
        std::cerr << "solver-" << "nRow(" << args["nRow"] << ") "
             << "nCol(" << args["nCol"] << ") "
             << "nBomb(" << args["nBomb"] << ") "
             << "nGame(" << args["nGame"] << ")\n";

    Solver s(args["nRow"], args["nCol"], args["nBomb"]);

    for (int g = 1; g <= args["nGames"]; g++) {
        do {
            // Reset Input Variables
            action = ' ';

            // Get board input
            getline(std::cin, line);
            if (args["debug"])
                std::cerr << "solver-" << "line = " << line << std::endl;
            ss.clear();
            ss.str(line);
            ss >> action;
            if (args["debug"])
                std::cerr << "solver-" << "action = " << action << std::endl;
            std::cin >> s;
            if (action.compare(contStr) == 0)
                std::cout << s.makeGuess() << std::endl;
        } while (action.compare(contStr) == 0);

        if (g < args["nGames"]) {
            std::cout << "y" << std::endl;
            s.reset();
        }
    }

    return 0;
}
