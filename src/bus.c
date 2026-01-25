#include "../include/bus.h"
#include <stdio.h>
#include <string.h>

void bus_init(Bus *bus) {
	memset(bus->memory, 0, 65536);
}

void bus_write(Bus *bus, word addr, byte data) {
	if (addr == 0xFF00) {
		printf("%c", data);
		return;
	}

	if (addr < 0x8000) {
		bus->memory[addr] = data;
	}
}

byte bus_read(Bus *bus, word addr, int read_only) {
	return bus->memory[addr];
}

void bus_load_data(Bus *bus, word addr, byte *data, int size) {
	for (int i = 0; i < size; i++) {
		bus->memory[addr + i] = data[i];
	}
}
