#include "../include/mos6502.h"
#include <stdio.h>

int main() {
	MOS6502 cpu;

	cpu_write(&cpu, 0x0010, 0x42);
	cpu_write(&cpu, 0x1234, 0x99);
	cpu_write(&cpu, 0x8000, 0xA5);
	cpu_write(&cpu, 0x8001, 0x10);
	cpu_write(&cpu, 0x8002, 0xAD);
	cpu_write(&cpu, 0x8003, 0x34);	// Low byte
	cpu_write(&cpu, 0x8004, 0x12);	// High byte
	cpu_write(&cpu, 0x8005, 0x00);
	cpu_write(&cpu, 0xFFFC, 0x00);
	cpu_write(&cpu, 0xFFFD, 0x80);
	cpu_reset(&cpu);
	cpu_execute(&cpu, 15);

	return 0;
}
