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


using namespace std;


const void *dataGetter(const char *pName)
{
      static std::string my_string = "Hello World!";
      if (std::string(pName) == "custom")
      {
            return my_string.c_str();
      }

      return nullptr;
}

int main()
{
      cout << "Hello World" << endl;
}