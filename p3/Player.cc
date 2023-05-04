/*
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
void Player::addShips(string ships) {
    stringstream ss(ships);
    string ship;
    while (ss >> ship) {
        char type = ship[0];
        string coord = ship.substr(2, 2);
        char orientation = ship[4];
        try {
            Ship s(typeFromChar(type), Coord(coord), orientationFromChar(orientation));
            addShip(s);
        } catch (const invalid_argument& e) {
            throw;
        }
    }
}
bool Player::attack(const Coordinate &coord) {
    bool hit = false;
    for (auto &ship : fleet_) {
        try {
            ship.hit(coord);
            hit = true;
            if (ship.isSunk()) {
                if (all_of(fleet_.begin(), fleet_.end(), [](const Ship& s){ return s.isSunk(); })) {
                    throw EXCEPTION_GAME_OVER;
                }
            } else {
                return true;
            }
        } catch (const invalid_argument& e) {
            // invalid argument exception thrown by hit()
            continue;
        } catch (const exception& e) {
            // EXCEPTION_ALREADY_SUNK or EXCEPTION_ALREADY_HIT thrown by hit()
            return false;
        }
    }
    if (!hit) {
        board_(coord) = WATER;
        return false;
    }
    // should never get here
    return false;
}
bool Player::attack(string coord) {
    try {
        int row = coord[1] - '0';
        int col = coord[0] - 'A';
        return attack(Coordinate(row, col));
    } catch (...) {
        // catch any exception thrown by attack(Coordinate)
        return false;
    }
}
ostream& operator<<(ostream& os, const Player& player) {
    os << player.name << ":\n\n";
    os << "  ";
    for (int col = 0; col < Board::SIZE; ++col) {
        if (col == 9) {
            os << "  " << col + 1;
        } else {
            os << "   " << col + 1;
        }
    }
    os << "\n";

    for (int row = 0; row < Board::SIZE; ++row) {
        if (row == 0) {
            os << "  ";
        } else {
            os << char(row + 'A' - 1) << " ";
        }
        for (int col = 0; col < Board::SIZE; ++col) {
            os << " " << player.board.get(Coordinate(row, col)).symbol();
        }
        os << "\n";
    }
    os << "\n";
    os << "Ships:\n\n";
    for (auto ship : player.ships) {
        os << "  " << ship << "  " << ship->getStateString() << "\n";
    }
    os << "\n";
    return os;
}
*/