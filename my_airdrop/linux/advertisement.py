import bluetooth_constants
import bluetooth_exceptions

import dbus
import dbus.exceptions
import dbus.service
import dbus.mainloop.glib
import sys
from gi.repository import GLib

sys.path.insert(0, '.')

bus = None
adapter_path = None
adv_mgr_interface = None

def get_hostname():
      bus = dbus.SystemBus()
      hostnameInterface = dbus.Interface(bus.get_object("org.freedesktop.hostname1", "/org/freedesktop/hostname1"), "org.freedesktop.DBus.Properties")
      hostname = hostnameInterface.Get("org.freedesktop.hostname1", "Hostname")
      print(hostname)
      return hostname


class Advertisement(dbus.service.Object):
      PATH_BASE = '/org/bluez/ldsg/advertisement'

      def __init__(self, bus, index, advertising_type):
            self.path = self.PATH_BASE + str(index)
            self.bus = bus
            self.ad_type = advertising_type
            self.service_uuids = None
            self.manufacturer_data = None
            self.solicit_uuids = None
            self.service_data = None
            self.local_name = "OSIFS_" + get_hostname()
            self.include_tx_power = False
            self.data = None
            self.discoverable = True
            dbus.service.Object.__init__(self, bus, self.path)

      def get_properties(self):
            properties = dict()
            properties['Type'] = self.ad_type

            if self.service_uuids is not None:
                  properties['ServiceUUIDs'] = dbus.Array(self.service_uuids, signature='s')
            
            if self.manufacturer_data is not None:
                  properties['ManufacturingData'] = dbus.Dictionary(self.manufacturer_data, signature='qv')
            
            if self.service_data is not None:
                  properties['ServiceData'] = dbus.Dictionary(self.service_data, signature = 'sv')

            if self.local_name is not None:
                  properties['LocalName'] = dbus.String(self.local_name)

            if self.discoverable is not None and self.discoverable == True:
                  properties["Discoverable"] = dbus.Boolean(self.discoverable)

            if self.include_tx_power:
                  properties['Includes'] = dbus.Array(["tx_power"], signature="s")
            
            if self.data is not None:
                  properties['Data'] = dbus.Dictionary(self.data, signature="yv")
            
            print(properties)

            return {bluetooth_constants.ADVERTISING_MANAGER_INTERFACE : properties}



      def get_path(self):
            return dbus.ObjectPath(self.path)



      @dbus.service.method(bluetooth_constants.DBUS_PROPERTIES, in_signature = 's', out_signature='a{sv}')

      def GetAll(self, interface):
            if interface != bluetooth_constants.ADVERTISEMENT_INTERFACE:
                  raise bluetooth_exceptions.InvalidArgsException()
            return self.get_properties()[bluetooth_constants.ADVERTISING_MANAGER_INTERFACE]


      @dbus.service.method(bluetooth_constants.ADVERTISING_MANAGER_INTERFACE, in_signature="", out_signature="")
      def Release(self):
            print("%s Released" % self.path)

def register_ad_cb():
      print('Advertisement registered')

def register_ad_error_cb(e):
      print("Error: failed to register advertisement " + str(e))
      mainloop.quit()

def start_advertising():
      global adv
      global adv_mgr_interface

      print('Registering advertisement: ', adv.get_path())

      adv_mgr_interface.RegisterAdvertisement(adv.get_path(), {}, reply_handler = register_ad_cb, error_handler=register_ad_error_cb)

dbus.mainloop.glib.DBusGMainLoop(set_as_default = True)

bus = dbus.SystemBus()

adapter_path = "/org/bluez/hci0" 

print(adapter_path)


adv_mgr_interface = dbus.Interface(bus.get_object(bluetooth_constants.BLUEZ_SERVICE_NAME, adapter_path), bluetooth_constants.ADVERTISING_MANAGER_INTERFACE)
adv = Advertisement(bus, 0, 'peripheral')
start_advertising()

mainloop = GLib.MainLoop()
mainloop.run()

