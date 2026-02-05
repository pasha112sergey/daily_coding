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

int main(int argc, char **argv)
{
    if (!Adapter::bluetooth_enabled())
    {
        cout << "Bluetooth is not enabled" << endl;
        return 1;
    }

    auto adapter = Adapter::get_adapters()[0];
    cout << "Adapter id: " << adapter.identifier() << endl;

    adapter.scan_for(5000);

    vector<Peripheral> ps = adapter.scan_get_results();

    for (Peripheral p : ps)
    {
        for (Service s : p.services())
        {
            BluetoothUUID service_uuid = s.uuid();
            service_uuid.erase(std::remove(service_uuid.begin(), service_uuid.end(), '-'), service_uuid.end());
            cout << "service id stripped: " << service_uuid << endl;
            if (compare_uuid_prefix(service_uuid, OSIFS_UUID_PREFIX))
            {
                cout << "Match: " <<  service_uuid << endl;
            }
        }
    }
}
