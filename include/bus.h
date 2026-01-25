#ifndef __BUS_H
#define __BUS_H

#include <stdint.h>

#define ADDR_RAM_START 0x0000
#define ADDR_RAM_END 0x1FFF
#define ADDR_RAM_MIRROR 0x07FF

#define ADDR_PPU_START 0x2000
#define ADDR_PPU_END 0x3FFF
#define ADDR_PPU_MIRROR 0x0007

#define ADDR_APU_START 0x4000
#define ADDR_APU_END 0x4017

#define ADDR_CART_START 0x4020
#define ADDR_CART_END 0xFFFF

typedef struct MOS6502 MOS6502;
typedef uint8_t byte;
typedef uint16_t word;

typedef struct {
	byte memory[65536];
} Bus;

void bus_init(Bus *bus);
void bus_write(Bus *bus, word addr, byte data);
byte bus_read(Bus *bus, word addr, int read_only);
void bus_load_data(Bus *bus, word addr, byte *data, int size);

#endif	// __BUS_H
