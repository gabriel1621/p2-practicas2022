
/*
#include "Player.h"
#include "Ship.h"
#include "Util.h"
#include "Coordinate.h"

Player::Player(const std::string& name) : name(name) {
    // Crear el tablero de coordenadas 10x10
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            Coordinate* coord = new Coordinate(row, col);
            board[row][col] = coord;
        }
    }
}

string Player::getName() const {
    return name;
}
void Player::addShip(const Coordinate& pos, ShipType type, Orientation orientation) {
    // Comprobar si se ha alcanzado el máximo número de barcos de ese tipo
    int maxShipTypeCount = Ship::shipSize(type);
    int currentShipTypeCount = 0;
    for (Ship* ship : ships) {
        if (ship->getType() == type) {
            currentShipTypeCount++;
        }
    }
    if (currentShipTypeCount >= maxShipTypeCount) {
        throw EXCEPTION_MAX_SHIP_TYPE;
    }

    // Comprobar si la partida ya ha iniciado
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            if (board[row][col]->getState() == CellState::WATER || board[row][col]->getState() == CellState::HIT) {
                throw EXCEPTION_GAME_STARTED;
            }
        }
    }

    // Construir el vector de coordenadas del barco
    std::vector<Coordinate*> shipCoords;
    Coordinate currentPos = pos;
    for (unsigned int i = 0; i < Ship::shipSize(type); ++i) {
        // Comprobar si la coordenada está dentro del tablero
        if (currentPos.getRow() < 0 || currentPos.getRow() >= 10 || currentPos.getColumn() < 0 || currentPos.getColumn() >= 10) {
            throw EXCEPTION_OUTSIDE;
        }

        // Comprobar si la coordenada del tablero está vacía
        if (board[currentPos.getRow()][currentPos.getColumn()]->getState() != CellState::NONE) {
            throw EXCEPTION_NONFREE_POSITION;
        }

        // Agregar la coordenada al vector de coordenadas del barco
        shipCoords.push_back(board[currentPos.getRow()][currentPos.getColumn()]);

        // Calcular la siguiente posición en base a la orientación
        currentPos = currentPos.addOffset(1, orientation);
    }

    // Crear el barco y almacenarlo en el vector de barcos del jugador
    Ship* newShip = new Ship(type, shipCoords);
    ships.push_back(newShip);
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