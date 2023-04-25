//Gabriel Pardo - 48775081Q
#ifndef _Ship_h
#define _Ship_h

#include <iostream>
#include <vector>
#include "Coordinate.h"
#include "Player.h"

using namespace std;


enum ShipType {
    BATTLESHIP,
    DESTROYER,
    CRUISE,
    SUBMARINE
};

enum ShipState {
    OK,
    DAMAGED,
    SUNK
};

class Ship{
    friend ostream& operator<<(ostream &os,const Ship &ship );
    protected:
        ShipState state;
        ShipType type;
        vector<Coordinate*> positions;
        Coordinate board[10][10];
    public:
        Ship(ShipType type,const vector<Coordinate *>&positions );
        static unsigned shipSize(ShipType type);
        static ShipType typeFromChar(char type);
        Coordinate *getPosition(unsigned pos) const;
        ShipState getState() const;
        ShipType getType() const;
        bool hit(const Coordinate &coord);

};

#endif
