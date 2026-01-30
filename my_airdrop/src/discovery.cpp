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

class GATT_Server {
      public:
            virtual ~GATT_Server() = default;

            Adapter get_adapter()
            {
                  if (!Adapter::bluetooth_enabled())
                  {
                       cout << "Bluetooth disabled" << endl;
                        return {};
                  }

                  return Adapter::get_adapters()[0];
            };
            
            virtual void start_server(string message) = 0;
};

class Linux_GATT_Server : public GATT_Server {
      private:
            string deviceName;

      public:
            void start_server(string message)
            {
                  cout << "[Linux] Configuring blueZ" << endl;

                  string cmds[] = {
                        "sudo btmgmt -i hci0 power on",
                        "sudo btmgmt -i hci0 le on",
                        "sudo btmgmt -i hci0 connectable on",
                        "sudo btmgmt -i hci0 advertising on",
                        "sudo btmgmt -i hci0 add-adv -c -s 1 -n 'OSIFS_framework-sergey'"
                  };
                  
                  for (const string cmd : cmds)
                  {
                        system(cmd.c_str());
                  }

                  cout << "Linux broadcasting as '" << this->deviceName << "'" << endl;
                  Adapter adp = Adapter::get_adapters()[0];
                  cout << "Address " << adp.address() << endl;

                  while(true) {}
            }

            Linux_GATT_Server()
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
      

      return 0;
}