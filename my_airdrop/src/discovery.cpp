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
void LogDebug(const char* p) { std::cout << "DEBUG: " << p << std::endl; }
void LogInfo(const char* p)  { std::cout << "INFO: "  << p << std::endl; }
void LogStatus(const char* p){ std::cout << "STATUS: "<< p << std::endl; }
void LogWarn(const char* p)  { std::cout << "WARN: "  << p << std::endl; }
void LogError(const char* p) { std::cout << "ERROR: " << p << std::endl; }
void LogFatal(const char* p) { std::cout << "FATAL: " << p << std::endl; }
void LogAlways(const char* p){ std::cout << "LOG: "   << p << std::endl; }
void LogTrace(const char* p) { std::cout << "TRACE: " << p << std::endl; }

int main()
{
    ggkLogRegisterDebug(LogDebug);
    ggkLogRegisterInfo(LogInfo);
    ggkLogRegisterStatus(LogStatus);
    ggkLogRegisterWarn(LogWarn);
    ggkLogRegisterError(LogError);
    ggkLogRegisterFatal(LogFatal);
    ggkLogRegisterAlways(LogAlways);
    ggkLogRegisterTrace(LogTrace);
    
      cout << "Hello World" << endl;

      if (!ggkStart("gobbledegook", "OSIFS_framework-sergey", "linux", dataGetter, nullptr, MAX_ASYNC_INIT_TIMEOUT_MS))
      {
            cout << "Failed to start" << endl;
            return -1;     
      }
      cout << "Success start!" << endl;
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