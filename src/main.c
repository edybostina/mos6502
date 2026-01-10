#include "../include/mos6502.h"
#include "../include/opcodes.h"
#include <stdio.h>

int main() {
	MOS6502 cpu;

	cpu_write(&cpu, 0xFFFC, 0x00);
	cpu_write(&cpu, 0xFFFD, 0x80);
	cpu_reset(&cpu);

	cpu_write(&cpu, 0x8000, OP_SEC);

	// 2. LDA #10
	cpu_write(&cpu, 0x8001, OP_LDA_IMM);
	cpu_write(&cpu, 0x8002, 10);

	// 3. SBC #5
	cpu_write(&cpu, 0x8003, OP_SBC_IMM);
	cpu_write(&cpu, 0x8004, 5);

	// 4. BRK
	cpu_write(&cpu, 0x8005, OP_BRK);

	cpu_execute(&cpu, 15);
	printf("10 - 5 = %d\n", cpu.a);
	return 0;
}
