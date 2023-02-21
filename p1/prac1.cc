//Pardo Ramon, Gabriel 48775081Q
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

const int KNAME=40;
const int KMAXOBSTACLES=20;


enum Error{
    ERR_OPTION,
    ERR_DIFFICULTY,
    ERR_LEVEL,
    ERR_COORDINATE,
    ERR_OBSTACLES,
    ERR_ID,
    ERR_INSTRUCTION
};

// Registro para las coordenadas
struct Coordinate{
    int row;
    int column;
};

// Registro para el jugador
struct Player{
    char name[KNAME];
    int difficulty;
    int score;
    int wins;
    int losses;
};

// Registro para el nivel
struct Level{
    int id;
    int size;
    int numObstacles;
    Coordinate obstacles[KMAXOBSTACLES];
    Coordinate start;
    Coordinate finish;
};


// Función que muestra los mensajes de error
void error(Error e){
    switch(e){
        case ERR_OPTION: cout << "ERROR: wrong option" << endl;
            break;
        case ERR_DIFFICULTY: cout << "ERROR: wrong difficulty" << endl;
            break;
        case ERR_LEVEL: cout << "ERROR: cannot create level" << endl;
            break;
        case ERR_COORDINATE: cout << "ERROR: wrong coordinate" << endl;
            break;
        case ERR_OBSTACLES: cout << "ERROR: wrong number of obstacles" << endl;
            break;
        case ERR_ID: cout << "ERROR: wrong id" << endl;
            break;
        case ERR_INSTRUCTION: cout << "ERROR: wrong instruction" << endl;
            break;
    }
}

// Función que muestra el menú de opciones
void showMenu(){
    cout << "[Options]" << endl
        << "1- Create level" << endl
        << "2- Delete level" << endl
        << "3- Show levels" << endl
        << "4- Play" << endl
        << "5- Report" << endl
        << "q- Quit" << endl
        << "Option: ";
}

// Funcion que crea el jugador
void createPlayer(Player &player){

    int difficultad=0;


    cout << "Name: ";
    cin.getline(player.name,KNAME-1);


    while((difficultad<1) || (difficultad>3)){

        cout << "Difficulty: ";
        cin >> difficultad;

        if((difficultad<1) || (difficultad>3)){

            error(ERR_DIFFICULTY);

        }

    }

    player.difficulty=difficultad;
    player.score=0;
    player.wins=0;
    player.losses=0;

}
bool comprobarCoordenada(int row, int column, vector<Level> &levels, Level &level, int levelID){

    int size= levels[levelID].size;
    
    
    if((row>=0 && row<size) && (column>=0 && column<size)){

        bool obstacle = true;
        
        // Comprobar si hay un obstáculo en la celda actual
        /*
        for (int k = 0; k < levels[levelID].numObstacles; k++) {
            if ((levels[levelID].obstacles[k].row == row && levels[levelID].obstacles[k].column == column) ||
                (levels[levelID].obstacles[k-1].row == row && levels[levelID].obstacles[k].column == column) ||
                (levels[levelID].obstacles[k].row == row && levels[levelID].obstacles[k-1].column == column) ||
                (levels[levelID].obstacles[k].row == row && levels[levelID].obstacles[k+1].column == column) &&
                (levels[levelID].obstacles[k-1].row != row || levels[levelID].obstacles[k-1].column != column) &&
                (levels[levelID].obstacles[k+1].row != row || levels[levelID].obstacles[k+1].column != column)) {
                obstacle = false;
                break;
            }
        }*/
        
        if(obstacle){
            if((levels[levelID].start.row != row && levels[levelID].start.column != column)&&
                levels[levelID].finish.row != row && levels[levelID].finish.column != column){

                return true;
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
        
    }
    else{
        return false;
    }
}



// Funcion que pone obstaculos en el nivel

void colocarObstaculo(Level &level, int levelID, vector<Level> &levels){

    string cadenaObstaculos;
    
    vector<int> coordenadas(0);

    do{
        cout << "Obstacles: ";
        getline(cin,cadenaObstaculos);

        for (const char c : cadenaObstaculos) {
            if (isdigit(c)) {
                coordenadas.push_back(c - '0');
            }
        }

        //compruebo numero de obstaculos
        
        if(levels[levelID].size==5 && coordenadas.size()>10){
            coordenadas.clear();
            error(ERR_OBSTACLES);
        }
        else if(levels[levelID].size==7 && coordenadas.size()>20){
            coordenadas.clear();
            error(ERR_OBSTACLES);
        }
        else if(levels[levelID].size==10 && coordenadas.size()>40){
            coordenadas.clear();
            error(ERR_OBSTACLES);
        }
        
        
    }while(coordenadas.size()<0);

    
    
    int x = 0;
    for (unsigned int i = 0; i < coordenadas.size()-1; i += 2) {
        int row = coordenadas[i];
        int column = coordenadas[i+1];
        
        if (comprobarCoordenada(row, column,levels, level,levelID)) {
            levels[levelID].obstacles[x].row = row;
            levels[levelID].obstacles[x].column = column;
            x++;
        }
        else{
            
            error(ERR_COORDINATE);
            colocarObstaculo(level, levelID,levels);
        }
    }

    levels[levelID].numObstacles = x;
}

// Funcion que dibuja el mapa
void dibujarMapa(Level &level) {
    

    
    // Dibujar el cuerpo del mapa
    for (int i = 0; i < level.size; i++) {
        for (int j = 0; j < level.size; j++) {
            bool obstacle = false;

            // Comprobar si hay un obstáculo en la celda actual
            for (int k = 0; k < level.numObstacles; k++) {
                if (level.obstacles[k].row == i && level.obstacles[k].column == j) {
                    obstacle = true;
                    break;
                }
            }

            // Dibujar la celda actual
            if (obstacle) {
                cout << "X ";
            } else if (level.finish.row == i && level.finish.column == j) {
                cout << "F ";
            } else if (level.start.row == i && level.start.column == j) {
                cout << "R ";
            } else {
                cout << "O ";
            }
        }
        cout << endl;
    }
}
// Funcion que crea el nivel
void createLevel(Level &level, Player &player, vector<Level> &levels){

    Level nuevoNivel;
    int nextID = levels.empty() ? 0 : levels.back().id + 1;
    
    if(levels.size() >= 10){
        error(ERR_LEVEL);
        showMenu();
        return;
    }

    nuevoNivel.id = nextID;
    switch(player.difficulty){
        case 1:
            nuevoNivel.size=5;
            nuevoNivel.numObstacles=5;
            nuevoNivel.start.row=4;
            nuevoNivel.start.column=0;
            nuevoNivel.finish.row=0;
            nuevoNivel.finish.column=4;
            break;
        case 2:
            nuevoNivel.size=7;
            nuevoNivel.numObstacles=10;
            nuevoNivel.start.row=6;
            nuevoNivel.start.column=0;
            nuevoNivel.finish.row=0;
            nuevoNivel.finish.column=6;
            break;
        case 3:
            nuevoNivel.size=9;
            nuevoNivel.numObstacles=15;
            nuevoNivel.start.row=8;
            nuevoNivel.start.column=0;
            nuevoNivel.finish.row=0;
            nuevoNivel.finish.column=8;
            break;
        default:
            error(ERR_DIFFICULTY);
            return;
    }
    
    levels.push_back(nuevoNivel);

    colocarObstaculo(levels.back(), nextID, levels);

    cout << "Level " << nextID << endl;
    dibujarMapa(levels[nextID]);

}


void deleteLevel(Level &level, vector<Level> &levels) {
    int borrarID = 0, identificador = 0;
    bool idCorrecta = true;

    cout << "Id: ";
    cin >> borrarID;
    cin.ignore(); // Agregar esta línea para consumir el salto de línea después de la entrada de cin

    for (int i = 0; i < (int)levels.size(); i++) {
        if (levels[i].id == borrarID) {
            idCorrecta = false;
            identificador = i;
        }
    }

    if (idCorrecta) {
        error(ERR_ID);
    } else {
        string preguntaSeguridad;

        cout << "Are you sure? [y/n]" << endl;
        getline(cin, preguntaSeguridad);

        if (preguntaSeguridad == "y" || preguntaSeguridad == "Y") {
            levels.erase(levels.begin() + identificador);
        } else {
            showMenu();
        }
    }
}

void showLevel(Level &level, vector<Level> &levels){
    
    for(unsigned int i=0;i<levels.size();i++){

        cout << "Level " << levels[i].id << endl;
        dibujarMapa(levels[i]);

    }
}


void playGame(Level &level, vector<Level> &levels, Player &player) {
    int id = 0, identificador = 0;
    bool idCorrecta = true;

    cout << "Id: ";
    cin >> id;

    for (int i = 0; i < (int) levels.size(); i++) {

        if (levels[i].id == id) {
            idCorrecta = false;
            identificador = i;
        }

    }

    if (idCorrecta) {
        error(ERR_ID);
    }
    else {
        cout << "Level " << id << endl;
        dibujarMapa(levels[id]);

        string instrucciones;
        cout << "Instructions: " << endl;
        cin >> instrucciones;

        int score = 0;
        bool lose = false;

        for (int i = 0; i < (int) instrucciones.length(); i++) {
            cout << "Instruction " << instrucciones[i] << endl;

            int newRow = levels[id].start.row;
            int newColumn = levels[id].start.column;

            switch (instrucciones[i]) {
            case 'U':
                newRow--;
                break;
            case 'D':
                newRow++;
                break;
            case 'L':
                newColumn--;
                break;
            case 'R':
                newColumn++;
                break;
            default:
                error(ERR_INSTRUCTION);
                lose = true;
                break;
            }

            if (newRow < 0 || newRow >= levels[id].size || newColumn < 0 || newColumn >= levels[id].size) {
                error(ERR_COORDINATE);
                lose = true;
                break;
            }

            bool collision = false;

            for (int j = 0; j < levels[id].numObstacles; j++) {
                if (levels[id].obstacles[j].row == newRow && levels[id].obstacles[j].column == newColumn) {
                    collision = true;
                    break;
                }
            }

            if (collision) {
                error(ERR_COORDINATE);
                lose = true;
                break;
            }

            levels[id].start.row = newRow;
            levels[id].start.column = newColumn;

            dibujarMapa(levels[id]);

            if (levels[id].start.row == levels[id].finish.row && levels[id].start.column == levels[id].finish.column) {
                levels[id].obstacles[levels[id].numObstacles].row = levels[id].finish.row;
                levels[id].obstacles[levels[id].numObstacles].column = levels[id].finish.column;
                levels[id].numObstacles++;
                score = 3 * (levels[id].size - 1) - instrucciones.length();
                if (score < 0) {
                    score = 0;
                }
                cout << "You win " << score << " points" << endl;
                player.score += score;
                player.wins++;
                return;
            }
        }

        if (lose) {
            cout << "You lose" << endl;
            player.losses++;
            return;
        }
    }
}



void reportPlayer(Player &player){

    string imprimirDificultad;

    switch(player.difficulty){

        case 1:
         imprimirDificultad= "Easy";
         cout << player.difficulty;
         break;
        case 2:
         imprimirDificultad= "Medium";
         break;
        case 3:
         imprimirDificultad= "Hard";
         break;

    }
    
    cout << "[Report]" << endl
         << "Name: " << player.name << endl
         << "Difficulty: " << imprimirDificultad << endl
         << "Score: " << player.score << endl
         << "Wins: " << player.wins << endl
         << "Losses: " << player.losses << endl
         << "Total: " << player.wins+player.losses<<endl; 

}

// Función principal (tendrás que añadirle más código tuyo)
int main(){
    char option;

    vector<Level> levels(0);

    Player player;
    Level level;

    createPlayer(player);
    
    do{
        showMenu();
        cin >> option;
        cin.get(); // Para evitar que el salto de línea se quede en el buffer de teclado y luego pueda dar problemas si usas "getline"
        
        switch(option){
            case '1': // Llamar a la función para crear un nuevo nivel
                createLevel(level,player,levels);
                break;
            case '2': // Llamar a la función para borrar un nivel existente
                deleteLevel(level,levels);
                break;
            case '3': // Llamar a la función para mostrar los niveles creados
                showLevel(level,levels);
                break;
            case '4': // Llamar a la función para jugar
                playGame(level,levels,player);
                break;
            case '5': // Llamar a la función para mostrar información del jugador
                reportPlayer(player);
                break;
            case 'q': break;
            default: error(ERR_OPTION); // Muestra "ERROR: wrong option"
        }
    }while(option!='q');
}
