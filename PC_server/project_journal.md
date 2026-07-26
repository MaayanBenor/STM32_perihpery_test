# STM32 Periphery Check - PC Server: Project Journal


## Completed Work

- Entry point set up in [main.c](main.c)
- UDP interface declared in [udp.h](udp.h)
- UDP socket init, receive, and send logic implemented in [udp.c](udp.c)
- Ethernet protocol packet structs defined in [eth_protocol.h](eth_protocol.h)
- Ethernet communication is working and using the protocols in [eth_protocol.h](eth_protocol.h)
- Build automation configured in [Makefile](Makefile)
- Doxygen-style documentation added across source and header files

## Implemented Behavior

1. UDP socket is created and bound to port 5000 in [udp.c](udp.c#L32).
2. A test packet is sent to the STM target at 10.0.1.101 in [udp.c](udp.c#L86).
3. One UDP packet is received and sent to main [udp.c](udp.c#L65).
4. The packet is saved to file and all saved packets are printed.
5. UDP socket is closed properly and used resources are freed. in [udp.c](udp.c#L102)

## Protocol Foundations

- Packed request structure: [eth_protocol.h](eth_protocol.h#L12)
- Packed response structure: [eth_protocol.h](eth_protocol.h#L24)

## Notes
### TODO:
- make it a CLI
- check which flags are activated through the args and appropriately handle them.
- clean main
- check pdf for extra TODOs


### At the very end of the project:
- remove \__FILE__ AND \__LINE__ from errors that the user might see. leave the rest that only developers will see.
    