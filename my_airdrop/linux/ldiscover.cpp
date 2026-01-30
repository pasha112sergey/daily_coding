#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include "BTHandler.h"

using namespace std;

class Linux_BTHandler : public BTHandler {
      public: 
            void start_advertising(const std::string &deviceName)
            {
                  cout << "[Linux] Configuring blueZ" << endl;
                  string cmd = "hciconfig hci0 name 'OSIFS_" + deviceName + "'";
                  system(cmd.c_str());

                  system("hciconfig hci0 leadv 0");
                  cout << "Linux broadcasting as 'OSIFS_" << deviceName + "'" << endl;
            }
}

#endif