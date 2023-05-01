#include "Player.h"
#include "Ship.h"
#include "Util.h"
#include "Coordinate.h"

Player::Player(string name) : name(name) {

}

string Player::getName() const {
    return name;
}

void Player::addShip(const Coordinate& pos, ShipType type, Orientation orientation) {
    int shipSize = Ship::shipSize(type);

    // Comprobar que el jugador no tenga ya en el tablero el máximo número de barcos de ese tipo
    int count = 0;
    for (const Ship& s : ships) {
        if (s.getType() == type) {
            count++;
        }
    }
    if (count >= Ship::maxShipCount(type)) {
        throw EXCEPTION_MAX_SHIP_TYPE;
    }

    // Comprobar que la partida no se ha iniciado ya
    for (int i = 0; i < BOARDSIZE; i++) {
        for (int j = 0; j < BOARDSIZE; j++) {
            if (board[i][j].getState() == WATER || board[i][j].getState() == HIT) {
                throw EXCEPTION_GAME_STARTED;
            }
        }
    }

    // Construir el vector de punteros a coordenadas necesario para crear el barco
    vector<Coordinate*> shipCoords;
    shipCoords.push_back(&board[pos.getRow()][pos.getCol()]);

    for (int i = 1; i < shipSize; i++) {
        Coordinate offset = Coordinate::addOffset(pos, orientation, i);
        if (!offset.inBounds()) {
            throw EXCEPTION_OUTSIDE;
        }
        Coordinate* pCoord = &board[offset.getRow()][offset.getCol()];
        if (pCoord->getState() != NONE) {
            throw EXCEPTION_NONFREE_POSITION;
        }
        shipCoords.push_back(pCoord);
    }

    // Crear y añadir el barco al vector de barcos del jugador
    Ship newShip(type, shipCoords);
    ships.push_back(newShip);

    // Marcar las coordenadas del tablero como ocupadas por el barco
    for (Coordinate* pCoord : shipCoords) {
        pCoord->setState(OCCUPIED);
    }
}
