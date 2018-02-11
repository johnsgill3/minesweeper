// Copyright 2018 John Gill
#ifndef OBJECTS_SQUARE_H_
#define OBJECTS_SQUARE_H_
#include <iostream>

class Square {
 private:
    bool visible{false}, bomb{false}, flagged{false};
    int numBombs{0}, xCoord{-1}, yCoord{-1};

 public:
    Square(int x, int y) :
        xCoord(x), yCoord(y) { }
    void setBomb() { bomb = true; numBombs = 0 ; }
    void touchingBomb() { numBombs++; }
    void setVisible() { if (!flagged) visible = true; }
    void toggleFlag() {if(!visible) flagged = !flagged; }

    bool isVisible() const  { return visible; }
    bool isBomb() const { return bomb; }
    bool isFlag() const { return flagged; }

    int getNumBombs() const { return numBombs; }
    int getX() const { return xCoord; }
    int getY() const { return yCoord; }

    friend std::ostream& operator<<(std::ostream& os, const Square & s);
    friend std::istream& operator>>(std::istream& is, Square& s);
};
#endif  // OBJECTS_SQUARE_H_
