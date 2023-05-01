#include "Ship.h"
#include "Util.h"

Ship::Ship(ShipType type, const vector<Coordinate*>& positions) {
    if (positions.size() != static_cast<size_t>(type)) {
        throw EXCEPTION_WRONG_COORDINATES;
    }
    this->type = type;
    this->positions = positions;
    for (Coordinate* coord : positions) {
        coord->setState(SHIP);
    }
    state = OK;
}


      
unsigned Ship::shipSize(ShipType type) {
    switch(type) {
        case BATTLESHIP:
            return 4;
        case DESTROYER:
            return 3;
        case CRUISE:
            return 2;
        case SUBMARINE:
            return 1;
        default:
            return 0;
    }
}
ShipType Ship::typeFromChar(char type) {
    switch (type) {
        case 'B':
            return BATTLESHIP;
        case 'D':
            return DESTROYER;
        case 'C':
            return CRUISE;
        case 'S':
            return SUBMARINE;
        default:
            throw EXCEPTION_WRONG_SHIP_TYPE;
    }
}

Coordinate* Ship::getPosition(unsigned pos) const {
    if (pos < positions.size()) {
        return positions[pos];
    }
    else {
        return NULL;
    }
}

ShipState Ship:: getState() const{
    return state;
}
ShipType Ship::getType() const{
    return type;
}

/*Este método recorre el vector de posiciones del barco y compara cada una de ellas con la coordenada que se le ha pasado como argumento (coord). Si encuentra una posición que coincide con la coordenada, comprueba su estado. Si el estado es SHIP, cambia el estado de la posición a HIT y actualiza el estado del barco según las reglas indicadas en la especificación.

Si la posición ya tenía el estado HIT, lanza la excepción EXCEPTION_ALREADY_HIT. Si el barco ya estaba hundido, lanza la excepción EXCEPTION_ALREADY_SUNK. En ambos casos, estas excepciones deben ser capturadas en la clase Player.

Si ninguna de las posiciones del barco coincide con la coordenada, el método devuelve false. En caso contrario, devuelve true si el ataque ha sido un acierto (es decir, la posición estaba en estado SHIP) y no ha lanzado ninguna excepción.*/
bool Ship::hit(const Coordinate &coord){
    bool hit = false;
    for (unsigned i = 0; i < positions.size(); i++) {
        if (coord.compare(*positions[i]) == 0) {
            if (positions[i]->getState() == SHIP) {
                hit = true;
                positions[i]->setState(HIT);
                if (state == OK) {
                    if (type == SUBMARINE) {
                        state = SUNK;
                    }
                    else {
                        state = DAMAGED;
                    }
                }
                else if (state == DAMAGED) {
                    bool all_hit = true;
                    for (unsigned j = 0; j < positions.size(); j++) {
                        if (positions[j]->getState() == SHIP) {
                            all_hit = false;
                            break;
                        }
                    }
                    if (all_hit) {
                        state = SUNK;
                    }
                }
                else if (state == SUNK) {
                    throw EXCEPTION_ALREADY_SUNK;
                }
            }
            else {
                throw EXCEPTION_ALREADY_HIT;
            }
            break;
        }
    }
    return hit;
}

/*/
string Ship::getTypeName() {
    string typeName;
    switch (type) {
        case SUBMARINE:
            typeName = "Submarine";
            break;
        case DESTROYER:
            typeName = "Destroyer";
            break;
        case CRUISE:
            typeName = "Cruise";
            break;
        case BATTLESHIP:
            typeName = "Battleship";
            break;
        default:
            typeName = "Unknown";
            break;
    }
    return typeName;
}*/

ostream& operator<<(ostream& os, const Ship& ship) {
  // os << ship.getTypeName() << " (";
    switch (ship.getState()) {
        case OK:
            os << "O";
            break;
        case DAMAGED:
            os << "D";
            break;
        case SUNK:
            os << "S";
            break;
    }
    os << "): ";/*
    for (int i = 0; i < ship.shipSize(); i++) {
        os << *ship.getPosition(i);
        if (i != ship.shipSize(type) - 1) {
            os << " ";
        }
    }*/
    os << "\n";
    return os;
}
