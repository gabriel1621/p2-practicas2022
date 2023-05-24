#include "Player.h"
#include "Ship.h"
#include "Util.h"
#include "Coordinate.h"
#include <iostream>  // Agregado

using namespace std;

Player::Player(const std::string& name) : name(name) {
    // Crear el tablero de coordenadas 10x10
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            board[row][col] = Coordinate(row, col);
        }
    }
}

std::string Player::getName() const {
    return name;
}

void Player::addShip(const Coordinate& pos, ShipType type, Orientation orientation) {
    // Comprobar si se ha alcanzado el máximo número de barcos de ese tipo
    int maxShipTypeCount = Ship::shipSize(type);
    int currentShipTypeCount = 0;
    for (Ship ship : ships) {
        if (ship.getType() == type) {
            currentShipTypeCount++;
        }
    }
    if (currentShipTypeCount >= maxShipTypeCount) {
        throw EXCEPTION_MAX_SHIP_TYPE;
    }

    // Comprobar si la partida ya ha iniciado
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            if (board[row][col].getState() == CellState::WATER || board[row][col].getState() == CellState::HIT) {
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
        if (board[currentPos.getRow()][currentPos.getColumn()].getState() != CellState::NONE) {
            throw EXCEPTION_NONFREE_POSITION;
        }

        // Agregar la coordenada al vector de coordenadas del barco
        shipCoords.push_back(&board[currentPos.getRow()][currentPos.getColumn()]);

        // Calcular la siguiente posición en base a la orientación
        currentPos = currentPos.addOffset(1, orientation);
    }

    // Crear el barco y almacenarlo en el vector de barcos del jugador
    Ship newShip(type, shipCoords);

    ships.push_back(newShip);
}

void Player::addShips( std::string ships) {
    std::stringstream ss(ships);
    std::string ship;
    while (ss >> ship) {
        char type = ship[0];
        std::string coord = ship.substr(2, 2);
        char orientation = ship[4];
        
        ShipType shipType;
        try {
            shipType = Ship::typeFromChar(type);
        } catch (const std::invalid_argument& e) {
            // Manejar el error de tipo de barco no válido si es necesario
            continue;  // O puedes elegir otra acción apropiada
            
        }

        Coordinate position;
        try {
            position = Coordinate(coord);
        } catch (const std::invalid_argument& e) {
            // Manejar el error de coordenada no válida si es necesario
            continue;  // O puedes elegir otra acción apropiada
        }
        
        Orientation shipOrientation;
        try {
            shipOrientation = Coordinate::orientationFromChar(orientation);
        } catch (const std::invalid_argument& e) {
            // Manejar el error de orientación no válida si es necesario
            continue;  // O puedes elegir otra acción apropiada
        }

        addShip(position, shipType, shipOrientation);
    }
}

bool Player::attack(const Coordinate& coord) {
    bool hit = false;
    for (Ship ship : ships) {
        try {
            if (ship.hit(coord)) {
                hit = true;
                break;
            }
        } catch (const std::invalid_argument& e) {
            continue;
        }
    }
    board[coord.getRow()][coord.getColumn()].setState(hit ? CellState::HIT : CellState::NONE);
    return hit;
}

bool Player::attack( std::string coord) {  // Corregido
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
    os << player.name << ":" << endl << endl;
    os << "  ";
    for (int col = 0; col < 10; ++col) {
        if (col == 9) {
            os << "  " << col + 1;
        } else {
            os << "   " << col + 1;
        }
    }
    os << endl;

    for (int row = 0; row < 10; ++row) {
        if (row == 0) {
            os << "  ";
        } else {
            os << char(row + 'A' - 1) << " ";
        }
        for (int col = 0; col < 10; ++col) {
            os << " " << (player.board[row][col].getState() == CellState::HIT ? "X" : " ");
        }
        os << endl;
    }
    os << endl;
    os << "Ships:" << endl << endl;
    for (auto ship : player.ships) {
        os << "  " << ship << "  " << player.board[0][0].getStateChar() << std::endl;
    }
    os << endl;
    return os;
}

