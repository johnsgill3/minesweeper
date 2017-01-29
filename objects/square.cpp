#include "stdio.h"
#include "square.h"

Square::Square(int x, int y)
{
    bomb = flagged = visible = false;
    numBombs = 0;
    xCoord = x;
    yCoord = y;
}

void Square::setBomb() { bomb = true; numBombs = 0; }
void Square::touchingBomb() { numBombs++; }
void Square::setVisible()
{
    if(!flagged)
        visible = true;
}
void Square::toggleFlag()
{
    if(!visible)
        flagged = !flagged;
}

bool Square::isVisible() { return visible; }
bool Square::isBomb() { return bomb; }
bool Square::isFlag() { return flagged; }

int Square::getNumBombs() { return numBombs; }
int Square::getX() { return xCoord; }
int Square::getY() { return yCoord; }

std::ostream& operator<<(std::ostream& os, const Square& s)
{
    if(s.flagged)
        os << "+";
    else if(s.visible)
    {
        if(s.bomb)
            os << "*";
        else
            os << s.numBombs;
    }
    else
        os << ".";
    return os;
}
