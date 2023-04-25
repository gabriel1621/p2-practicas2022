#include <iostream>

using namespace std;

#include "Util.h"

void Util::error(Error e){
  switch(e){
    case ERR_SHIPS:
      cout << "ERROR: cannot place ships" << endl;
      break;
    case ERR_GAME_OVER:
      cout << "GAME OVER!!" << endl;
      break;
  }
}

void Util::debug(Exception e) {

  const string EXCEPT_STR[] = {
    "EXCEPTION_WRONG_ORIENTATION",
    "EXCEPTION_WRONG_SHIP_TYPE",
    "EXCEPTION_WRONG_COORDINATES",
    "EXCEPTION_MAX_SHIP_TYPE",
    "EXCEPTION_GAME_STARTED",
    "EXCEPTION_OUTSIDE",
    "EXCEPTION_NONFREE_POSITION",
    "EXCEPTION_ALREADY_SUNK",
    "EXCEPTION_ALREADY_HIT",
    "EXCEPTION_GAME_OVER" };
    
  cout << "DEBUG: error produced by exception " << EXCEPT_STR[e] << endl;

}