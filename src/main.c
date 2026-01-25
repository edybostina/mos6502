#include "../include/mos6502.h"
#include "../include/bus.h"
#include <stdio.h>

int main() {
	// 1. Instantiate Components
	MOS6502 cpu;
	Bus bus;

	// 2. Initialize Bus
	bus_init(&bus);

	// 3. Connect Components
	cpu_connect_bus(&cpu, &bus);

	// 4. Reset CPU
	cpu_reset(&cpu);

	// 5. Test Write/Read via Bus
	// Write 0x55 to address 0x0010 (RAM)
	cpu_write(&cpu, 0x0010, 0x55);

	// Read it back
	// Remember: 0x0810 mirrors 0x0010 on this architecture!
	uint8_t val = cpu_read(&cpu, 0x0810);

	printf("Written to 0x0010, Read from 0x0810: 0x%02X\n", val);

	return 0;
}
