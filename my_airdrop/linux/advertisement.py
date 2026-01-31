import dbus
import dbus.service
import dbus.mainloop.glib
from gi.repository import GLib

LE_ADVERTISEMENT_IFACE = "org.bluez.LEAdvertisement1"
DBUS_PROP_IFACE = "org.freedesktop.DBus.Properties"
OSIFS_UUID = "4c9375eb-db5b-4146-96a6-30b96df27987"

def register_ad_cb():
    print("Ad registered")

def register_ad_error_cb(error):
    print("Failed to register: ", error)

class Advertisement(dbus.service.Object):
    PATH_BASE = "/com/osifs/advertisement"

    def __init__(self, bus, index, adv_type, base_path=None):
        self.path = (base_path or self.PATH_BASE) + str(index)
        self.bus = bus
        self.adv_type = adv_type
        self.service_uuids = []
        self.manufacturer_data = {}
        self.local_name = None
        self.include_tx_power = False

        super().__init__(bus, object_path=self.path)

    def get_properties(self):
        props = {
            "Type": self.adv_type,
            "ServiceUUIDs": dbus.Array(self.service_uuids, signature="s"),
            "ManufacturerData": dbus.Dictionary(self.manufacturer_data, signature="qv"),
        }

        if self.local_name:
            props["LocalName"] = dbus.String(self.local_name)

        if self.include_tx_power:
            props["Includes"] = dbus.Array(["tx-power"], signature="s")

        return {LE_ADVERTISEMENT_IFACE: props}

    def get_path(self):
        return dbus.ObjectPath(self.path)

    def add_service_uuid(self, uuid):
        self.service_uuids.append(uuid)

    def add_local_name(self, name):
        self.local_name = name

    def add_manufacturer_data(self, company_id, data):
        self.manufacturer_data[dbus.UInt16(company_id)] = dbus.Array(data, signature="y")

    @dbus.service.method(DBUS_PROP_IFACE, in_signature="ss", out_signature="v")
    def Get(self, interface, prop):
        return self.get_properties()[interface][prop]

    @dbus.service.method(DBUS_PROP_IFACE, in_signature="s", out_signature="a{sv}")
    def GetAll(self, interface):
        return self.get_properties().get(interface, {})

    @dbus.service.method(LE_ADVERTISEMENT_IFACE)
    def Release(self):
        pass



class OSIFS_Advertisement(Advertisement):
      def __init__(self, bus, index):
            Advertisement.__init__(
                  self, bus, index, "peripheral", base_path = "/com/osifs/advertisement/ip"
            )

            self.add_manufacturer_data(
                  0xFFFF, [0x70, 0x74]
            )

            self.add_service_uuid(OSIFS_UUID)
            self.add_local_name("linux")
            self.include_tx_power = True


def main():
      dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
      bus = dbus.SystemBus()
      adp = bus.get_object("org.bluez", '/org/bluez/hci0')

      print(adp)

      adp_props = dbus.Interface(adp, dbus_interface="org.freedesktop.DBus.Properties")
      adp_props.Set("org.bluez.Adapter1", "Powered", dbus.Boolean(1))

      ad_manager = dbus.Interface(adp, "org.bluez.LEAdvertisingManager1")

      ad = OSIFS_Advertisement(bus, 0)


      ad_manager.RegisterAdvertisement(
          ad.get_path(),
          dbus.Dictionary({}, signature="sv"),
          reply_handler=register_ad_cb,
          error_handler=register_ad_error_cb,
      )

      GLib.MainLoop().run()

main()