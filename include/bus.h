#ifndef __BUS_H
#define __BUS_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "platform.h"

typedef struct MOS6502 MOS6502;
typedef uint8_t byte;
typedef uint16_t word;

#define MEM_SIZE 0x10000

#define ADDR_ZP_START    0x0000
#define ADDR_ZP_END      0x00FF
#define ADDR_STACK_START 0x0100
#define ADDR_STACK_END   0x01FF

#define ADDR_RAM_START  0x0000
#define ADDR_RAM_END    0x7FFF
#define ADDR_RAM_MIRROR 0x07FF

#define ADDR_IO_START   0xC000
#define ADDR_IO_END     0xC0FF
#define ADDR_IO_KBD     0xC000
#define ADDR_IO_KBDSTRB 0xC010
#define ADDR_IO_OUTPUT  0xFF00

#define ADDR_ROM_START    0x8000
#define ADDR_ROM_END      0xFFFF
#define ADDR_MONITOR_ROM  0xF800
#define ADDR_RESET_VECTOR 0xFFFC
#define ADDR_IRQ_VECTOR   0xFFFE
#define ADDR_NMI_VECTOR   0xFFFA

typedef struct Bus {
    byte *ram;
    byte rom_locked;
    Platform *platform;
} Bus;

void bus_init(Bus *bus, Platform *platform);
void bus_destroy(Bus *bus);
void bus_load_data(Bus *bus, word addr, const byte *data, int size);
void bus_tick(Bus *bus, int cycles);

static inline byte bus_read(Bus *bus, word addr, int read_only)
{
    (void)read_only;

    if (bus->platform && bus->platform->read_hook) {
        byte data;
        if (bus->platform->read_hook(bus, addr, &data)) {
            return data;
        }
    }

    return bus->ram[addr];
}

static inline void bus_write(Bus *bus, word addr, byte data)
{
    if (bus->platform && bus->platform->write_hook) {
        if (bus->platform->write_hook(bus, addr, data)) {
            return;
        }
    }

    if (addr >= ADDR_ROM_START && bus->rom_locked) {
        return;
    }

    bus->ram[addr] = data;
}

#endif // __BUS_H
