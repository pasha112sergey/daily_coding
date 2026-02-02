from bluez_peripheral.gatt.service import Service
from bluez_peripheral.gatt.characteristic import characteristic, CharacteristicFlags as CharFlags
from bluez_peripheral.util import get_message_bus, is_bluez_available

import asyncio
import struct

OSIFS_UUID = "4c9375eb-db5b-4146-96a6-30b96df27987"

class OSIFS_Service(Service):
      def __init__(self):
            self.ip = "255.255.255.255"
            super().__init__(uuid=OSIFS_UUID)
      
      # getter of self.ip - no setter bc no need for it
      @characteristic(OSIFS_UUID, CharFlags.READ)
      def ip(self, options):
            return bytes(self.ip, "utf-8")
      
      # setter of self.ip
      @ip.setter
      def ip(self, value, options):
            self.ip = value
      
      
adInterfacePath = ".org.blueZ.LEAdvertisement1"

async def main():
      discoveryService = OSIFS_Service()

      bus = await get_message_bus()

      if not await is_bluez_available(bus):
            print("BlueZ not available, returning")
            exit(0)

      print("BlueZ available")

      await discoveryService.register(bus, path=adInterfacePath)
      print("registered bus")

      await bus.wait_for_disconnect()


if __name__ == "__main__":
      asyncio.run(main())

