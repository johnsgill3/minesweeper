#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>
#include "solver.h"
#include <stdlib.h>

using namespace std;
///////////////////////////////////////////////////////////////////////////////
// Utility Functions
bool oddsCompare(squareOdds *a, squareOdds *b)
{
    if(a->s->getNumBombs() == a->numFound)
        return false;
    else if(b->s->getNumBombs() == b->numFound)
        return true;

    if(a->s->getNumBombs() - a->numFound == a->numBlank)
        return false;
    else if(b->s->getNumBombs() - b->numFound == b->numBlank)
        return true;

    if(a->s->getNumBombs() < b->s->getNumBombs())
        return false;
    else if(a->s->getNumBombs() > b->s->getNumBombs())
        return true;

    if(a->numUncalculated < b->numUncalculated)
        return false;
    else if(a->numUncalculated > b->numUncalculated)
        return true;

    if(a->numBlank < b->numBlank)
        return false;
    else if(a->numBlank > b->numBlank)
        return true;

    return false;
}

int probCompare(const void *a, const void *b)
{
    float diff = ((squareOdds *)a)->prob - ((squareOdds *)b)->prob;
    if(diff < 0.0)
        return -1;
    else if(diff > 0.0)
        return 1;
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Private Functions
void Solver::countBlanks(squareOdds *o)
{
    int i, j, x = o->s->getX(), y = o->s->getY();
    for(i = x - 1; i <= x+1; i++)
    {
        if(i < 0 || i > board->getNumRow() - 1)
            continue;
        for(j = y - 1; j <= y+1; j++)
        {
            if(j < 0 || j > board->getNumCol() - 1 || (i == x && j == y))
                continue;
            if(!odds[(i*board->getNumRow())+j].s->isVisible())
            {
                if(odds[(i*board->getNumRow())+j].s->isFlag())
                    odds[(x*board->getNumRow())+y].numFound++;
                else
                    odds[(x*board->getNumRow())+y].numBlank++;

                if(!odds[(i*board->getNumRow())+j].calculated)
                    odds[(x*board->getNumRow())+y].numUncalculated++;
            }
        }
    }
}

float Solver::sumCalculated(squareOdds *o)
{
    float ret = 0.0;
    int i, j, x = o->s->getX(), y = o->s->getY();
    for(i = x - 1; i <= x+1; i++)
    {
        if(i < 0 || i > board->getNumRow() - 1)
            continue;
        for(j = y - 1; j <= y+1; j++)
        {
            if(j < 0 || j > board->getNumCol() - 1 || (i == x && j == y))
                continue;
            if(odds[(i*board->getNumRow())+j].calculated &&
               !odds[(i*board->getNumRow())+j].s->isVisible())
                ret += odds[(i*board->getNumRow())+j].prob;
        }
    }
    return ret;
}

void Solver::updateOdds()
{
    int i, j, x, y;
    Square *s;
    vector<squareOdds *> foundSquares;
    float remainingProb = board->getNumBomb(), currProb;
    vector<squareOdds *>::iterator it;

    // Reset all values
    for(i = 0; i < board->getNumRow(); i++)
    {
        for(j = 0; j < board->getNumCol(); j++)
        {
            odds[(i*board->getNumRow())+j].prob = 0.0;
            odds[(i*board->getNumRow())+j].calculated = false;
            odds[(i*board->getNumRow())+j].numBlank = 0;
            odds[(i*board->getNumRow())+j].numFound = 0;
            odds[(i*board->getNumRow())+j].numUncalculated = 0;

            if(odds[(i*board->getNumRow())+j].s->isFlag())
            {
                odds[(i*board->getNumRow())+j].calculated = true;
                odds[(i*board->getNumRow())+j].prob = 1.0;
                remainingProb -= 1.0;
            }
        }
    }

    // Calculate initial blank squares
    for(i = 0; i < board->getNumRow(); i++)
    {
        for(j = 0; j < board->getNumCol(); j++)
        {
            if(odds[(i*board->getNumRow())+j].s->isVisible())
            {
                countBlanks(&odds[(i*board->getNumRow())+j]);
                if(odds[(i*board->getNumRow())+j].numBlank > 0)
                    foundSquares.push_back(&odds[(i*board->getNumRow())+j]);
                odds[(i*board->getNumRow())+j].calculated = true;
                odds[(i*board->getNumRow())+j].prob = -1.0;
            }
        }
    }

    while(!foundSquares.empty())
    {
        // Sort the remaining squares to find next to process
        sort(foundSquares.begin(), foundSquares.end(), oddsCompare);
        // if(args["debug"])
        // {
        //     for (it=foundSquares.begin(); it!=foundSquares.end(); ++it)
        //     {
        //         cerr << "solver - post-square foundSquare (" << (*it)->s->getX() << ", " << (*it)->s->getY() << ")"
        //              << " numBombs = " << (*it)->s->getNumBombs()
        //              << " calculated = " << (*it)->calculated
        //              << " prob = " << fixed << setprecision(2) << (*it)->prob
        //              << " numBlank = " << (*it)->numBlank
        //              << " numFound = " << (*it)->numFound
        //              << " numUncalculated = " << (*it)->numUncalculated << endl;
        //     }
        // }
        // 'pop' next square off list
        squareOdds *cur = foundSquares.back();

        // Calculate Odds for ajacent blanks
        if(cur->numUncalculated > 0)
        {

            currProb = (cur->s->getNumBombs() - sumCalculated(cur)) / (cur->numUncalculated);
            if(currProb < 0.0) currProb = 0.0;
            if(currProb >= 1.0) currProb = 1.0;
            //   If probability of remaining squares > remaining probability
            if(remainingProb - (currProb * cur->numUncalculated) < 0)
                currProb = remainingProb / cur->numUncalculated;

            // Assign to uncalculated blank ajacent squares
            x = cur->s->getX();
            y = cur->s->getY();
            for(i = x - 1; i <= x+1; i++)
            {
                if(i < 0 || i > board->getNumRow() - 1)
                    continue;
                for(j = y - 1; j <= y+1; j++)
                {
                    if(j < 0 ||
                       j > board->getNumCol() - 1 ||
                       (i == x && j == y) ||
                       odds[(i*board->getNumRow())+j].s->isVisible() ||
                       odds[(i*board->getNumRow())+j].calculated)
                    {
                        continue;
                    }
                    odds[(i*board->getNumRow())+j].calculated = true;
                    odds[(i*board->getNumRow())+j].prob = currProb;
                    remainingProb -= currProb;
                }
            }
            // cur->calculated = true;
            // cur->prob = -1;
        }
        // Remove this square from list
        foundSquares.pop_back();

        // Update stats for remaining squres
        for (it=foundSquares.begin(); it!=foundSquares.end(); ++it)
        {
            (*it)->numFound = (*it)->numBlank = (*it)->numUncalculated = 0;
            countBlanks(*it);
        }
        cerr << endl << *this;
    }

    if(remainingProb > 0)
    {
        // Get list of uncalculated blank squares
        for(i = 0; i < board->getNumRow(); i++)
        {
            for(j = 0; j < board->getNumCol(); j++)
            {
                if(!odds[(i*board->getNumRow())+j].s->isVisible() &&
                   !odds[(i*board->getNumRow())+j].calculated)
                {
                    foundSquares.push_back(&odds[(i*board->getNumRow())+j]);
                }
            }
        }

        // Assign remaining probability to uncalculated blank squares
        currProb = remainingProb / (float)foundSquares.size();
        for (it=foundSquares.begin(); it!=foundSquares.end(); ++it)
        {
            (*it)->calculated = true;
            (*it)->prob = currProb;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Public Functions
Solver::Solver(int r, int c, int b)
{
    board = new Board(r, c, b, false);
    odds = new squareOdds[board->getNumRow()*board->getNumCol()];
    int i, j;
    Square *s;
    for(i = 0; i < board->getNumRow(); i++)
        for(j = 0; j < board->getNumCol(); j++)
            odds[(i*board->getNumRow())+j].s = board->board[(i*board->getNumRow())+j];
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
    squareOdds *tOdds;
    int numOdds = board->getNumRow()*board->getNumCol(),
        gMin = 0, gMax = 0, gIdx = 0;
    tOdds = new squareOdds[numOdds];
    string retStr;
    cerr << "solver - odds = " << endl << *this;

    memcpy(tOdds, odds, sizeof(squareOdds)*(numOdds));
    qsort(tOdds, numOdds, sizeof(squareOdds), probCompare);
    if(args["debug"])
    {
        cerr << "solver - tOdds:";
        for(int i = 0; i < numOdds; i++)
            cerr << " " << tOdds[i].prob;
        cerr << endl;
    }
    gIdx = numOdds-1;
    while(tOdds[gIdx].prob == 1.0 && tOdds[gIdx].s->isFlag()) gIdx -= 1;
    if(tOdds[gIdx].prob == 1.0 && !tOdds[gIdx].s->isFlag())
    {
        if(args["debug"]) cerr << "solver - guessIdx = " << gIdx << endl;
        retStr = "f " + to_string(tOdds[gIdx].s->getX()) + " " + to_string(tOdds[gIdx].s->getY());
        if(args["debug"]) cerr << "solver - guessString = \"" << retStr << "\"" << endl;
    }
    else
    {
        while(tOdds[gMin].prob == -1) gMin++; // Skip displayed cells
        while(tOdds[gMin+gMax].prob == tOdds[gMin].prob) gMax++; // Find end of same prob
        gMax += gMin;
        if(args["debug"]) cerr << "solver - gMin = " << gMin << " gMax = " << gMax << endl;
        if(gMax - gMin == 1) // Single point to guess
        {
            gIdx = gMin;
        }
        else // Multiple of same prob. Randomly choose one
        {
            random_device rd; // obtain a random number from hardware
            mt19937 eng(rd()); // seed the generator
            uniform_int_distribution<> distr(gMin, gMax-1); // define the range

            gIdx = distr(eng);
        }
        if(args["debug"]) cerr << "solver - guessIdx = " << gIdx << endl;
        retStr = "c " + to_string(tOdds[gIdx].s->getX()) + " " + to_string(tOdds[gIdx].s->getY());
        if(args["debug"]) cerr << "solver - guessString = \"" << retStr << "\"" << endl;
    }

    // Delete the temporary sorted copy
    delete tOdds;

    return retStr;
}

ostream& operator<<(ostream &os, const Solver &s)
{
    int i, j;
    for(i = 0; i < s.board->getNumRow(); i++)
    {
        for(j = 0; j < s.board->getNumCol(); j++)
        {
            if(s.odds[(i*s.board->getNumRow())+j].prob == -1.0)
                os << "X.XX ";
            else
                os << fixed << setprecision(2) << s.odds[(i*s.board->getNumRow())+j].prob << " ";
        }
        os << endl;
    }
    return os;
}

istream& operator>>(istream &is, Solver &s)
{
    is >> *s.board;
    s.updateOdds();
    return is;
}
