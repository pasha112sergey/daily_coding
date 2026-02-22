import Foundation
import CoreBluetooth
import Foundation
import SystemConfiguration

func getLocalIPAddress() -> String? {
    var address: String?

    var ifaddr: UnsafeMutablePointer<ifaddrs>? = nil
    if getifaddrs(&ifaddr) == 0 {
        var ptr = ifaddr
        while ptr != nil {
            defer { ptr = ptr?.pointee.ifa_next }

            let interface = ptr!.pointee
            let addrFamily = interface.ifa_addr.pointee.sa_family

            // IPv4 only
            if addrFamily == UInt8(AF_INET) {

                let name = String(cString: interface.ifa_name)

                // en0 = WiFi on most Macs
                if name == "en0" {
                    var hostname = [CChar](repeating: 0, count: Int(NI_MAXHOST))

                    getnameinfo(
                        interface.ifa_addr,
                        socklen_t(interface.ifa_addr.pointee.sa_len),
                        &hostname,
                        socklen_t(hostname.count),
                        nil,
                        socklen_t(0),
                        NI_NUMERICHOST
                    )

                    address = String(cString: hostname)
                }
            }
        }
        freeifaddrs(ifaddr)
    }
    return address
}

let OSIFS_PREFIX = "4c9375ebdb5b414696a630b900000000"

func generateUUID() -> CBUUID {
    let ip = getLocalIPAddress() ?? "0000"
    let ipDigits = ip.replacingOccurrences(of: ".", with: "")
    let normalizedIP = String(ipDigits.prefix(8)).padding(toLength: 8, withPad: "0", startingAt: 0)

    var rawUUID = OSIFS_PREFIX.lowercased()
    let start = rawUUID.index(rawUUID.startIndex, offsetBy: 24)
    let end = rawUUID.index(start, offsetBy: 8)
    rawUUID.replaceSubrange(start..<end, with: normalizedIP.lowercased())

    let formattedUUID =
        "\(rawUUID.prefix(8))-" +
        "\(rawUUID.dropFirst(8).prefix(4))-" +
        "\(rawUUID.dropFirst(12).prefix(4))-" +
        "\(rawUUID.dropFirst(16).prefix(4))-" +
        "\(rawUUID.dropFirst(20).prefix(12))"

    // print(formattedUUID)
    return CBUUID(string: formattedUUID)
}

final class BLEPeripheral: NSObject, CBPeripheralManagerDelegate {
    private var peripheralManager: CBPeripheralManager!
    private var valueCharacteristic: CBMutableCharacteristic!
    private var value = Data("Hello".utf8)

    private var serviceUUID : CBUUID
    private let charUUID    = CBUUID(string: "87654321-4321-4321-4321-BA0987654321")

    override init() {
        self.serviceUUID = generateUUID()
        super.init()
        print(serviceUUID)
        peripheralManager = CBPeripheralManager(delegate: self, queue: nil)
    }

    func peripheralManagerDidUpdateState(_ peripheral: CBPeripheralManager) {
        guard peripheral.state == .poweredOn else {
            return
        }

        valueCharacteristic = CBMutableCharacteristic(
            type: charUUID,
            properties: [.read, .write, .notify],
            value: nil,
            permissions: [.readable, .writeable]
        )

        let service = CBMutableService(type: serviceUUID, primary: true)
        service.characteristics = [valueCharacteristic]

        peripheralManager.add(service)
    }

    func peripheralManager(_ peripheral: CBPeripheralManager, didAdd service: CBService, error: Error?) {
        if let error = error {
            print("Failed to add service:", error)
            return
        }

        let advertisement: [String: Any] = [
            CBAdvertisementDataLocalNameKey: "MacBLE",
            CBAdvertisementDataServiceUUIDsKey: [serviceUUID]
        ]

        peripheralManager.startAdvertising(advertisement)
        print("Advertising as MacBLE")
    }

    func peripheralManager(_ peripheral: CBPeripheralManager, didReceiveRead request: CBATTRequest) {
        guard request.characteristic.uuid == charUUID else {
            peripheral.respond(to: request, withResult: .attributeNotFound)
            return
        }

        guard request.offset <= value.count else {
            peripheral.respond(to: request, withResult: .invalidOffset)
            return
        }

        request.value = value.subdata(in: request.offset..<value.count)
        peripheral.respond(to: request, withResult: .success)
    }

    func peripheralManager(_ peripheral: CBPeripheralManager, didReceiveWrite requests: [CBATTRequest]) {
        for req in requests where req.characteristic.uuid == charUUID {
            if let data = req.value {
                value = data
                print("New value:", String(decoding: data, as: UTF8.self))

                peripheralManager.updateValue(value, for: valueCharacteristic, onSubscribedCentrals: nil)
            }
        }

        for req in requests {
            peripheral.respond(to: req, withResult: .success)
        }
    }

    func peripheralManager(_ peripheral: CBPeripheralManager,
                           central: CBCentral,
                           didSubscribeTo characteristic: CBCharacteristic) {
        print("Central subscribed:", central.identifier)
    }

    func peripheralManager(_ peripheral: CBPeripheralManager,
                           central: CBCentral,
                           didUnsubscribeFrom characteristic: CBCharacteristic) {
        print("Central unsubscribed:", central.identifier)
    }
}

let blue = BLEPeripheral()
RunLoop.main.run()