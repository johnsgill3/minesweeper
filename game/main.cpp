#include "stdio.h"
#include "board.h"

int main()
{
    int r, c, i, j;
    r = c = 7;
    Board b(9, 9, 10);

    b.printBoard(1);
    b.clickSquare(3, 0);
    b.printBoard(1);
    b.clickSquare(0, 3);
    b.printBoard(1);
    b.flagSquare(0, 0);
    b.printBoard(1);

    return 0;
}
