// Copyright 2018 John Gill
#include <random>
#include "./utils.h"
#include "../objects/board.h"

///////////////////////////////////////////////////////////////////////////////
// Utility Functions
auto reservoidSample(const vector2d<std::shared_ptr<Square>> & stream, int n, int k) {
    int i, j;
    // Create the reservoir and
    // populate with first k elements from stream
    std::vector<std::shared_ptr<Square>> reservoir(k);
    for (i = 0; i < k; i++)
        reservoir[i] = stream[(i % stream.size())][i / stream.size()];

    // Initialize Random
    std::random_device rd;

    // Iterate over remaining elements
    // Randomly replace with elements
    for (; i < n; i++) {
        j = rd() % (i+1);
        // If the randomly  picked index is smaller than k, then replace
        // the element present at the index with new element from stream
        if (j < k)
            reservoir[j] = stream[(i % stream.size())][i / stream.size()];
    }
    return std::move(reservoir);
}

///////////////////////////////////////////////////////////////////////////////
// Private Functions
void Board::setBomb(int x, int y) {
    int i, j;
    for (i = x - 1; i <= x+1; i++) {
        if (i < 0 || i > numRow - 1)
            continue;
        for (j = y - 1; j <= y+1; j++) {
            if (j < 0 || j > numCol - 1)
                continue;

            if (i == x && j == y)
                board[i][j]->setBomb();
            else
                board[i][j]->touchingBomb();
        }
    }
}

void Board::initBoard() {
    int i = 0, j = 0;
    board.resize(numRow);
    for (auto & row : board) {
        row.resize(numCol);
        for (auto & sq : row)
            sq = std::make_shared<Square>(i, j++);
        i++; j = 0;
    }

    if (isGame) {
        bombSquares = reservoidSample(board, numRow*numCol, numBomb);
        for (auto & sq : bombSquares)
            setBomb(sq->getX(), sq->getY());
    }
    state = INPROGRESS;
    numVisible = 0;
}

boardState_t Board::clickSquareR(int x, int y) {
    if (board[x][y]->isVisible() || board[x][y]->isFlag())
        return INPROGRESS;

    board[x][y]->setVisible();
    if (board[x][y]->isBomb())
        return LOSS;
    numVisible++;

    if (board[x][y]->getNumBombs() == 0) {
        int i, j;
        for (i = x - 1; i <= x+1; i++) {
            if (i < 0 || i > numRow - 1)
                continue;
            for (j = y - 1; j <= y+1; j++) {
                if (j < 0 || j > numCol - 1 || board[i][j]->isBomb())
                    continue;

                if (board[i][j]->getNumBombs() == 0)
                    clickSquareR(i, j);
                if (!board[i][j]->isVisible()) {
                    board[i][j]->setVisible();
                    numVisible++;
                }
            }
        }
    }

    return INPROGRESS;
}

inline bool Board::validSquare(int x, int y) {
    if (x < 0 || x > numRow)
        return false;
    if (y < 0 || y > numCol)
        return false;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Public Functions
boardState_t Board::clickSquare(int x, int y) {
    if (!validSquare(x, y))
        return INPROGRESS;

    if (state != INPROGRESS)
        return state;

    state = clickSquareR(x, y);

    if (state == LOSS) {
        for (int i = 0; i < numBomb; i++)
            bombSquares[i]->setVisible();
    } else if ((numVisible+numBomb) == (numRow*numCol)) {
        state = WON;
    }

    return state;
}

void Board::flagSquare(int x, int y) {
    if (!validSquare(x, y)) return;

    board[x][y]->toggleFlag();
}

std::ostream& operator<<(std::ostream& os, const Board& b) {
    os << b.board;
    return os;
}

std::istream& operator>>(std::istream& is, Board& b) {
    char c;
    for (auto & row : b.board) {
        for (auto & sq : row) {
            is >> *sq;
            if (sq->isVisible())
                b.numVisible++;
        }
        is.get(c);  // Consume the new line
    }
    return is;
}
