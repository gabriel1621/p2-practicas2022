#include "Coordinate.h"
#include "Util.h"

Coordinate::Coordinate(){
    row=-1;
    column=-1;
    state = NONE;
}

Coordinate::Coordinate(int row, int column){
    this->row=row;
    this->column=column;
}
/*En este constructor se obtiene la letra de la fila y la cadena de la columna a partir de la cadena proporcionada.

A continuación, se comprueba que la letra de la fila es una letra del alfabeto y se convierte a mayúsculas 
si no lo está. Luego, se convierte la letra de la fila a un entero de fila (donde 'A' es 0, 'B' es 1, etc.).

Finalmente, se convierte la cadena de la columna en un entero de columna y se asigna el valor de la fila, 
la columna y el estado a la instancia actual de Coordinate. */
Coordinate::Coordinate(string coord) {

    char rowChar = coord[0];
    string columnString = coord.substr(1);


    int row = rowChar - 'A';
    int column = stoi(columnString) - 1;

    
    this->row = row;
    this->column = column;
    this->state = NONE;
}


int Coordinate::getRow() const{
    return row;
}
int Coordinate::getColumn() const{
    return column;
}

CellState Coordinate::getState() const{
    return state;
}

char Coordinate::getStateChar() const{
    char stateChar;
    switch (state) {
        case NONE:
            stateChar = 'N';
            break;
        case SHIP:
            stateChar = 'S';
            break;
        case HIT:
            stateChar = 'H';
            break;
        case WATER:
            stateChar = 'W';
            break;

    }
    return stateChar;
}

void Coordinate::setRow(int row){
    this->row=row;
}
void Coordinate::setColumn(int column){
    this->column=column;
}
void Coordinate::setState(CellState state){
    this->state=state;
}

bool Coordinate::compare(const Coordinate &coord) const{
    
    return row == coord.row && column == coord.column;

}

Coordinate Coordinate::addOffset(int offset, Orientation orientation) const {
    int newRow = row;
    int newColumn = column;

    switch (orientation) {
        case NORTH:
            newRow -= offset;
            break;
        case EAST:
            newColumn += offset;
            break;
        case SOUTH:
            newRow += offset;
            break;
        case WEST:
            newColumn -= offset;
            break;
    }

    Coordinate coord(newRow, newColumn);
    coord.setState(NONE);
    return coord;
}

Orientation Coordinate::orientationFromChar(char orientation) {
    switch (toupper(orientation)) {
        case 'N':
            return NORTH;
        case 'E':
            return EAST;
        case 'S':
            return SOUTH;
        case 'W':
            return WEST;
        default:
            throw EXCEPTION_WRONG_ORIENTATION;
    }
}

ostream& operator<<(ostream& os, const Coordinate& coord) {
    if (coord.getRow() < 0 || coord.getColumn() < 0) {
        os << "--";
    } else {
        char row = 'A' + coord.getRow();
        int col = coord.getColumn() + 1;
        os << row << col;
        char stateChar = coord.getStateChar();
        if (stateChar != 'N') {
            os << stateChar;
        }
    }
    return os;
}
