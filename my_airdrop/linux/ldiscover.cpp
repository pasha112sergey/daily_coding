#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include "bluetooth.hpp"

using namespace std;

#ifdef OS_LINUX

#pragma message("Linux is defined")

void broadcast(string hostname)
{
      cout << "[Linux] Configuring blueZ" << endl;
      string cmd = "hciconfig hci0 name 'OSIFS_" + deviceName + "'";
      system(cmd.c_str());

      system("hciconfig hci0 leadv 0");
      cout << "Linux broadcasting as 'OSIFS_" << deviceName + "'" << endl;
}

#endif