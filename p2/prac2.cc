#include <iostream>
#include <vector>
#include <cctype>
#include <cstring>
#include <fstream> //para trabajar con ficheros
#include <sstream>
#include <regex> //para trabajar con patrones en cadenas de texto

using namespace std;

const int KMAXSTRING = 50;
const int KMAXIP = 16;
const int MIN_NAME_LENGTH = 3;
const char DOSPUNTOS = ':';

enum Error {
  ERR_OPTION,
  ERR_NAME,
  ERR_EMAIL,
  ERR_ID,
  ERR_IP,
  ERR_FILE,
  ERR_ARGS
};

struct Subscriber {
  unsigned int id;
  string name;
  string email;
  string mainIp;
  vector<string> ips;
};

struct BinSubscriber {
  unsigned int id;
  char name[KMAXSTRING];
  char email[KMAXSTRING];
  char mainIp[KMAXIP];
};

struct Platform {
  string name;
  vector<Subscriber> subscribers;
  unsigned int nextId;
};

struct BinPlatform {
  char name[KMAXSTRING];
  unsigned int nextId;
};

void error(Error e) {
  switch (e) {
    case ERR_OPTION:
      cout << "ERROR: wrong menu option" << endl;
      break;
    case ERR_NAME:
      cout << "ERROR: wrong name" << endl;
      break;
    case ERR_EMAIL:
      cout << "ERROR: wrong email" << endl;
      break;
    case ERR_ID:
      cout << "ERROR: wrong subscriber id" << endl;
      break;
    case ERR_IP:
      cout << "ERROR: wrong IP" << endl;
      break;
    case ERR_FILE:
      cout << "ERROR: cannot open file" << endl;
      break;
    case ERR_ARGS:
      cout << "ERROR: wrong arguments" << endl;
      break;
  }
}
/*Funcion que imprime el menu de opciones*/
void showMainMenu() {
  cout << "[Options]" << endl
       << "1- Show subscribers" << endl
       << "2- Add subscriber" << endl
       << "3- Add subscriber IP" << endl
       << "4- Delete subscriber" << endl
       << "5- Import/export" << endl
       << "q- Quit" << endl
       << "Option: ";
}
/*Muestra el listado con la infomacion de los subcriptores*/
void showSubscribers(const Platform &platform) {
  for (const auto &subscriber : platform.subscribers) {
    string allIP;
    cout << subscriber.id << ":" << subscriber.name << ":" << subscriber.email << ":" << subscriber.mainIp << ":";
    //Creo un solo string con todas las ips
    for (const auto &ip : subscriber.ips) {
      allIP += ip + "|";
    }
    //borro la "|" de la ultima ip
    if(allIP.size()>0){
      allIP.erase(allIP.size() - 1, 1); 
    }
    cout << allIP;
    cout << endl;
  }
}
/*Funcion que comprueba la validez del nombre*/
bool isValidName(const string& name) {
  /* La condición name.find(':') == string::npos verifica si el carácter ':' no está presente 
  en la cadena name devuelve string::npos, que es un valor especial que representa "no encontrado"*/
  return name.size() >= MIN_NAME_LENGTH && name.find(':') == string::npos;
}
/*Funcion que comprueba la validez del mail*/
bool isValidEmail(const string& email) {
   regex regexEmail("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    // Validar que el email tenga un único @ y que no comience ni termine con un punto
    if (count(email.begin(), email.end(), '@') != 1 || email.front() == '.' || email.back() == '.') {
        return false;
    }
    // Dividir el email en usuario y dominio
    string usuario = email.substr(0, email.find('@'));
    string dominio = email.substr(email.find('@') + 1);
    // Validar que las partes izquierda y derecha no sean cadenas vacías y que no comiencen ni terminen con un punto
    if (usuario.empty() || dominio.empty() || usuario.front() == '.' || usuario.back() == '.' || dominio.front() == '.' || dominio.back() == '.') {
        return false;
    }
    // Validar que las partes izquierda y derecha sólo contengan caracteres válidos
    if (!regex_match(usuario, regex("^[a-zA-Z0-9._%+-]+$")) || !regex_match(dominio, regex("^[a-zA-Z0-9._%+-]+$"))) {
        return false;
    }
    // Validar que la parte derecha contenga al menos un punto
    if (count(dominio.begin(), dominio.end(), '.') < 1) {
        return false;
    }
    return regex_match(email, regexEmail); // Validar el formato del email completo

}
/*Esta opción permite añadir un suscriptor nuevo al programa*/
void addSubscriber(Platform& platform) {
  //Introduzco el nombre y compruebo que sea correcto
  string name;
  do {
    cout << "Enter name: ";
    getline(cin,name);
    if (!isValidName(name)) {
      error(ERR_NAME);
    }
  } while (!isValidName(name));

  //Introduzco el nombre y compruebo que sea correcto 
  string email;
  do {
    cout << "Enter email: ";
    cin >> email;
    if (!isValidEmail(email)) {
      error(ERR_EMAIL);
    }
  } while (!isValidEmail(email));
    
  //Creo el nuevo subscriptor
  Subscriber newSubscriber;
  newSubscriber.id = platform.nextId++;
  newSubscriber.name = name;
  newSubscriber.email = email;
    
  //Lo añado al vector de la plataforma
  platform.subscribers.push_back(newSubscriber);
    
}
/*Funcion que comprueba la validez de la ip*/
bool isValidIp(string ip) {
  /*Compruebo la ip numero por numero usando de separador los puntos*/
  vector<string> parts;
  string part;
  stringstream ss(ip);

  // Separo por puntos almacenado en el vector cada agrupacion de numeros
  while (getline(ss, part, '.')) {
    parts.push_back(part);
  }

  // Compruebo que sean exacteamente 4 grupos
  if (parts.size() != 4) {
    return false;
  }

  // Convierto a enteros y compruebo que estente entre los valores correctos
  for (string part : parts) {
    int value = stoi(part);
    if (value < 0 || value > 255) {
      return false;
    }
  }
  return true;
}
/*Funcion que comprueba la validez de la id*/
bool isValidId(Platform& platform, unsigned int id){
  bool idCorrecta = true;

  // Si el identificador es vacío o no existe, mostramos un error y volvemos al menú principal
  for (unsigned int i = 0; i < platform.subscribers.size(); i++) {
    if (platform.subscribers[i].id == id) {
      idCorrecta = false;
    }
  }
  if (idCorrecta) {
    return false;
  }
  else if(id==0){
    return false;
  }
  else{
    return true;
  }
}
/*Funcio permite añadir una dirección IP de un dispositivo que pertenezca a un suscriptor*/
void addSubscriberIp(Platform& platform) {
  string ip, idString;
  int id = 0;
  
  // Pedimos el identificador del suscriptor
  cout << "Enter subscriber id: ";
  if (!getline(cin, idString) || idString.empty()) {
    error(ERR_ID);
    return;
  }
  id = stoi(idString);
  //pedimos la direccion ip
  if(isValidId(platform, id)){
    while (true) {
      cout << "Enter IP: ";
      getline(cin, ip);

      // Si la dirección es vacía o no es válida, mostramos un error y pedimos de nuevo la dirección
      if (ip.empty() || !isValidIp(ip)) {
        error(ERR_IP);
        continue;
      }

      // Añadimos la dirección IP al vector de direcciones del suscriptor
      platform.subscribers[id-1].ips.push_back(ip);

      // Determinamos la mainIp del suscriptor como la que más se repite en el vector de direcciones
      int maxCount = 0;
      string mainIp = "";
      /*se utiliza la función count_if para contar el número de veces que aparece cada dirección IP en la lista. 
      se utiliza otraIP como referencia constante a una cadena de ips 
      y verifica si es igual a la dirección IP actual.*/

      for (const auto& ipActual : platform.subscribers[id-1].ips) {
        int count = count_if(platform.subscribers[id-1].ips.begin(), platform.subscribers[id-1].ips.end(), 
            [&](const string& otraIP){ return otraIP == ipActual; });

        if (count > maxCount || (count == maxCount && ipActual < mainIp)) {
          maxCount = count;
          mainIp = ipActual;
        }
      }
      platform.subscribers[id-1].mainIp = mainIp;;

      break;
    }
  }
  else{
    error(ERR_ID);
  }
}
/*Funcion que permite borrar un suscriptor existente en la plataforma*/
void deleteSubscriber(Platform &platform) {
  unsigned int id=0;
  string idString;
  
  // Pedimos el identificador del suscriptor
  cout << "Enter subscriber id: ";
  if (!getline(cin, idString) || idString.empty()) {
    error(ERR_ID);
    return;
  }
  id = stoi(idString);

  if(isValidId(platform, id)){
    /*recorro el vector para buscar la poscion correcta*/
    for(int h=0; h<(int)platform.subscribers.size();h++){
      if (platform.subscribers[h].id==id){
        id = h;
      }
    }
    platform.subscribers.erase(platform.subscribers.begin() +id);
  }
  else{
    error(ERR_ID);
  }
}
/*Funcion proceso de importacion fichero de texto usando los : como separador*/
void importCSV(Platform &platform, ifstream &ficheroCSV){
  string subscriberImport;
       
  do{//leo todas la lineas del ficero

    while(getline(ficheroCSV,subscriberImport)){ 
      Subscriber newSubscriberImport;

      string name, email, ip1, ips;
      stringstream ss(subscriberImport);

      getline(ss, name, ':');
      if (isValidName(name)) { //compruebo el nombre
        newSubscriberImport.name=name;

        getline(ss, email, ':');
        if(isValidEmail(email)){ //compruebo el mail
          newSubscriberImport.email=email;
          
          getline(ss, ip1, ':');
          if(isValidIp(ip1)){ //compruebo ips
            newSubscriberImport.mainIp=ip1;

            string ipsImport;
            while (getline(ss, ipsImport, '|')) {
              if (isValidIp(ipsImport)) {
                newSubscriberImport.ips.push_back(ipsImport);
              }
            }

            newSubscriberImport.id = platform.nextId++;
            platform.subscribers.push_back(newSubscriberImport);
          }
          else{
            error(ERR_IP);
          }
        }
        else{
          error(ERR_EMAIL);
        }
      }
      else{
        error(ERR_NAME);
      }
    }   
  }while(!ficheroCSV.eof()); //compruebo que ha llegado al final

  ficheroCSV.close(); //cierro el fichero 
}
/*Funcion permite importar información de suscriptores almacenados en un fichero de texto*/
void importFromCsv(Platform &platform) {
  ifstream ficheroImport;
  string fileName;

  cout << "Enter filename: ";
  getline(cin,fileName);

  ficheroImport.open(fileName,ios::in);
  //si puedo abrir el fichero llamo a la funcion para iniciar el preceso
  if(ficheroImport.is_open()){
    importCSV(platform, ficheroImport);
  }

  else{
    error(ERR_FILE);
  }
}
/*Funcion que guarda en un fichero de texto*/
void exportToCsv(const Platform &platform) {
  ofstream ficheroEsc;
  string fileName,allIP;

  cout << "Enter filename: ";
  getline(cin,fileName);

  ficheroEsc.open(fileName,ios::out); //abro y si existe lo machaca

  if (ficheroEsc.is_open()){ //compruebo que se puede abrir
    
    int numSubscriber=(int)platform.subscribers.size();
    
    /*guardo en cada linea de fichero un libro con la estructura pedida*/
    for (int a=0;a<numSubscriber;a++){
      for(unsigned int i=0; i<platform.subscribers[a].ips.size();i++){
        allIP += platform.subscribers[a].ips[i] + "|";
      }

      //borro la "|" de la ultima ip
      if(allIP.size()>0){
        allIP.erase(allIP.size() - 1, 1); 
      }
      
      ficheroEsc <<  platform.subscribers[a].name << DOSPUNTOS
      << platform.subscribers[a].email << DOSPUNTOS
      << platform.subscribers[a].mainIp << DOSPUNTOS
      << allIP<<endl;

      //vacio las ips
      allIP.clear();
    }
    ficheroEsc.close(); //cierro el fichero
  }
  else{
    error(ERR_FILE);
  }
}
/*Funcion proceso de importacion fichero binario*/
void loadProces(Platform &platform, string fileName){
  ifstream ficheroBinLec;

  ficheroBinLec.open(fileName,ios::in | ios::binary);//abro el fichero binario

  if (ficheroBinLec.is_open()){//compruebo si se puede abrir

    BinPlatform binPlatformLoad;
    BinSubscriber binSubscriberLoad;
    Subscriber subscriberLoad;
        
    platform.subscribers.clear();//limpio el vector

    //asigno el nombre de la plataforma y el id
    ficheroBinLec.read((char *)&binPlatformLoad, sizeof(BinPlatform));
    platform.name=binPlatformLoad.name;
    platform.nextId=(binPlatformLoad.nextId);
          
    //voy grabando los libros
    while(ficheroBinLec.read((char *)&binSubscriberLoad, sizeof(BinSubscriber))){
          
      subscriberLoad.id=binSubscriberLoad.id;
      subscriberLoad.name=binSubscriberLoad.name; //nombre
      subscriberLoad.email=binSubscriberLoad.email;
      subscriberLoad.mainIp=binSubscriberLoad.mainIp;

      platform.subscribers.push_back(subscriberLoad);
      platform.subscribers[binSubscriberLoad.id-1].ips.push_back(binSubscriberLoad.mainIp);
      
    } 
        
    ficheroBinLec.close();
  }
  else{
    error(ERR_FILE);
    
  }
}
/*Funcion permite importar información de suscriptores almacenados en un fichero binario*/
void loadData(Platform &platform) {
  bool preguntaSeguridad=true;
  char option;
  string fileName;
  do{
    cout << "All data will be erased. Continue? [y/n]: ";
    cin >> option;
    cin.get();
    if (option=='N'||option=='n'){
      preguntaSeguridad=false;
      
    }
    else if(option=='Y'||option=='y'){
      preguntaSeguridad=false;
      cout<< "Enter filename: ";
      getline(cin,fileName);

      ifstream ficheroBinLec;

      ficheroBinLec.open(fileName,ios::in | ios::binary);//abro el fichero binario

      if (ficheroBinLec.is_open()){
        loadProces(platform,fileName);
      }
      else{
        error(ERR_FILE);
      }
    }
  }while (preguntaSeguridad==true);
}

void stringToChar(string name, char nameConvert[]){
 //convieto el string en char recortandolo has la constante-1 
  strncpy(nameConvert, name.c_str(), KMAXSTRING-1);
 //asigno en la ultima posicion el caracter '\0' 
  nameConvert[KMAXSTRING-1]='\0';

}
void stringToCharIP(string name, char nameConvert[]){
 //convieto el string en char recortandolo has la constante-1 
  strncpy(nameConvert, name.c_str(), KMAXIP-1);
 //asigno en la ultima posicion el caracter '\0' 
  nameConvert[KMAXIP-1]='\0';

}
/*Funcion que guarda en un fichero binario*/
void saveData(const Platform &platform) {
  string fileName;
  ofstream ficherBinGuardar;

  cout << "Enter filename: ";
  getline(cin,fileName);

  ficherBinGuardar.open(fileName,ios::out | ios::binary); //abro el fichero

  if (ficherBinGuardar.is_open()){
    
    BinPlatform binplatformSave;

    binplatformSave.nextId=platform.nextId;
    //ayudandome de la funcion paso el nombre de la platform a char
    stringToChar(platform.name,binplatformSave.name);
    
    //almaceno el nombre de la platform y el id
    ficherBinGuardar.write((const char *)&binplatformSave, sizeof(BinPlatform));
    
    //almaceno subscribtor
    for (int unsigned i=0;i<platform.subscribers.size();i++){
      BinSubscriber binsubscriberSave;

      binsubscriberSave.id=platform.subscribers[i].id; //id
      stringToChar(platform.subscribers[i].name,binsubscriberSave.name); 
      stringToChar(platform.subscribers[i].email,binsubscriberSave.email);
      stringToCharIP(platform.subscribers[i].mainIp, binsubscriberSave.mainIp);

      ficherBinGuardar.write((const char *)&binsubscriberSave, sizeof(BinSubscriber));
  
    }

    ficherBinGuardar.close();
  }
  else{
    error(ERR_FILE);
  }
}
/*Funcion que imprime el menu de manejo de ficheros*/
void showImportMenu(){
  cout << "[Import/export options]" << endl
       <<"1- Import from CSV"<< endl
       <<"2- Export to CSV"<<endl
       <<"3- Load data"<<endl
       <<"4- Save data"<<endl
       <<"b- Back to main menu"<<endl
       <<"Option: ";
}

void importExportMenu(Platform &platform) {
  char option;
  do {
    showImportMenu();
    cin >> option;
    cin.get();

    switch (option) {
      case '1':
        importFromCsv(platform);
        break;
      case '2':
        exportToCsv(platform);
        break;
      case '3':
        loadData(platform);
        break;
      case '4':
        saveData(platform);
        break;
      case 'b':
        break;
      default:
        error(ERR_OPTION);
    }
  } while (option != 'b');
}
/*Importar un archivo binario por argumentos*/
void loadArgument(Platform &platform, string argument, int &cont){
  ifstream ficheroLoad;
  string fileName=argument;

  ficheroLoad.open(fileName,ios::in | ios::binary);
  
  if(ficheroLoad.is_open()){
    loadProces(platform, fileName); //llamo al proceso de importacion binaria
  }
  //contador para no mostra el menu en caso de exixtir un error
  else{
    error(ERR_FILE);
    cont++;
  }
}
/*Importar fichero de texto por argumento*/
void importArgument(Platform &platform, string argument, int &cont){
  ifstream ficheroImport;
  string fileName=argument;

  ficheroImport.open(fileName,ios::in);
  
  if(ficheroImport.is_open()){
    importCSV(platform, ficheroImport);
  }
  
  else{
    error(ERR_FILE);
    cont++;
  }
}

/*Funcion para verificar los argumentos*/
void isValidArgument(vector<string> argument, int argc, int &cont){

  if((argc==3)||(argc==5)){ //poscion correcta
    if((argument[0]=="-i")||(argument[0]=="-l")){
      if(argc==5){//aqui solo entra si hay mas de un argumento
        if((argument[2]=="-i")||(argument[2]=="-l")){
          if(argument[0]==argument[2]){
            cont++;
          }
        }
        else{
          cont++;
        }
      }
    }
    else{
      cont++;
    }
  }
  else{
    cont++;
  }
}
/*Funcion que tras comprobar los argumentos elije el orden de ejecucion*/
void programArgument(Platform &platform, int argc, char *argv[], int &cont){
  vector<string> argument; //Vector que uso para guardarme los argumentos que hay en argv.
  int binary=0, text=0;

  for(int i=1; i<argc; i++){
    argument.push_back(argv[i]);
  }

  isValidArgument(argument, argc, cont);// Verifico si los argumentos son correctos.
  /*Con se aumenta en el caso de que exista un error en los argumentos*/
  if(cont!=0){
    error(ERR_ARGS);
  }
  else{
    for(int i=0; i<(int)argument.size(); i++){
      if((argument[i]=="-l")&&((i==0)||(i==2))){
        binary=i+1;
      }

      else if((argument[i]=="-i")&&((i==0)||(i==2))){
        text=i+1;
      }
    }
    //Si hay algun "-l", binary no valdra 0 y se activara. 
    if(binary!=0){
      loadArgument(platform, argument[binary], cont);
    }

    /*no se volverá al menu principal y se cerrará el programa*/
    if((text!=0)&&(cont==0)){
      importArgument(platform, argument[text], cont);
    }
  }
}
int main(int argc, char *argv[]) {
  Platform platform;
  platform.name = "Streamflix";
  platform.nextId = 1;

  char option;
  int cont=0;

  if(argc>1){
    programArgument(platform, argc, argv, cont);
  }
  if((argc==1)||(cont==0)){
    do {
      showMainMenu();
      cin >> option;
      cin.get();

      switch (option) {
        case '1':
          showSubscribers(platform);
          break;
        case '2':
          addSubscriber(platform);
          break;
        case '3':
          addSubscriberIp(platform);
          break;
        case '4':
          deleteSubscriber(platform);
          break;
        case '5':
          importExportMenu(platform);
          break;
        case 'q':
          break;
        default:
          error(ERR_OPTION);
      }
    } while (option != 'q');
  }
  return 0;
}
