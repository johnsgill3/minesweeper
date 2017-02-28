#ifndef _SOLVER_H_
#define _SOLVER_H_
#include <iostream>
#include <string>
#include "utils.h"
#include "board.h"

typedef struct {
    Square *s;
    bool calculated;
    float prob;
    int numBlank;
    int numFound;
    int numUncalculated;
} squareOdds;

class Solver {
    Board *board;
    squareOdds *odds;
    void countBlanks(squareOdds *o);
    float sumCalculated(squareOdds *o);
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
