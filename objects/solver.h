// Copyright 2018 John Gill
#ifndef OBJECTS_SOLVER_H_
#define OBJECTS_SOLVER_H_
#include <iostream>
#include <string>
#include "./utils.h"
#include "./board.h"

class SquareOdds {
 public:
    std::shared_ptr<Square> s;
    bool calculated{false};
    float prob{0.0};
    int numBlank{0};
    int numFound{0};
    int numUncalculated{0};
    void reset() {
        calculated = false;
        prob = 0.0;
        numBlank = numFound = numUncalculated = 0;
    }
};

class Solver : Board {
 private:
    vector2d<std::shared_ptr<SquareOdds>> odds;
    void countBlanks(const std::shared_ptr<SquareOdds> &);
    float sumCalculated(const std::shared_ptr<SquareOdds> &);
    void updateOdds();

 public:
    Solver(int, int, int);

    void reset();
    std::string makeGuess();

    friend std::ostream& operator<<(std::ostream&, const Solver&);
    friend std::istream& operator>>(std::istream&, Solver&);
};
#endif  // OBJECTS_SOLVER_H_
