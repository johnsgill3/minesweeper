// Copyright 2018 John Gill
#include "./utils.h"
#include "../objects/square.h"

std::ostream& operator<<(std::ostream& os, const Square& s) {
    if (args["debug"]) {
        if (s.isBomb())
            std::cerr << "*";
        else
            std::cerr << s.getNumBombs();
    }

    if (s.isFlag()) {
        os << "+";
    } else if (s.isVisible()) {
        if (s.isBomb())
            os << "*";
        else
            os << s.getNumBombs();
    } else {
        os << ".";
    }

    return os;
}

std::istream& operator>>(std::istream& is, Square& s) {
    char c;
    is.get(c);
    if (args["debug"])
        std::cerr << "square - character = '" << c << "'" << std::endl;
    s.bomb = s.flagged = s.visible = false;
    switch (c) {
        case '+':
            s.flagged = true;
            break;
        case '*':
            s.bomb = true;
            break;
        case '.': break;  // Do nothing, unknown square
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
            s.visible = !args["hardcode"];
            break;
        default:
            std::cerr << "Unknown Square Value - '" << c << "'" << std::endl;
            exit(1);
            break;
    }
    return is;
}
