#include <iostream>
#include "square.h"

typedef enum {
    LOSS,
    INPROGRESS,
    WON
} boardState_t;

class Board {
    bool isGame;
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
    Board(int, int, int, bool g=true);
    ~Board();
    boardState_t clickSquare(int, int);
    void flagSquare(int, int);
    void reset();

    friend std::ostream& operator<<(std::ostream&, const Board&);
    friend std::istream& operator>>(std::istream&, Board&);
};
