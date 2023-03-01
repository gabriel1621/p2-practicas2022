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
const regex EMAIL_REGEX(R"(^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$)");

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
  cout << "Subscribers:" << endl;
  for (const auto &subscriber : platform.subscribers) {
    cout << subscriber.id << ":" << subscriber.name << ":" << subscriber.email << ":" << subscriber.mainIp << ":";
    for (const auto &ip : subscriber.ips) {
      cout << ip << "|";
    }
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
        cin >> name;
        if (!isValidName(name)) {
            cout << "ERR_NAME" << endl;
        }
    } while (!isValidName(name));
    
    string email;
    do {
        cout << "Enter email: ";
        cin >> email;
        if (!isValidEmail(email)) {
            cout << "ERR_EMAIL" << endl;
        }
    } while (!isValidEmail(email));
    
    // Create new subscriber
    Subscriber newSubscriber;
    newSubscriber.id = platform.nextId++;
    newSubscriber.name = name;
    newSubscriber.email = email;
    
    // Add new subscriber to platform
    platform.subscribers.push_back(newSubscriber);
    
    cout << "Subscriber added successfully with ID " << newSubscriber.id << endl;
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



void addSubscriberIp(Platform& platform) {
    string id, ip;

    // Pedimos el identificador del suscriptor
    cout << "Enter subscriber id: ";
    getline(cin, id);

    // Si el identificador es vacío o no existe, mostramos un error y volvemos al menú principal
    if (id.empty() || platform.subscribers.count(id) == 0) {
        cout << "ERR_ID" << endl;
        return;
    }

    // Pedimos la dirección IP a añadir
    while (true) {
        cout << "Enter IP: ";
        getline(cin, ip);

        // Si la dirección es vacía o no es válida, mostramos un error y pedimos de nuevo la dirección
        if (ip.empty() || !isValidIp(ip)) {
            cout << "ERR_IP" << endl;
            continue;
        }

        // Añadimos la dirección IP al vector de direcciones del suscriptor
        platform.subscribers[id].ips.push_back(ip);

        // Calculamos la dirección IP principal y la almacenamos en el campo mainIp del suscriptor
        //subscribers[id].mainIp = calculateMainIp(subscribers[id].ips);

        break;
    }
}


void deleteSubscriber(Platform &platform) {
}

void importFromCsv(Platform &platform) {
}

void exportToCsv(const Platform &platform) {
}

void loadData(Platform &platform) {
}

void saveData(const Platform &platform) {
}

void importExportMenu(Platform &platform) {
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
