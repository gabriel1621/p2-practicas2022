//Gabriel Pardo - 48775081Q
#ifndef COORDINATE_H
#define COORDINATE_H

#include <iostream>

using namespace std;
enum CellState {
NONE,
SHIP,
HIT,
WATER
};


enum Orientation {
NORTH,
EAST,
SOUTH,
WEST
};

class Coordinate {
protected:
    int row;
    int column;
    CellState state;

public:
    Coordinate();
    Coordinate(int row, int column);
    Coordinate(string coord);

    int getRow() const;
    int getColumn() const;
    CellState getState() const;
    char getStateChar() const;

    void setRow(int row);
    void setColumn(int column);
    void setState(CellState state);

    bool compare(const Coordinate& coord) const;
    Coordinate addOffset(int offset, Orientation orientation) const;

    static Orientation orientationFromChar(char orientation);

    friend std::ostream& operator<<(std::ostream& os, const Coordinate& coord);
};

#endif // COORDINATE_H
