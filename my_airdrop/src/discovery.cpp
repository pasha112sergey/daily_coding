#include <iostream>
#include <vector>
#include <format>
#include <string>
#include <memory>
#include <cstdlib>
#include <unistd.h>
#include <limits.h>

#include <simpleble/SimpleBLE.h>

#define HOST_NAME_MAX 64

using namespace std;
using namespace SimpleBLE;

class BTHandler {
      public:
            virtual ~BTHandler() = default;

            Adapter get_adapter()
            {
                  if (!Adapter::bluetooth_enabled())
                  {
                       cout << "Bluetooth disabled" << endl;
                        return {};
                  }

                  return Adapter::get_adapters()[0];
            };
            
            virtual void broadcast(string message) = 0;
};

class Linux_BTHandler : public BTHandler {
      private:
            string deviceName;

      public:
            void broadcast(string message)
            {
                  cout << "[Linux] Configuring blueZ" << endl;

                  string cmd1 = format("hciconfig hci0 name '{}'", this->deviceName);
                  system(cmd1.c_str());
                  cout << "cmd: " << cmd1 << endl;

                  

                  string cmd3 = "hciconfig hci0 leadv 0";
                  system(cmd3.c_str());


                  cout << "Linux broadcasting as '" << this->deviceName << "'" << endl;

                  while(true) {}
            }

            Linux_BTHandler()
            {
                  char hostname[HOST_NAME_MAX];
                  if (gethostname(hostname, HOST_NAME_MAX) == 0)
                  {
                        cout << "Hostname: " << hostname << endl;
                        this->deviceName = format("OSIFS_{}", string(hostname));
                  }
                  else
                  {
                        perror("gethostname");
                        EXIT_FAILURE;
                  }
            }
};



int main()
{
      // unique_ptr<BTHandler> bt_handler = make_unique<Linux_BTHandler>();


      // bt_handler->broadcast("Hello World");

      Adapter adp = Adapter::get_adapters()[0];

      adp.scan_start();

      while(true)
      {
            cout << "Starting..." << endl;

            sleep(3);

            vector<Peripheral> ps = adp.scan_get_results();

            for (Peripheral p : ps)
            {
                  cout << "Found!!" << p.identifier() << endl;
            }
      }

      return 0;
}