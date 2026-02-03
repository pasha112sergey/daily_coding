import dbus
import dbus.exceptions
import dbus.service
import bluetooth_constants
import bluetooth_exceptions
import sys
sys.path.insert(0, '.')

class Service(dbus.service.Object):
      """
      org.bluez.GattService1 interface implementation
      """

      def __init__(self, bus, path_base, index, uuid, primary):
            self.path = path_base + "/service_" + str(index)
            self.bus = bus
            self.uuid = uuid
            self.primary = primary
            self.characteristics = []
            dbus.service.Object.__init__(self, bus, self.path)

      @dbus.service.method(bluetooth_constants.DBUS_PROPERTIES, in_signature='s', out_signature='a{sv}')
      def GetAll(self, interface):
            if interface != bluetooth_constants.GATT_SERVICE_INTERFACE:
                  raise bluetooth_exceptions.InvalidArgsException()

            return self.getProperties()[bluetooth_constants.GATT_SERVICE_INTERFACE]

      def getProperties(self):
            return {
                  bluetooth_constants.GATT_SERVICE_INTERFACE: {
                        "UUID": self.uuid,
                        'Primary': self.primary,
                        'Characteristics': dbus.Array(self.get_characteristic_paths(), signature='o')
                  }
            }

      def get_path(self):
            return dbus.ObjectPath(self.path)
      
      def add_characteristic(self, characteristic):
            self.characteristics.append(characteristic)
      
      def get_characteristic_paths(self):
            res = []
            for chrc in self.characteristics:
                  res.append(chrc.get_path())
            return res

      def get_characteristics(self):
            return self.characteristics

class Characteristic(dbus.service.Object)