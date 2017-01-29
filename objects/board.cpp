#include <stdio.h>
#include <stdlib.h>
#include <random>
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
    std::random_device rd;

    // Iterate over remaining elements
    // Randomly replace with elements
    for(; i < n; i++)
    {
        j = rd() % (i+1);
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
                board[(i*numCol)+j]->setBomb();
            else
                board[(i*numCol)+j]->touchingBomb();
            if(BDEBUG)
                printBoard();
        }
    }
}

void Board::initBoard()
{
    int i, j;
    board = new Square*[numRow*numCol];

    for(i = 0; i < numRow; i++)
        for(j = 0; j < numCol; j++)
            board[(i*numCol)+j] = new Square(i, j);
    placeBombs();
    state = INPROGRESS;
}

void Board::finiBoard()
{
    int i, j;
    for(i = 0; i < numRow; i++)
        for(j = 0; j < numCol; j++)
            delete board[(i*numCol)+j];
    delete board;
    board = NULL;
    delete bombSquares;
    bombSquares = NULL;
}

void Board::placeBombs()
{
    bombSquares = reservoidSample(board, numRow*numCol, numBomb);
    for(int i = 0; i < numBomb; i++)
        setBomb(bombSquares[i]->getX(), bombSquares[i]->getY());
}

boardState_t Board::clickSquareR(int x, int y)
{
    if(board[(x*numCol)+y]->isVisible() ||
       board[(x*numCol)+y]->isFlag())
        return INPROGRESS;

    board[(x*numCol)+y]->setVisible();
    if(board[(x*numCol)+y]->isBomb())
        return LOSS;

    if(board[(x*numCol)+y]->getNumBombs() == 0)
    {
        int i, j;
        for(i = x - 1; i <= x+1; i++)
        {
            if(i < 0 || i > numRow - 1)
                continue;
            for(j = y - 1; j <= y+1; j++)
            {
                if(j < 0 || j > numCol - 1 || board[(i*numCol)+j]->isBomb())
                    continue;

                if(board[(i*numCol)+j]->getNumBombs() == 0)
                    clickSquareR(i, j);
                board[(i*numCol)+j]->setVisible();
            }
        }
    }

    return INPROGRESS;
}

bool Board::validSquare(int x, int y)
{
    if(x < 0 || x > numRow)
        return false;
    if(y < 0 || y > numCol)
        return false;

    return true;
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

Board::~Board()
{
    finiBoard();
}

void Board::reset()
{
    finiBoard();
    initBoard();
}

boardState_t Board::clickSquare(int x, int y)
{
    if(!validSquare(x, y))
        return INPROGRESS;

    if(state != INPROGRESS)
        return state;

    state = clickSquareR(x, y);
    int i, j;

    if(state == LOSS)
    {
        for(i = 0 ;i < numBomb; i++)
            board[(i*numCol)+j]->setVisible();
    }
    else
    {
        // check win condition
        bool allVisible = true;
        for(i = 0; i < numRow; i++)
        {
            for(j = 0; j < numCol; j++)
            {
                if(board[(i*numCol)+j]->isBomb())
                    continue;
                if(!board[(i*numCol)+j]->isVisible())
                {
                    allVisible = false;
                    break;
                }
            }
        }
        if(allVisible)
            state = WON;
    }

    return state;
}

void Board::flagSquare(int x, int y)
{
    if(!validSquare(x, y)) return;

    board[(x*numCol)+y]->toggleFlag();
}

void Board::printBoard(bool debug)
{
    int i, j;
    for(i = 0; i < numRow; i++)
    {
        for(j = 0; j < numCol; j++)
            board[(i*numCol)+j]->print();
        printf("\n");
    }
    printf("\n");

    if(debug)
    {
        for(i = 0; i < numRow; i++)
        {
            for(j = 0; j < numCol; j++)
                board[(i*numCol)+j]->print(debug);
            printf("\n");
        }
        printf("\n");
    }
}
