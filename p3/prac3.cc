#include <iostream>

#include "Util.h"


// ----------------------------------------------------------------------------
#include "Coordinate.h"

// test Coordinate methods

void testCoordinate()
{
    Coordinate c;
    cout << c.getRow() << "," << c.getColumn() 
         << " : " << c.getStateChar() << endl;   // -1,-1 : N 
    cout << "(" << c << ")" << endl;             // (--)
    

    Coordinate c2(3,9);
    c2.setState(WATER);
    cout << c2.getRow() << "," << c2.getColumn() 
         << " : " << c2.getStateChar() << endl;  // 3,9 : W
    cout << "(" << c2 << ")" << endl;            // (D10W)


    Coordinate c3 = c2.addOffset(3,WEST);
    cout << c3.getRow() << "," << c3.getColumn() 
         << " : " << c3.getStateChar() << endl;  // 3,6 : N
    cout << "(" << c3 << ")" << endl;            // (D7)
         
    // ... (you may add your own tests here)
    
}

// ----------------------------------------------------------------------------

/*   // uncomment from this line to the next function to test Ship methods 
   // (and uncomment the call to 'testShip' in the 'main' function)

#include "Ship.h"

// test Ship methods

void testShip()
{
    Coordinate coords[10];  // coordinates for test 

    coords[0].setRow(3);
    coords[0].setColumn(6);
    Coordinate *pc0 = &(coords[0]);
    
    coords[1].setRow(3);
    coords[1].setColumn(7);
    Coordinate *pc1 = &(coords[1]);
    
    vector<Coordinate *> ccru;
    ccru.push_back(pc0);
    ccru.push_back(pc1);
    
    Ship myCruise(CRUISE,ccru);
    cout << myCruise ;          // CRUISE (O): D7S D8S
    cout << coords[0] << endl;  // D7S
    cout << coords[1] << endl;  // D8S
    
    cout << endl;
    
    Coordinate chit(3,7);
    bool isHit = myCruise.hit(chit);
    
    cout << isHit << endl;      // 1       
    cout << myCruise ;          // CRUISE (D): D7S D8H
    cout << coords[0] << endl;  // D7S
    cout << coords[1] << endl;  // D8H
        
    // ... (you may add your own tests here)
}

*/

// ----------------------------------------------------------------------------

/*   // uncomment from this line to the 'main' function to test Player methods 
   // (and uncomment the call to 'testPlayer' in the 'main' function)
  
#include "Player.h"

// test Player methods


// play the game asking for coordinates, changing the turn if the attacker fails
void play(int &turn,Player &p1,Player &p2)
{
   cout << p1;
   cout << p2;

   string scoord;    
   switch(turn) {
      case 1:
         {
           cout << p1.getName() << " attacks, enter coordinate: ";
           getline(cin,scoord);
           if (!p2.attack(scoord))  // turn changes if the attack fails
             turn=2;
         }
         break;
      case 2:
         {
           cout << p2.getName() << " attacks, enter coordinate: ";
           getline(cin,scoord);
           if (!p1.attack(scoord))  // turn changes if the attack fails
              turn=1;
         }
         break;
   }
}


// simulate an interactive game, asking for coordinates to each player
void playInteractive(Player &player1,Player &player2)
{
    bool gameOver=false;
    int turn=1;  

    do
    {
      try {

        play(turn,player1,player2);

      }
      catch (Exception e) {
        Util::error(ERR_GAME_OVER);
        gameOver = true;
        cout << "================= After game over:" << endl 
             << player1 << player2 << endl;
      }
    } while (!gameOver);
}

enum TestMode {
  INTERACTIVE_TEST,
  DIRECT_TEST
};

void testPlayer()
{
    Player player1("Mudito");
    Player player2("Dormilón");

    // First, place player's ships
    cout << "Player " << player1.getName() << " placing ships..." << endl;
    try {
        Coordinate c(3,1);
        player1.addShip(c,DESTROYER,EAST);

        string ships="B-B3-E S-J10-N   C-D7-W  D-A1-S";
        player1.addShips(ships);
    }
    catch (Exception e) {
        Util::error(ERR_SHIPS);
        Util::debug(e);
        return;
    }

    cout << "Player " << player2.getName() << " placing ships..." << endl;
    try {
        string ships="S-J1-E C-J10-N";
        player2.addShips(ships);
    }
    catch (Exception e) {
        Util::error(ERR_SHIPS);
        Util::debug(e);
        return;
    }

    TestMode testMode = DIRECT_TEST;  // you may change this to INTERACTIVE_TEST

    if (testMode == INTERACTIVE_TEST)
    {
      playInteractive(player1,player2);
    }
    else
    {
      try {

        // test some shots (hit/sunk/water) on player1
        player1.attack("H9"); // water
        cout << player1;
        
        player1.attack("B3"); // hit
        cout << player1;
        
        player1.attack("J10"); // submarine sunk
        cout << player1;

        // test game over player2, sink all ships from player2
        Coordinate sub("J1"), cru1("J10"), cru2("I10");

        cout << "----------------- Before:" << endl 
             << player1 << player2 << endl;
        player2.attack(sub);
        cout << "----------------- After submarine sinks;" << endl 
             << player1 << player2 << endl;
        player2.attack(cru1);
        cout << "----------------- After cruise is hit:" << endl 
             << player1 << player2 << endl;
        player2.attack(cru2);
        cout << "----------------- After cruise sinks (but exception raises and THIS WILL NOT SHOW):" << endl 
             << player1 << player2 << endl;
        
      }
      catch (Exception e) {
        if (e == EXCEPTION_GAME_OVER) 
            Util::error(ERR_GAME_OVER);
        else
            Util::debug(e);
            
        cout << "================= End:" << endl 
             << player1 << player2 << endl;
      }
    }
}
*/

// ----------------------------------------------------------------------------


int main()
{
  cout << "-------------------- Coordinate tests -------------------" << endl;
  testCoordinate();  
  
  // uncomment these lines to test Ship methods
  /*
  cout << "-------------------- Ship tests -------------------------" << endl;
  testShip();     
  */

  // uncomment this line to test Player methods
  /*
  cout << "-------------------- Player tests -----------------------" << endl;
  testPlayer();   
  */
  
  return 0;
}
