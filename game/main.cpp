#include "stdio.h"
#include "board.h"

int main()
{
    Board *b = new Board(10, 10, 10);

    for(int i = 0; i < 2; i++)
    {
        b->printBoard(1);
        b->clickSquare(3, 0);
        b->printBoard(1);
        b->clickSquare(0, 3);
        b->printBoard(1);
        b->flagSquare(0, 0);
        b->printBoard(1);
        printf("\nRESET\n");
        b->reset();
    }

    return 0;
}
