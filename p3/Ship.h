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

class Ship {
    private:
        ShipState state;
        ShipType type;
        std::vector<Coordinate*> positions;
    public:
        Ship(ShipType type, const std::vector<Coordinate*>& positions);
        static unsigned shipSize(ShipType type);
        static ShipType typeFromChar(char type);
        Coordinate* getPosition(unsigned pos) const;
        ShipState getState() const;
        ShipType getType() const;
        bool hit(const Coordinate& coord);
        string getTypeName()const;
        friend std::ostream& operator<<(std::ostream& os, const Ship& ship);
};

#endif 

