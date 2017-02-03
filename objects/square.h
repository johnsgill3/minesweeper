#ifndef _SQUARE_H_
#define _SQUARE_H_
#include <iostream>
class Square {
    bool visible, bomb, flagged, isGame;
    int numBombs, xCoord, yCoord;
public:
    Square(int, int, bool g = true);
    void setBomb();
    void touchingBomb();
    void setVisible();
    void toggleFlag();

    bool isVisible();
    bool isBomb();
    bool isFlag();

    int getNumBombs();
    int getX();
    int getY();

    friend std::ostream& operator<<(std::ostream&, const Square&);
    friend std::istream& operator>>(std::istream&, Square&);
};
#endif
