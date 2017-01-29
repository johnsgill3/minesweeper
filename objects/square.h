#include <iostream>
class Square {
    bool visible, bomb, flagged;
    int numBombs, xCoord, yCoord;
public:
    Square(int, int);
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
};
