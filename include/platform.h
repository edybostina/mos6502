#ifndef __PLATFORM_H
#define __PLATFORM_H

#include <stdint.h>

typedef uint8_t byte;
typedef uint16_t word;

typedef struct Bus Bus;
typedef struct Platform Platform;

struct Platform {
	const char *name;

	void (*init)(Bus *bus);
	void (*destroy)(Bus *bus);
	int (*read_hook)(Bus *bus, word addr, byte *data);
	int (*write_hook)(Bus *bus, word addr, byte data);
	void (*tick)(Bus *bus, int cycles);

	void *platform_data;
};

#endif	// __PLATFORM_H
