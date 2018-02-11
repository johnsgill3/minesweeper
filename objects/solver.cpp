// Copyright 2018 John Gill
#include <iomanip>
#include <algorithm>
#include <random>
#include "../objects/solver.h"

///////////////////////////////////////////////////////////////////////////////
// Utility Functions
bool oddsCompare(const std::shared_ptr<SquareOdds> & a, const std::shared_ptr<SquareOdds> & b) {
    if (a->s->getNumBombs() == a->numFound)
        return false;
    else if (b->s->getNumBombs() == b->numFound)
        return true;

    if ((a->s->getNumBombs() - a->numFound) == a->numBlank)
        return false;
    else if ((b->s->getNumBombs() - b->numFound) == b->numBlank)
        return true;

    if (a->s->getNumBombs() < b->s->getNumBombs())
        return false;
    else if (a->s->getNumBombs() > b->s->getNumBombs())
        return true;

    if (a->numUncalculated < b->numUncalculated)
        return false;
    else if (a->numUncalculated > b->numUncalculated)
        return true;

    if (a->numBlank < b->numBlank)
        return false;
    else if (a->numBlank > b->numBlank)
        return true;

    return false;
}

int probCompare(const std::shared_ptr<SquareOdds> & a, const std::shared_ptr<SquareOdds> & b) {
    float diff = a->prob - b->prob;
    if (diff < 0.0)
        return -1;
    else if (diff > 0.0)
        return 1;
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Private Functions
void Solver::countBlanks(const std::shared_ptr<SquareOdds> & o) {
    int i, j, x = o->s->getX(), y = o->s->getY();
    for (i = x - 1; i <= x+1; i++) {
        if (i < 0 || i > getNumRow() - 1)
            continue;
        for (j = y - 1; j <= y+1; j++) {
            if (j < 0 || j > getNumCol() - 1 || (i == x && j == y))
                continue;
            if (!odds[i][j]->s->isVisible()) {
                if (odds[i][j]->s->isFlag())
                    odds[x][y]->numFound++;
                else
                    odds[x][y]->numBlank++;

                if (!odds[i][j]->calculated)
                    odds[x][y]->numUncalculated++;
            }
        }
    }
}

float Solver::sumCalculated(const std::shared_ptr<SquareOdds> & o) {
    float ret = 0.0;
    int i, j, x = o->s->getX(), y = o->s->getY();
    for (i = x - 1; i <= x+1; i++) {
        if (i < 0 || i > getNumRow() - 1)
            continue;
        for (j = y - 1; j <= y+1; j++) {
            if (j < 0 || j > getNumCol() - 1 || (i == x && j == y))
                continue;
            if (odds[i][j]->calculated && !odds[i][j]->s->isVisible())
                ret += odds[i][j]->prob;
        }
    }
    return ret;
}

void Solver::updateOdds() {
    int i, j, x, y;
    std::vector<std::shared_ptr<SquareOdds>> foundSquares;
    float remainingProb = getNumBomb(), currProb = 0.0;
    std::vector<std::shared_ptr<SquareOdds>>::iterator it;

    // Reset all values
    for (auto & row : odds) {
        for (auto & sq : row) {
            sq->reset();

            if (sq->s->isFlag()) {
                sq->calculated = true;
                sq->prob = 1.0;
                remainingProb -= 1.0;
            }
        }
    }

    // Calculate initial blank squares
    for (auto & row : odds) {
        for (auto & sq : row) {
            if (sq->s->isVisible()) {
                countBlanks(sq);
                if (sq->numBlank > 0)
                    foundSquares.push_back(sq);
                sq->calculated = true;
                sq->prob = -1.0;
            }
        }
    }

    while (!foundSquares.empty()) {
        // Sort the remaining squares to find next to process
        sort(foundSquares.begin(), foundSquares.end(), oddsCompare);
        // if(args["debug"])
        // {
        //     for (it=foundSquares.begin(); it!=foundSquares.end(); ++it)
        //     {
        //         cerr << "solver - post-square foundSquare ("
        //              << (*it)->s->getX() << ", "
        //              << (*it)->s->getY() << ")"
        //              << " numBombs = " << (*it)->s->getNumBombs()
        //              << " calculated = " << (*it)->calculated
        //              << " prob = " << fixed << setprecision(2) << (*it)->prob
        //              << " numBlank = " << (*it)->numBlank
        //              << " numFound = " << (*it)->numFound
        //              << " numUncalculated = " << (*it)->numUncalculated << endl;
        //     }
        // }
        // 'pop' next square off list
        auto cur = foundSquares.back();

        // Calculate Odds for ajacent blanks
        if (cur->numUncalculated > 0) {
            currProb = (cur->s->getNumBombs() - sumCalculated(cur)) / (cur->numUncalculated);
            if (currProb < 0.0) currProb = 0.0;
            if (currProb >= 1.0) currProb = 1.0;
            // If probability of remaining squares > remaining probability
            if (remainingProb - (currProb * cur->numUncalculated) < 0)
                currProb = remainingProb / cur->numUncalculated;

            // Assign to uncalculated blank ajacent squares
            x = cur->s->getX();
            y = cur->s->getY();
            for (i = x - 1; i <= x+1; i++) {
                if (i < 0 || i > getNumRow() - 1)
                    continue;
                for (j = y - 1; j <= y+1; j++) {
                    if (j < 0 || j > getNumCol() - 1 || (i == x && j == y) ||
                        odds[i][j]->s->isVisible() || odds[i][j]->calculated)
                    {
                        continue;
                    }
                    odds[i][j]->calculated = true;
                    odds[i][j]->prob = currProb;
                    remainingProb -= currProb;
                }
            }
        }
        // Remove this square from list
        foundSquares.pop_back();

        // Update stats for remaining squres
        for (it = foundSquares.begin(); it != foundSquares.end(); ++it) {
            (*it)->numFound = (*it)->numBlank = (*it)->numUncalculated = 0;
            countBlanks(*it);
        }
        std::cerr << std::endl << *this;
    }

    if (remainingProb > 0) {
        // Get list of uncalculated blank squares
        for (auto & row : odds) {
            for (auto & sq : row) {
                if (!sq->s->isVisible() && !sq->calculated) {
                    foundSquares.push_back(sq);
                }
            }
        }

        // Assign remaining probability to uncalculated blank squares
        currProb = remainingProb / static_cast<float>(foundSquares.size());
        for (it = foundSquares.begin(); it != foundSquares.end(); ++it) {
            (*it)->calculated = true;
            (*it)->prob = currProb;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Public Functions
Solver::Solver(int r, int c, int b) : Board(r, c, b, false), odds() {
    int i = 0, j = 0;
    odds.resize(getNumRow());
    for (auto & row : odds) {
        row.resize(getNumCol());
        for (auto & sq : row) {
            sq = std::make_shared<SquareOdds>();
            sq->s = board[i][j++];
        }
        i++; j = 0;
    }
}

void Solver::reset() {
    Board::reset();
    updateOdds();
}

std::string Solver::makeGuess() {
    std::vector<std::shared_ptr<SquareOdds>> tOdds;
    int gMin = 0, gMax = 0, gIdx = 0;
    std::string retStr;
    std::cerr << "solver - odds = " << std::endl << *this;
    if (args["debug"]) {
        std::cerr << "solver - odds (pre-sort):";
        for (const auto & row : odds)
            for (const auto & sq : row)
                std::cerr << " " << sq->prob;
        std::cerr << std::endl;
    }
    for (const auto & row : odds)
        for (const auto & sq : row)
            tOdds.push_back(sq);

    if (args["debug"]) {
        std::cerr << "solver - tOdds (pre-sort):";
        for (const auto & sq : tOdds)
            std::cerr << " " << sq->prob;
        std::cerr << std::endl;
    }
    sort(tOdds.begin(), tOdds.end(), probCompare);
    if (args["debug"]) {
        std::cerr << "solver - tOdds (post-sort):";
        for (const auto & sq : tOdds)
            std::cerr << " " << sq->prob;
        std::cerr << std::endl;
    }
    gIdx = tOdds.size()-1;
    while (tOdds[gIdx]->prob == 1.0 && tOdds[gIdx]->s->isFlag()) gIdx -= 1;
    if (tOdds[gIdx]->prob == 1.0 && !tOdds[gIdx]->s->isFlag()) {
        if (args["debug"])
            std::cerr << "solver - guessIdx = " << gIdx << std::endl;
        retStr = "f " + std::to_string(tOdds[gIdx]->s->getX()) + " " +
                        std::to_string(tOdds[gIdx]->s->getY());
        if (args["debug"])
            std::cerr << "solver - guessString = \"" << retStr << "\"" << std::endl;
    } else {
        while (tOdds[gMin]->prob == -1) gMin++;  // Skip displayed cells
        while (tOdds[gMin+gMax]->prob == tOdds[gMin]->prob) gMax++;  // Find end of same prob
        gMax += gMin;
        if (args["debug"])
            std::cerr << "solver - gMin = " << gMin << " gMax = " << gMax << std::endl;
        if (gMax - gMin == 1) {
            // Single point to guess
            gIdx = gMin;

        } else {
            // Multiple of same prob. Randomly choose one
            std::random_device rd;  // obtain a random number from hardware
            std::mt19937 eng(rd());  // seed the generator
            std::uniform_int_distribution<> distr(gMin, gMax-1);  // define the range

            gIdx = distr(eng);
        }
        if (args["debug"])
            std::cerr << "solver - guessIdx = " << gIdx << std::endl;
        retStr = "c " + std::to_string(tOdds[gIdx]->s->getX()) + " " +
                        std::to_string(tOdds[gIdx]->s->getY());
        if (args["debug"])
            std::cerr << "solver - guessString = \"" << retStr << "\"" << std::endl;
    }

    return retStr;
}

std::ostream& operator<<(std::ostream &os, const Solver &s) {
    for (const auto & row : s.odds) {
        for (const auto & sq : row) {
            if (sq->prob == -1.0)
                os << "X.XX ";
            else
                os << std::fixed << std::setprecision(2) << sq->prob << " ";
        }
        os << std::endl;
    }
    return os;
}

std::istream& operator>>(std::istream &is, Solver &s) {
    char c;
    for (auto & row : s.board) {
        for (auto & sq : row)
            is >> *sq;
        is.get(c);  // Consume the new line
    }
    s.updateOdds();
    return is;
}
