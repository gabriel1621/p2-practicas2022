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
const regex EMAIL_REGEX(R"(^[^./\s][^/@\s]*@([a-zA-Z0-9-]+\.)+[a-zA-Z]{2,}$)"); //no apacer /

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

void showSubscribers(const Platform &platform) {
  
  for (const auto &subscriber : platform.subscribers) {
    string allIP;
    cout << subscriber.id << ":" << subscriber.name << ":" << subscriber.email << ":" << subscriber.mainIp << ":";
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

bool isValidName(const string& name) {
  return name.size() >= MIN_NAME_LENGTH && name.find(':') == string::npos;
}

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

void addSubscriber(Platform& platform) {
  string name;
  do {
    cout << "Enter name: ";
    getline(cin,name);
    if (!isValidName(name)) {
      error(ERR_NAME);
    }
  } while (!isValidName(name));
    
  string email;
  do {
    cout << "Enter email: ";
    cin >> email;
    if (!isValidEmail(email)) {
      error(ERR_EMAIL);
    }
  } while (!isValidEmail(email));
    
  // Create new subscriber
  Subscriber newSubscriber;
  newSubscriber.id = platform.nextId++;
  newSubscriber.name = name;
  newSubscriber.email = email;
    
  // Add new subscriber to platform
  platform.subscribers.push_back(newSubscriber);
    
  
}

bool isValidIp(string ip) {
  vector<string> parts;
  string part;
  stringstream ss(ip);

  // Separo por puntos
  while (getline(ss, part, '.')) {
    parts.push_back(part);
  }

  // Compruebo que sean exacteamente 4 grupos
  if (parts.size() != 4) {
    return false;
  }

  // Compruebo que estente entre los valores correctos
  for (string part : parts) {
    int value = stoi(part);
    if (value < 0 || value > 255) {
      return false;
    }
  }
  return true;
}

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

void addSubscriberIp(Platform& platform) {
  string ip, input;
  int id = 0;
  
  // Pedimos el identificador del suscriptor
  cout << "Enter subscriber id: ";
  if (!getline(cin, input) || input.empty()) {
    error(ERR_ID);
    return;
  }
  id = stoi(input);

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
      for (const auto& addr : platform.subscribers[id-1].ips) {
        int count = count_if(platform.subscribers[id-1].ips.begin(), platform.subscribers[id-1].ips.end(), 
            [&](const string& other){ return other == addr; });
        if (count > maxCount || (count == maxCount && addr < mainIp)) {
          maxCount = count;
          mainIp = addr;
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

void deleteSubscriber(Platform &platform) {
  unsigned int id=0;
  string input;
  
  // Pedimos el identificador del suscriptor
  cout << "Enter subscriber id: ";
  if (!getline(cin, input) || input.empty()) {
    error(ERR_ID);
    return;
  }
  id = stoi(input);

  if(isValidId(platform, id)){
    /*recorro el vector para buscar coincidencias y saber
    si el id exist*/
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

void importCSV(Platform &platform, ifstream &ficheroCSV){
  string subscriberImport;
       
  do{//leo todas la lineas del ficero

    while(getline(ficheroCSV,subscriberImport)){ 
      Subscriber newSubscriberImport;

      string name, email, ip1, ips;
      stringstream ss(subscriberImport);

      getline(ss, name, ':');
      if (isValidName(name)) {
        newSubscriberImport.name=name;

        getline(ss, email, ':');
        if(isValidEmail(email)){
          newSubscriberImport.email=email;
          
          getline(ss, ip1, ':');
          if(isValidIp(ip1)){
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


void importFromCsv(Platform &platform) {
  ifstream ficheroImport;
  string fileName;

  cout << "Enter filename: ";
  getline(cin,fileName);

  ficheroImport.open(fileName,ios::in);

  if(ficheroImport.is_open()){
    importCSV(platform, ficheroImport);
  }

  else{
    error(ERR_FILE);
  }
}

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

void saveData(const Platform &platform) {
  string fileName;
  ofstream ficherBinGuardar;

  cout << "Enter filename: ";
  getline(cin,fileName);

  /*char fileName[90];//le asigno un tamaño ya que tiene que ser un char con tamaño constante
  ofstream ficherBinGuardar;

  cout << "Enter filename: ";
  cin >> fileName;
  cin.get();*/

  ficherBinGuardar.open(fileName,ios::out | ios::binary); //abro el fichero

  if (ficherBinGuardar.is_open()){
    
    BinPlatform binplatformSave;

    binplatformSave.nextId=platform.nextId;
    //ayudandome de la funcion paso el nombre de la platform a char
    stringToChar(platform.name,binplatformSave.name);
    
    //alamaceno el nombre de la book store y el id
    ficherBinGuardar.write((const char *)&binplatformSave, sizeof(BinPlatform));
    
    //alaceno libro por libro con el bucle
    for (int unsigned i=0;i<platform.subscribers.size();i++){
      BinSubscriber binsubscriberSave;

      binsubscriberSave.id=platform.subscribers[i].id; //id
      stringToChar(platform.subscribers[i].name,binsubscriberSave.name); 
      stringToChar(platform.subscribers[i].email,binsubscriberSave.email);
      //cout<< platform.subscribers[i].mainIp;
      stringToCharIP(platform.subscribers[i].mainIp, binsubscriberSave.mainIp);

      

      
      ficherBinGuardar.write((const char *)&binsubscriberSave, sizeof(BinSubscriber));
  
    }

    ficherBinGuardar.close();

  }
  else{
    error(ERR_FILE);
  }
}
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
//Módulo donde a través de los argumentos introducidos importaré un archivo binario.
void loadArgument(Platform &platform, string argument, int &cont){
  ifstream ficheroLoad;
  string fileName=argument;

  //strcpy(fileName, argument.c_str());

  ficheroLoad.open(fileName,ios::in | ios::binary);
  
  if(ficheroLoad.is_open()){
    loadProces(platform, fileName); // Recurro a una función creada antes para procesar los datos.
  }

  else{
    error(ERR_FILE);
    cont++;
  }
}

//Módulo donde a través de los argumentos introducidos importaré un archivo de texto.
void importArgument(Platform &platform, string argument, int &cont){
  ifstream ficheroImport;
  string fileName=argument;

  //strcpy(fileName, argument.c_str());

  ficheroImport.open(fileName,ios::in);
  
  if(ficheroImport.is_open()){
    importCSV(platform, ficheroImport); // Recurro a una función creada antes para procesar los datos.
  }
  
  else{
    error(ERR_FILE);
    cont++;
  }
}

//Módulo donde se va a comprobar si la forma en la que se han introducido los argumentos es la correcta.
void veracityArgument(vector<string> argument, int argc, int &cont){

  if((argc==3)||(argc==5)){
    if((argument[0]=="-i")||(argument[0]=="-l")){
      if(argc==5){
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
//Módulo donde voy a procesar los distintos argumentos.
void programArgument(Platform &platform, int argc, char *argv[], int &cont){
  vector<string> argument; //Vector que uso para guardarme los argumentos que hay en argv.
  int binary=0, text=0; //Los inicio a 0 por que jamas van a guardar un numero que no sea 1 o 3 y no puedo dejarlos sin inicializar.

  for(int i=1; i<argc; i++){// Me guardo los argumentos.
    argument.push_back(argv[i]);
  }

  veracityArgument(argument, argc, cont);// Verifico si los argumentos son correctos.

  if(cont!=0){
    error(ERR_ARGS);
  }

  else{

    //Teniendo en cuenta que aqui solo se va entrar si los argumentos son correctos.
    /*Con este bucle y sabiendo las posiciones de los argumentos -i y -l 
    me guardo la posición donde estarán escritos los archivos. */
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

    /*Uso la misma forma que para acceder al loadArgument, pero le añado lo del contador, ya que si el anterior da ERR_FILE 
    no se volverá al menu principal y se cerrará el progrma, por lo que es innecesario buscar el otro archivo */
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
