#include <simpleble/SimpleBLE.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
      if (!SimpleBLE::Adapter::bluetooth_enabled())
      {
            cout << "Bluetooth disabled!\n";
            return EXIT_FAILURE;
      }
      
      cout << "Bluetooth enabled!\n";

      vector<SimpleBLE::Adapter> adapters = SimpleBLE::Adapter::get_adapters();

      for (SimpleBLE::Adapter a : adapters)
      {
            cout << a.identifier() << endl;
      }

      return EXIT_SUCCESS;
}