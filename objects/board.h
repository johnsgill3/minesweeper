// Copyright 2018 John Gill
#ifndef OBJECTS_BOARD_H_
#define OBJECTS_BOARD_H_
#include <iostream>
#include <vector>
#include <memory>
#include "./square.h"

typedef enum {
    LOSS,
    INPROGRESS,
    WON
} boardState_t;

template <typename T>
using vector2d = std::vector<std::vector<T>>;

template <typename T>
std::ostream &
operator<<(std::ostream & os, const vector2d<T> & v2d) {
    for (const auto & row : v2d) {
        for (const auto & e : row)
            os << e << ' ';
        if (args["debug"]) std::cerr << std::endl;
        os << std::endl;
    }
    return os;
}

class Board {
    // friend class Solver;
 private:
    bool isGame{true};
    int numRow{-1}, numCol{-1}, numBomb{-1}, numVisible{-1};
    boardState_t state;
    std::vector<std::shared_ptr<Square>> bombSquares;
    void setBomb(int, int);
    void initBoard();
    inline bool validSquare(int, int);
    boardState_t clickSquareR(int x, int y);

 protected:
    vector2d<std::shared_ptr<Square>> board;

 public:
    Board(int r, int c, int b, bool g = true) :
        numRow(r), numCol(c), numBomb(b), isGame(g) { initBoard(); }
    boardState_t clickSquare(int, int);
    void flagSquare(int, int);
    void reset() { initBoard(); }

    int getNumRow() const { return numRow; }
    int getNumCol() const { return numCol; }
    int getNumBomb() const { return numBomb; }
    int getNumVisible() const { return numVisible; }

    friend std::ostream& operator<<(std::ostream&, const Board&);
    friend std::istream& operator>>(std::istream&, Board&);
};
#endif  // OBJECTS_BOARD_H_
