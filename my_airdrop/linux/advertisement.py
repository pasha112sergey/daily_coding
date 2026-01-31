from bluez_peripheral.gatt.service import Service
from bluez_peripheral.gatt.characteristic import characteristic, CharacteristicFlags as CharFlags
from bluez_peripheral import get_message_bus

import struct

OSIFS_UUID = "4c9375eb-db5b-4146-96a6-30b96df27987"

class OSFIS_Service(Service):
      def __init__(self):
            self.ip = "255.255.255.255"
            super().__init__(uuid=OSIFS_UUID, primary=True, Service=None)
      
      # getter of self.ip - no setter bc no need for it
      @characteristic(OSIFS_UUID, CharFlags.READ)
      def ip(self, options):
            return bytes(self.ip, "utf-8")
      
      # setter of self.ip
      @characteristic.setter
      def ip(self, value, options):
            self.ip = value
      
      


async def main():
      discoveryService = Service()
      bus = await get_message_bus()

      await discoveryService.register(bus)

      await bus.wait_for_disconnect()


if __name__ == "__main__":
      asyncio.run(main())

