#ifndef _UTIL_H_
#define _UTIL_H_

enum Exception {
  EXCEPTION_WRONG_ORIENTATION,
  EXCEPTION_WRONG_SHIP_TYPE,
  EXCEPTION_WRONG_COORDINATES,
  EXCEPTION_MAX_SHIP_TYPE,
  EXCEPTION_GAME_STARTED,
  EXCEPTION_OUTSIDE,
  EXCEPTION_NONFREE_POSITION,
  EXCEPTION_ALREADY_SUNK,
  EXCEPTION_ALREADY_HIT,
  EXCEPTION_GAME_OVER
};



enum Error{
  ERR_SHIPS,
  ERR_GAME_OVER
};



class Util{
  public:
    // prints an error message
    static void error(Error e);

    // prints exception type (debug)
    static void debug(Exception e);
    
};

#endif
