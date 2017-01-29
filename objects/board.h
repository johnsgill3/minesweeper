#include "square.h"
#define BDEBUG 0
typedef enum {
    LOSS,
    INPROGRESS,
    WON
} boardState_t;

class Board {
    int numRow, numCol, numBomb;
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
    Board(int, int, int);
    ~Board();
    boardState_t clickSquare(int, int);
    void flagSquare(int, int);
    void reset();

    void printBoard(bool debug = BDEBUG);
};
