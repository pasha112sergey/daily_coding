import bluetooth_constants
import bluetooth_exceptions
import ipaddress
import dbus
import dbus.exceptions
import dbus.service
import dbus.mainloop.glib
import sys
import uuid
import socket
from gi.repository import GLib

sys.path.insert(0, '.')

bus = None
adapter_path = None
adv_mgr_interface = None
connected = 0
OSIFS_UUID = "4c9375eb-db5b-4146-96a6-30b900000000"


def get_hostname():
      bus = dbus.SystemBus()
      hostnameInterface = dbus.Interface(bus.get_object("org.freedesktop.hostname1", "/org/freedesktop/hostname1"), "org.freedesktop.DBus.Properties")
      hostname = hostnameInterface.Get("org.freedesktop.hostname1", "Hostname")
      print(hostname)
      return hostname

def get_ip():
      hostname = socket.gethostname()
      local_ip = socket.gethostbyname(hostname)
      return local_ip

def create_service_uuid():
      i = int(OSIFS_UUID.replace("-", ""), 16)
      ip = int(ipaddress.ip_address(get_ip()))
      id = hex(ip | i)
      res = uuid.UUID(id[2:])
      return res.__str__()



class Advertisement(dbus.service.Object):
      PATH_BASE = '/org/bluez/ldsg/advertisement'

      def __init__(self, bus, index, advertising_type):
            self.path = self.PATH_BASE + str(index)
            self.bus = bus
            self.ad_type = advertising_type
            self.service_uuids = [create_service_uuid()]
            print("Created uuid: ", create_service_uuid())
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

def unregister_ad_cb():
      print("Advertisement unregistered")

def register_ad_error_cb(e):
      print("Error: failed to register advertisement " + str(e))
      mainloop.quit()

def unregister_ad_error_cb(e):
      print('Error: failed to unregister advertisement: ' + str(e))
      mainloop.quit()

def start_advertising():
      global adv
      global adv_mgr_interface
      try:
            print('Registering advertisement: ', adv.get_path())
            adv_mgr_interface.RegisterAdvertisement(adv.get_path(), {}, reply_handler = register_ad_cb, error_handler=register_ad_error_cb)
      except bluetooth_exceptions.FailedException:
            print("Error re-registering the advertisement")

def stop_advertising():
      global adv
      global adv_mgr_interface
      print('Unregistering advertisement: ', adv.get_path())
      adv_mgr_interface.UnregisterAdvertisement(adv.get_path(), reply_handler=unregister_ad_cb, error_handler=unregister_ad_error_cb)


def set_connected_status(status):
      global connected
      if (status == 1):
            print("connected")
            connected = 1
            stop_advertising()
      else:
            print("disconnected")
            connected = 0
            start_advertising()

def properties_changed(interface, changed, invalidated, path):
      if (interface == bluetooth_constants.DEVICE_INTERFACE):
            if ("Connected" in changed):
                  set_connected_status(changed['Connected'])

def interfaces_added(path, interfaces):
      if bluetooth_constants.DEVICE_INTERFACE in interfaces:
            properties = interfaces[bluetooth_constants.DEVICE_INTERFACE]
            if ("Connected" in properties):
                  set_connected_status(properties["Connected"])

dbus.mainloop.glib.DBusGMainLoop(set_as_default = True)
bus = dbus.SystemBus()

bus.add_signal_receiver(properties_changed, dbus_interface= bluetooth_constants.DBUS_PROPERTIES, signal_name = "PropertiesChanged", path_keyword ="path")
bus.add_signal_receiver(interfaces_added, dbus_interface=bluetooth_constants.DBUS_OM_IFACE, signal_name = "InterfacesAdded")

adapter_path = "/org/bluez/hci0" 
print(adapter_path)

adv_mgr_interface = dbus.Interface(bus.get_object(bluetooth_constants.BLUEZ_SERVICE_NAME, adapter_path), bluetooth_constants.ADVERTISING_MANAGER_INTERFACE)
adv = Advertisement(bus, 0, 'peripheral')
start_advertising()

mainloop = GLib.MainLoop()
mainloop.run()
