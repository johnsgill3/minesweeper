#include <iostream>
#include <iomanip>
#include "solver.h"

using namespace std;

void Solver::updateOdds()
{
    int i, j;
    float baseOdds = ((float)board->numBomb) / ((board->numRow * board->numCol) - board->numVisible);

    for(i = 0; i < board->numRow; i++)
    {
        for(j = 0; j < board->numCol; j++)
        {
            if(board->board[(i*board->numCol)+j]->isVisible())
                odds[(i*board->numCol)+j] = 0.0;
            else
                odds[(i*board->numCol)+j] = baseOdds;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Public Functions
Solver::Solver(int r, int c, int b)
{
    board = new Board(r, c, b, false);
    odds = new float[board->numRow*board->numCol];
}

Solver::~Solver()
{
    delete board;
    delete odds;
}

void Solver::reset()
{
    board->reset();
    updateOdds();
}

string Solver::makeGuess()
{
    static int gCnt = 0;
    string retStr;
    cerr << "odds" << endl << *this;
    retStr = "c " + to_string(gCnt / board->numRow) + " " + to_string((gCnt) % board->numCol);
    gCnt++;
    return retStr;
}

ostream& operator<<(ostream &os, const Solver &s)
{
    int i, j;
    for(i = 0; i < s.board->getNumRow(); i++)
    {
        for(j = 0; j < s.board->getNumCol(); j++)
        {
            os << fixed << setprecision(2) << s.odds[(i*s.board->getNumRow())+j] << " ";
        }
        os << endl;
    }
    return os;
}

istream& operator>>(istream &is, Solver &s)
{
    s.board->reset();
    is >> *s.board;
    s.updateOdds();
    return is;
}
