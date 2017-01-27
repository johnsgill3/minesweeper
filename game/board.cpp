#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"
///////////////////////////////////////////////////////////////////////////////
// Utility Functions
Square **reservoidSample(Square **stream, int n, int k)
{
    int i, j;
    // Create the reservoir and
    // poopulate with first k elements from stream
    Square **reservoir = new Square*[k];
    for(i = 0; i < k; i++)
        reservoir[i] = stream[i];

    // Initialize Random
    srand(time(NULL));

    // Iterate over remaining elements
    // Randomly replace with elements
    for(; i < n; i++)
    {
        j = rand() % (i+1);
        // If the randomly  picked index is smaller than k, then replace
        // the element present at the index with new element from stream
        if (j < k)
            reservoir[j] = stream[i];
    }
    return reservoir;
}

///////////////////////////////////////////////////////////////////////////////
// Private Functions
void Board::setBomb(int x, int y)
{
    int i, j;
    if(BDEBUG)
        printf("(%d, %d)\n", x, y);
    for(i = x - 1; i <= x+1; i++)
    {
        if(i < 0 || i > numRow - 1)
            continue;
        for(j = y - 1; j <= y+1; j++)
        {
            if(j < 0 || j > numCol - 1)
                continue;
            if(BDEBUG)
                printf("    (%d, %d)\n", i, j);
            if(i == x && j == y)
                board[i][j]->setBomb();
            else
                board[i][j]->touchingBomb();
            if(BDEBUG)
                printBoard();
        }
    }
}

void Board::initBoard()
{
    int i, j;
    board = new Square**[numRow];
    allSquares = new Square*[numRow*numCol];

    for(i = 0; i < numRow; i++)
    {
        board[i] = new Square*[numCol];
        for(j = 0; j < numCol; j++)
        {
            board[i][j] = new Square(i, j);
            allSquares[(i*numCol)+j] = board[i][j];
        }
    }
    placeBombs();
}

void Board::placeBombs()
{
    bombSquares = reservoidSample(allSquares, numRow*numCol, numBomb);
    for(int i = 0; i < numBomb; i++)
        setBomb(bombSquares[i]->getX(), bombSquares[i]->getY());
}

///////////////////////////////////////////////////////////////////////////////
// Public Functions
Board::Board(int r, int c, int b)
{
    numRow = r;
    numCol = c;
    numBomb = b;
    initBoard();
}

bool Board::clickSquare(int x, int y)
{
    if(board[x][y]->isVisible())
        return false;

    board[x][y]->setVisible();
    if(board[x][y]->isBomb())
        return true;

    if(board[x][y]->getNumBombs() == 0)
    {
        int i, j;
        for(i = x - 1; i <= x+1; i++)
        {
            if(i < 0 || i > numRow - 1)
                continue;
            for(j = y - 1; j <= y+1; j++)
            {
                if(j < 0 || j > numCol - 1 || board[i][j]->isBomb())
                    continue;

                if(board[i][j]->getNumBombs() == 0)
                    clickSquare(i, j);
                board[i][j]->setVisible();
            }
        }
    }

    return false;
}

void Board::flagSquare(int x, int y)
{
    board[x][y]->toggleFlag();
}

void Board::printBoard(bool debug)
{
    int i, j;
    for(i = 0; i < numRow; i++)
    {
        for(j = 0; j < numCol; j++)
        {
            board[i][j]->print();
        }
        printf("\n");
    }
    printf("\n");

    if(debug)
    {
        for(i = 0; i < numRow; i++)
        {
            for(j = 0; j < numCol; j++)
            {
                board[i][j]->print(debug);
            }
            printf("\n");
        }
        printf("\n");
    }
}

/* TODO:
 * 2. Win Conditions
 * 3. Lose Conditions
 * 4. ?
 */
