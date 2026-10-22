#include "platform_basic.h"
#include "../include/bus.h"
#include <stdio.h>
#include <stdlib.h>

static int basic_read_hook(Bus *bus, word addr, byte *data)
{
    (void)bus;
    (void)addr;
    (void)data;
    return 0;
}

static int basic_write_hook(Bus *bus, word addr, byte data)
{
    (void)bus;

    if (addr == 0xFF00) {
        putchar(data);
        fflush(stdout);
        return 1;
    }

    return 0;
}

static void basic_init(Bus *bus)
{
    (void)bus;
}

static void basic_destroy(Bus *bus)
{
    (void)bus;
}

static void basic_tick(Bus *bus, int cycles)
{
    (void)bus;
    (void)cycles;
}

static Platform basic_platform = {.name = "basic platform",
                                  .init = basic_init,
                                  .destroy = basic_destroy,
                                  .read_hook = basic_read_hook,
                                  .write_hook = basic_write_hook,
                                  .tick = basic_tick,
                                  .platform_data = NULL};

Platform *platform_basic_create(void)
{
    return &basic_platform;
}

void platform_basic_free(Platform *platform)
{
    (void)platform;
}
