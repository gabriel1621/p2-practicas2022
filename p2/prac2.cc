#include <iostream>
#include <vector>

using namespace std;

const int KMAXSTRING = 50;
const int KMAXIP = 16;

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
}

void addSubscriber(Platform &platform) {
}

void addSubscriberIp(Platform &platform) {
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
