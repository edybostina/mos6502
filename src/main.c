#include "../include/mos6502.h"
#include "../include/bus.h"
#include "../include/opcodes.h"
#include <stdio.h>

int main() {
	MOS6502 cpu;
	Bus bus;

	bus_init(&bus);
	cpu_connect_bus(&cpu, &bus);

	byte program[] = {0xA2, 0x00,  // LDX #00       (Initialize Index X = 0)

						 // LOOP START (Address 0x8002)
						 OP_LDA_ABS_X, 0x0F, 0x80,	// LDA $800F,X   (Load char from String + X)
						 OP_BEQ, 0x07,  // BEQ +7        (If Char is 0, jump to BRK)

						 OP_STA_ABS, 0x00, 0xFF,	// STA $FF00     (Write to Output Port -> Prints to Console)

						 OP_INX,	// INX           (Increment Index)
						 OP_JMP_ABS, 0x02, 0x80,	// JMP $8002     (Jump back to LOOP START)

						 OP_BRK,	// BRK           (Stop)

						 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', '\n', 0x00};

	bus_load_data(&bus, 0x8000, program, sizeof(program));

	byte reset_vector[] = {0x00, 0x80};
	bus_load_data(&bus, 0xFFFC, reset_vector, 2);

	cpu_reset(&cpu);

	printf("--- Computer Power On ---\n");

	while (1) {
		cpu_execute(&cpu, 1);
		if (bus_read(&bus, cpu.pc, 1) == OP_BRK) {
			break;
		}
	}

	printf("--- CPU Halted ---\n");
	return 0;
}
