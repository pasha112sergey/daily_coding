# Organizing ideas and things to do

## Immediate next steps

- [ ] Figure out dBus library
- [ ] Refactor to C++
- [ ] Clean up code and update it to best practices
- [x] Separate discovery functionality to different OS's
- [x] Test broadcasting on subnet instead of 255.255.255.255 find the subnet you are on and mask .255 to broadcast properly - fail
- [x] Test outgoing packets using Wireshark
- [x] Add functionality to remove hosts (on exit)
- [x] Add signaling/semaphore functionality that automatically prints the hosts when new host information becomes available instead of manual 'L' input
- [x] Add reception of identity packets
- [x] Troubleshoot self-IP reporting

## Necessary features (next section of the project)

- [ ] Add functionality to select files
- [ ] Create tests
- [ ] Documentation
- [ ] TUI creation
- [ ] Functionality to modify your discoverability

## Leads for BLE

- [ ] DBus library and python for linux - currently not getting picked up by macOs
- [ ] gobbledegook library
- [ ] own low-level blueZ implementation