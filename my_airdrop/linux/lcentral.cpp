#include <iostream>
#include <simpleble/SimpleBLE.h>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace SimpleBLE;

#define OSIFS_UUID_PREFIX "4c9375ebdb5b414696a630b900000000"
#define OSIFS_UUID_PREFIX_LENGTH 12

bool compare_uuid_prefix(string src, string target)
{
    return strncmp(src.c_str(), target.c_str(), OSIFS_UUID_PREFIX_LENGTH) == 0;
}

vector<Peripheral> get_osifs_devices() {

	if (!Adapter::bluetooth_enabled()) {
		cout << "Bluetooth is not enabled!" << endl;
		return {};
	}

      vector<Peripheral> osifs_devices = {};

	Adapter adapter = Adapter::get_adapters()[0];
	adapter.scan_for(3000);

	vector<Peripheral> ps = adapter.scan_get_results();
	for (Peripheral p : ps) {
		for (Service s: p.services()) {
			BluetoothUUID service_uuid = s.uuid();
                  if (compare_uuid_prefix(service_uuid, OSIFS_UUID_PREFIX))
                  {
                        cout << "Match" << service_uuid << endl;
                        osifs_devices.push_back(p);
                  }
            }
      }

      return osifs_devices;
}

int main(int argc, char **argv)
{
    vector<Peripheral> ps = get_osifs_devices();
    for (Peripheral p : ps) {
      cout << p.identifier() << endl;
    }

    return 0;
}
