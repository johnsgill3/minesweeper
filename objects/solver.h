#ifndef _SOLVER_H_
#define _SOLVER_H_
#include <iostream>
#include <string>
#include "utils.h"
#include "board.h"

class Solver {
    Board *board;
    float *odds;
    void updateOdds();
public:
    Solver(int, int, int);
    ~Solver();

    void reset();
    std::string makeGuess();

    friend std::ostream& operator<<(std::ostream&, const Solver&);
    friend std::istream& operator>>(std::istream&, Solver&);
};
#endif
