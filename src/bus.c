#include "../include/bus.h"
#include "../include/platform.h"
#include <stdio.h>
#include <stdlib.h>

void bus_init(Bus *bus, Platform *platform)
{
    bus->ram = (byte *)malloc(MEM_SIZE);
    if (!bus->ram) {
        fprintf(stderr, "FATAL: Failed to allocate memory\n");
        exit(1);
    }

    memset(bus->ram, 0, MEM_SIZE);
    bus->rom_locked = 0;
    bus->platform = platform;

    if (platform && platform->init) {
        platform->init(bus);
    }
}

void bus_destroy(Bus *bus)
{
    if (bus->platform && bus->platform->destroy) {
        bus->platform->destroy(bus);
    }

    if (bus->ram) {
        free(bus->ram);
        bus->ram = NULL;
    }
}

void bus_load_data(Bus *bus, word addr, const byte *data, int size)
{
    if (addr + size > MEM_SIZE) {
        fprintf(stderr, "WARNING: Load exceeds memory bounds\n");
        size = MEM_SIZE - addr;
    }

    memcpy(bus->ram + addr, data, size);
}

void bus_tick(Bus *bus, int cycles)
{
    if (bus->platform && bus->platform->tick) {
        bus->platform->tick(bus, cycles);
    }
}
