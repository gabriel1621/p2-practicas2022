//Gabriel Pardo - 48775081Q
#ifndef _Player_h
#define _Player_h

#include "Coordinate.h"
#include "Ship.h"
using namespace std;

class Player{
    friend ostream& operator<<(ostream &os,const Player &player );
    protected:
        string name;
        vector<Ship> ships;
    public:
        Player(string name);
        string getName() const;
        void addShip(const Coordinate& pos,ShipType type,Orientation orientation);
        void addShips(string ships);
        bool attack(const Coordinate &coord);
        bool attack(string coord);

};

#endif
