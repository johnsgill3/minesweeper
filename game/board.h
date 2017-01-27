#include "square.h"
#define BDEBUG 0
class Board {
    int numRow, numCol, numBomb;
    Square ***board;
    Square **bombSquares;
    void setBomb(int, int);
    void initBoard();
    void placeBombs();
public:
    Board(int, int, int);
    bool clickSquare(int, int);
    void flagSquare(int, int);

    void printBoard(bool debug = BDEBUG);
};
