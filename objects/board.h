#ifndef _BOARD_H_
#define _BOARD_H_
#include <iostream>
#include "utils.h"
#include "square.h"

typedef enum {
    LOSS,
    INPROGRESS,
    WON
} boardState_t;

class Board {
    friend class Solver;
    bool isGame;
    int numRow, numCol, numBomb, numVisible;
    boardState_t state;
    Square **board;
    Square **bombSquares;
    void setBomb(int, int);
    void initBoard();
    void finiBoard();
    void placeBombs();
    bool validSquare(int,int);
    boardState_t clickSquareR(int x, int y);
public:
    Board(int, int, int, bool g=true);
    ~Board();
    boardState_t clickSquare(int, int);
    void flagSquare(int, int);
    void reset();

    int getNumRow();
    int getNumCol();
    int getNumBomb();
    int getNumVisible();

    friend std::ostream& operator<<(std::ostream&, const Board&);
    friend std::istream& operator>>(std::istream&, Board&);
};
#endif
