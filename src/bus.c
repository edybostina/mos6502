#include "../include/bus.h"
#include <stdio.h>

void bus_init(Bus *bus) {
	for (int i = 0; i < 2048; i++) {
		bus->ram[i] = 0;
	}
}
void bus_write(Bus *bus, uint16_t addr, uint8_t data) {
	if (addr >= ADDR_RAM_START && addr <= ADDR_RAM_END) {
		bus->ram[addr & ADDR_RAM_MIRROR] = data;
	} else if (addr >= ADDR_PPU_START && addr <= ADDR_PPU_END) {
		// ppu_write(addr & ADDR_PPU_MIRROR, data);
		printf("PPU Write: 0x%04X\n", addr);
	} else if (addr >= ADDR_APU_START && addr <= ADDR_APU_END) {
		printf("APU Write: 0x%04X\n", addr);
	} else if (addr >= ADDR_CART_START && addr <= ADDR_CART_END) {
		// cart_write(addr, data);
	}
}

uint8_t bus_read(Bus *bus, uint16_t addr, int read_only) {
	uint8_t data = 0;

	if (addr >= ADDR_RAM_START && addr <= ADDR_RAM_END) {
		data = bus->ram[addr & ADDR_RAM_MIRROR];
	} else if (addr >= ADDR_PPU_START && addr <= ADDR_PPU_END) {
		// data = ppu_read(addr & ADDR_PPU_MIRROR);
	} else if (addr >= ADDR_CART_START && addr <= ADDR_CART_END) {
		// data = cart_read(addr);
	}

	return data;
}
