#include "stdio.h"
#include "square.h"
using namespace std;

Square::Square(int x, int y, bool g)
{
    bomb = flagged = visible = false;
    numBombs = 0;
    xCoord = x;
    yCoord = y;
    isGame = g;
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

ostream& operator<<(ostream& os, const Square& s)
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

istream& operator>>(istream& is, Square& s)
{
    char c;
    is.get(c);
    cerr << "Got character - '" << c << "'" << endl;
    switch (c)
    {
        case '+':
            s.flagged = true;
            break;
        case '*':
            s.bomb = true;
            break;
        case '.': break; // Do nothing, unknown square
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            s.numBombs = c - 0x30;
            s.visible = true;
            break;
        default:
            cerr << "Unknown Square Value - '" << c << "'" << endl;
            exit(1);
            break;
    }
    return is;
}
