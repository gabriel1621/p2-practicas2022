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