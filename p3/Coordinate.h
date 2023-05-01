//Gabriel Pardo - 48775081Q
#ifndef _Coordinate_h
#define _Coordinate_h

#include <string>
#include "Ship.h"
#include "Player.h"
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




class Coordinate{
    friend ostream& operator<<(ostream &os,const Coordinate &coordinate );
    protected:
        int row;
        int column;
        CellState state;
        vector<Coordinate*> positions;
        
    public:
        Coordinate();
        Coordinate(int row, int column);
        Coordinate(string coord);
        int getRow() const;
        int getColumn() const;
        CellState getState() const;
        char getStateChar() const;
        void setRow(int row);
        void setColun(int column);
        void setState(CellState state);
        bool compare(const Coordinate &coord) const;
        Coordinate addOffset(int offset, Orientation orientation) const;
        static Orientation orientationFromChar(char orientation);


};

#endif
