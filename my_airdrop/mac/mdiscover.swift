import CoreBluetooth

let OSIFS_UUID = CBUUID(string: "4c9375eb-db5b-4146-96a6-30b96df27987")

class BluetoothViewModel: NSObject, ObservableObject {
    private var centralManager: CBCentralManager?
    private var peripherals: [CBPeripheral] = []
    @Published var peripheralNames: [String] = []

    override init()
    {
        super.init()
        self.centralManager = CBCentralManager(delegate: self, queue: .main)
    }
}

extension BluetoothViewModel: CBCentralManagerDelegate {
    func centralManagerDidUpdateState(_ central: CBCentralManager)
    {
        if central.state == .poweredOn {
            print("Bluetooth ON. Scanning...")
            self.centralManager?.scanForPeripherals(withServices: [OSIFS_UUID])
        } else {
            print("Bluetooth is not available. \(central.state.rawValue)")
        }
    }

    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber)
    {
         print("ADV: ", advertisementData)
        let advUUIDs = (advertisementData[CBAdvertisementDataServiceUUIDsKey] as? [CBUUID]) ?? []
        let matches = advUUIDs.contains(OSIFS_UUID)

        if matches && !peripherals.contains(peripheral)
        {
            self.peripherals.append(peripheral)
            self.peripheralNames.append(peripheral.name ?? "unnamed device")
            print("Discovered: \(peripheral.name ?? "unnamed device"), UUID: \(peripheral.identifier.uuidString)")
        }
    }
}

let scanner = BluetoothViewModel()
RunLoop.main.run()