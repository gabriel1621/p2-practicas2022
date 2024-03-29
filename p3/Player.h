#ifndef _Player_h
#define _Player_h

#include <iostream>
#include <vector>
#include <sstream>
#include "Coordinate.h"
#include "Ship.h"

using namespace std;

class Player {
    friend ostream& operator<<(ostream& os, const Player& player);

protected:
    string name;
    vector<Ship> ships;
    Coordinate board[10][10];

public:
        Player(const std::string& name);
        string getName() const;
        void addShip(const Coordinate& pos,ShipType type,Orientation orientation);
        void addShips(string ships);
        bool attack(const Coordinate &coord);
        bool attack(string coord);

};

#endif

