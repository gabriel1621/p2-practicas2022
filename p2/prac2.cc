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
const regex EMAIL_REGEX(R"(^[^./\s][^/@\s]*@([a-zA-Z0-9-]+\.)+[a-zA-Z]{2,}$)"); //no apacer /
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
  return regex_match(email, EMAIL_REGEX);
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
    error(ERR_ID);
    return false;
  }
  else{
    return true;
  }

}

void addSubscriberIp(Platform& platform) {
  string ip;
  int id=0;
  do{
    // Pedimos el identificador del suscriptor
    cout << "Enter subscriber id: ";
    cin >> id;
    cin.ignore();

  }while(!isValidId(platform, id));


  // Pedimos la dirección IP a añadir
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

    // a almacenamos en el campo mainIp del suscriptor
    platform.subscribers[id-1].mainIp = ip;

    break;
  }
}

void deleteSubscriber(Platform &platform) {
  int id=0;
  
  // Pedimos el identificador del suscriptor
  cout << "Enter subscriber id: ";
  cin >> id;
  cin.ignore();

  if(isValidId(platform, id)){
    platform.subscribers.erase(platform.subscribers.begin()+id-1);
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
          
      subscriberLoad.id=binSubscriberLoad.id; //id
      subscriberLoad.name=binSubscriberLoad.name; //nombre
      subscriberLoad.email=binSubscriberLoad.email;
      subscriberLoad.mainIp=binSubscriberLoad.mainIp;


      platform.subscribers.push_back(subscriberLoad);
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

void saveData(const Platform &platform) {
  string fileName;//le asigno un tamaño ya que tiene que ser un char con tamaño constante
  ofstream ficherBinGuardar;

  cout << "NAMEFILE";
  cin >> fileName;
  getline(cin,fileName);
}
void showImportMenu(){
  cout << "[Import/export options]" << endl
       <<"1- Import from CSV"<< endl
       <<"2- Export to CSV"<<endl
       <<"3- Load data"<<endl
       <<"4- Save data"<<endl
       <<"b- Back to main menu"<<endl
       <<"Option:";
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

int main(int argc, char *argv[]) {
  Platform platform;
  platform.name = "Streamflix";
  platform.nextId = 1;

  char option;
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

  return 0;
}
