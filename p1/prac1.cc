//Pardo Ramon, Gabriel 48775081Q
#include <iostream>
#include <vector> //trabajar con vectores
#include <cstring> //trabajar string
#include <sstream>

using namespace std;

const int KNAME=40;
const int KMAXOBSTACLES=20;
const string DERROTA="You lose";


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

    //Pido el nombre de jugador
    cout << "Name: ";
    cin.getline(player.name,KNAME-1);

    //Verifico la dificultad
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

// funcion que verifica las coordenadas
bool comprobarCoordenada(int row, int column, Level &level) {

    int size = level.size;
    bool obstacle = false;

    // Verificar si la coordenada está dentro de los límites del nivel
    if ((row >= 0 && row < size) && (column >= 0 && column < size)) {

        // Verificar si hay algún obstáculo en la celda actual
        for (int k = 0; k < level.numObstacles; k++) {
            if (level.obstacles[k].row == row && level.obstacles[k].column == column) {
                obstacle = true;
                break;
            }
        }
        
        // Verificar si hay algún obstáculo adyacente a la coordenada actual
        if (!obstacle) {
            for (int k = 0; k < level.numObstacles; k++) {
                if ((level.obstacles[k].row == row - 1 && level.obstacles[k].column == column) ||
                    (level.obstacles[k].row == row + 1 && level.obstacles[k].column == column) ||
                    (level.obstacles[k].row == row && level.obstacles[k].column == column - 1) ||
                    (level.obstacles[k].row == row && level.obstacles[k].column == column + 1)) {
                    obstacle = true;
                    break;
                }
            }
        }

        // Verificar si la coordenada no es el inicio o el fin del nivel
        if (!obstacle && (level.start.row != row || level.start.column != column) 
        && (level.finish.row != row || level.finish.column != column)) {
            return true;
        }
        return true;
    }

    return false;
}

// Funcion que pone obstaculos en el nivel
void colocarObstaculo(Level &level, int levelID, vector<Level> &levels){
    string cadenaObstaculos;
    vector<int> coordenadas;
    
    //Pido las coordenadas
    cout << "Obstacles: ";
    getline(cin, cadenaObstaculos);

    stringstream ssObstaculos(cadenaObstaculos);
    string cadenaSinBarras;

    //elimino las barras
    while (getline(ssObstaculos, cadenaSinBarras, '|')) {
      stringstream ssObstaculosComas(cadenaSinBarras);
      string coordenada;

      //elimino las comas
      while (getline(ssObstaculosComas, coordenada, ',')) {
         int num = stoi(coordenada); //convierto a int
         coordenadas.push_back(num); 
        }
    }

    // Verificar el número de obstáculos
    if (level.size == 5 && coordenadas.size() > 10) {
        coordenadas.clear();
        error(ERR_OBSTACLES);
        colocarObstaculo(level,levelID,levels);
    } 
    else if (level.size == 7 && coordenadas.size() > 20) {
        coordenadas.clear();
        error(ERR_OBSTACLES);
        colocarObstaculo(level,levelID,levels);
    } 
    else if (level.size == 10 && coordenadas.size() > 40) {
        coordenadas.clear();
        error(ERR_OBSTACLES);
        colocarObstaculo(level,levelID,levels);
    }

    // Verificar las coordenadas
    bool errorCoord = false;
    int numObstacles = coordenadas.size() / 2;
    int x = 0;

    for (int i = 0; i < numObstacles; i++) {
        int row = coordenadas[i * 2];
        int column = coordenadas[i * 2 + 1];
        if (!comprobarCoordenada(row, column, level)) {
            errorCoord = true;
            break;
        }

        // Verificar si hay obstáculos adyacentes
        bool adjObstacle = false;
        for (int j = 0; j < x; j++) {
            if (abs(level.obstacles[j].row - row) <= 1 && abs(level.obstacles[j].column - column) <= 1) {
                adjObstacle = true;
                break;
            }
        }
        if (adjObstacle) {
            errorCoord = true;
            break;
        }

        // Agregar el obstáculo al nivel
        level.obstacles[x].row = row;
        level.obstacles[x].column = column;
        x++;
    }

    // Verificar si se ha producido algún error
    if (errorCoord) {
        coordenadas.clear();
        error(ERR_COORDINATE);
        colocarObstaculo(level,levelID,levels);
    } 
    else {
        level.numObstacles = x;
    }  
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
                cout << "X";
            } 
            else if (level.start.row == i && level.start.column == j) {
                cout << "R";
            }
            else if (level.finish.row == i && level.finish.column == j) {
                cout << "F";
            }
            else {
                cout << "O";
            }
        }
        cout << endl;
    }
}

// Funcion que crea el nivel
void createLevel(Level &level, Player &player, vector<Level> &levels){

    Level nuevoNivel;
    int nextID = levels.empty() ? 1 : levels.back().id + 1;
    
    
    if(levels.size() >= 10){
        error(ERR_LEVEL);
        showMenu();
        return;
    }

    nuevoNivel.id = nextID;
    switch(player.difficulty){
        case 1:
            nuevoNivel.size=5;
            nuevoNivel.numObstacles=0;
            nuevoNivel.start.row=4;
            nuevoNivel.start.column=0;
            nuevoNivel.finish.row=0;
            nuevoNivel.finish.column=4;
            
            break;
        case 2:
            nuevoNivel.size=7;
            nuevoNivel.numObstacles=0;
            nuevoNivel.start.row=6;
            nuevoNivel.start.column=0;
            nuevoNivel.finish.row=0;
            nuevoNivel.finish.column=6;
            break;
        case 3:
            nuevoNivel.size=10;
            nuevoNivel.numObstacles=0;
            nuevoNivel.start.row=9;
            nuevoNivel.start.column=0;
            nuevoNivel.finish.row=0;
            nuevoNivel.finish.column=9;
            break;
        default:
            error(ERR_DIFFICULTY);
            return;
    }
    
    levels.push_back(nuevoNivel);

    int numLevel=nextID;
    colocarObstaculo(levels[nextID-1], nextID, levels);
    cout << "Level " << numLevel << endl;
    dibujarMapa(levels[nextID-1]);

}

//Funcion que elimina el nivel
void deleteLevel(Level &level, vector<Level> &levels) {
    int borrarID = 0, identificador = 0;
    bool idCorrecta = true;
    bool comprobar=false;

    cout << "Id: ";
    cin >> borrarID;
    cin.ignore(); // Agrego esta línea para consumir el salto de línea después de la entrada de cin

    //compruebo que existe el nivel
    for (int i = 0; i < (int)levels.size(); i++) {
        if (levels[i].id == borrarID) {
            idCorrecta = false;
            identificador = i;
        }
    }

    if (idCorrecta) {
        error(ERR_ID);
    } else {
        char preguntaSeguridad;
        
        //imprimo pregunta de seguridad
        do{
            cout << "Are you sure? [y/n] " ;
            cin>>preguntaSeguridad;
            cin.get();
         
            if (preguntaSeguridad == 'y' || preguntaSeguridad == 'Y') {
                levels.erase(levels.begin() + identificador); //elimino poscion del vector
                comprobar=true;
            }
            else if(preguntaSeguridad == 'n' || preguntaSeguridad == 'N'){
                comprobar=true;
            }
        }while(comprobar == false);      
    }
}

//Funcion que muestra los niveles
void showLevel(Level &level, vector<Level> &levels){
    for(unsigned int i=0;i<levels.size();i++){
        int numLevel=levels[i].id;
        cout << "Level " << numLevel << endl;
        dibujarMapa(levels[i]);
    }
}

//Funcion jugar un nivel
void playGame(Level &level, vector<Level> &levels, Player &player) {
    int id = 0;
    bool idCorrecta = true;

    cout << "Id: ";
    cin >> id;

    //compruebo que exista
    for (int i = 0; i < (int) levels.size(); i++) {
        if (levels[i].id == id) {
            idCorrecta = false;
        }
    }

    if (idCorrecta) {
        error(ERR_ID);
    }
    else {
        //imprimo el nivel
        cout << "Level " << id << endl;
        id--;//resto 1 para acceder a la poscion correcta del vector
        dibujarMapa(levels[id]);

        //pido las instriciones
        string instrucciones;
        cout << "Instructions: ";
        cin >> instrucciones;

        int score = 0;
        bool lose = false;

        //modifco las cordenadas iniciales para ir moviendo el robot
        for (int i = 0; i < (int) instrucciones.length(); i++) {
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
            //compruebo que es una instrucion correcta
            if (lose) {
                cout << DERROTA << endl;
                player.losses++;
                return;
            }
            //compruebo que este dentro del mapa
            if (newRow < 0 || newRow >= levels[id].size || newColumn < 0 || newColumn >= levels[id].size) {
                error(ERR_INSTRUCTION);
                cout << DERROTA << endl;
                player.losses++;
                return;
            }

            bool collision = false;
            //compruebo que no coincida con un obstaculo
            for (int j = 0; j < levels[id].numObstacles; j++) {
                if (levels[id].obstacles[j].row == newRow && levels[id].obstacles[j].column == newColumn) {
                    collision = true;
                    break;
                }
            }

            if (collision) {
                error(ERR_INSTRUCTION);
                cout << DERROTA << endl;
                player.losses++;
                return;
            }

            levels[id].start.row = newRow;
            levels[id].start.column = newColumn;

            //imprimo el mapa con la instrucion
            cout << "Instruction " << instrucciones[i] << endl;
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
    }
}

//Función para mostrar información del jugador
void reportPlayer(Player &player){
    string imprimirDificultad;
    //Asigno la dificultad a imprimir
    switch (player.difficulty) {
    case 1:
     imprimirDificultad= "Easy";
     break;
    case 2:
     imprimirDificultad= "Medium";
     break;
    case 3:
     imprimirDificultad= "Hard";
     break;
    default: break;
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
    vector<Level> levels; //defino el vector de niveles
    Player player;
    Level level;

    //Llamo a la funcion para crear el jugador
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
