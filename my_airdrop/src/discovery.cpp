#include <iostream>
#include <vector>
#include <format>
#include <string>
#include <memory>
#include <cstdlib>
#include <unistd.h>
#include <limits.h>

#include "Gobbledegook.h"

// #include <simpleble/SimpleBLE.h>


#define HOST_NAME_MAX 64
#define MAX_ASYNC_INIT_TIMEOUT_MS 30 * 1000

using namespace std;

string ip = "255.255.255.255'";

const void *dataGetter(const char *pName)
{
      if (nullptr == pName)
      {
            cout << "NULL name sent to server data getter " << endl;
            return nullptr;
      }

      if (string(pName) == "ip")
      {
            cout << "Returning ip" << endl;
            return ip.c_str();
      }

      return nullptr;
}

int main()
{
      cout << "Hello World" << endl;

      if (!ggkStart("Test", "test", "test", dataGetter, nullptr, MAX_ASYNC_INIT_TIMEOUT_MS))
      {
            cout << "Failed to start" << endl;
            return -1;     
      }

      while (ggkGetServerRunState() < EStopping)
      {
            continue;
      }

      if (!ggkWait())
      {
            return -1;
      }

      return ggkGetServerHealth() == EOk ? 0 : 1;
}