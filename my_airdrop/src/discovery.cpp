#include <iostream>
#include <vector>
#include <format>
#include <memory>
#include <cstdlib>
#include <unistd.h>
#include <limits.h>

#include <simpleble/SimpleBLE.h>

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
                  string adp_name = this->get_adapter().identifier();

                  cout << "adapter name: " << adp_name << endl;
                  string cmd1 = format("hciconfig hci0 name 'OSIFS_{}'", this->deviceName);
 
                  cout << "cmd: " << cmd1 << endl;

                  system(cmd1.c_str());

                  string cmd3 = "hciconfig hci0 leadv 0";
                  system(cmd3.c_str());

                  cout << "Linux broadcasting as 'OSIFS_{}" << endl;

                  while(true) {}
            }

            Linux_BTHandler()
            {
                  char hostname[HOST_NAME_MAX];
                  if (gethostname(hostname, HOST_NAME_MAX) == 0)
                  {
                        cout << "Hostname: " << hostname << endl;

                  }
            }
};


int main()
{
      unique_ptr<BTHandler> bt_handler = make_unique<Linux_BTHandler>();

      bt_handler->broadcast("Hello World");
      return 0;
}