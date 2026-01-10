#include "../include/instructions.h"

byte fetch(MOS6502 *cpu) {
	if (!(lookup[cpu->opcode].mode == addr_imp)) {
		cpu->fetched_data = cpu_read(cpu, cpu->addr_abs);
	}
	return cpu->fetched_data;
}

byte addr_imp(MOS6502 *cpu) {
	cpu->fetched_data = cpu->a;
	return 0;
}

byte addr_imm(MOS6502 *cpu) {
	cpu->addr_abs = cpu->pc++;
	return 0;
}

byte addr_zp0(MOS6502 *cpu) {
	cpu->addr_abs = cpu_read(cpu, cpu->pc);
	cpu->pc++;
	cpu->addr_abs &= 0x00FF;
	return 0;
}

byte addr_abs(MOS6502 *cpu) {
	word lo = cpu_read(cpu, cpu->pc);
	cpu->pc++;
	word hi = cpu_read(cpu, cpu->pc);
	cpu->pc++;
	cpu->addr_abs = (hi << 8) | lo;
	return 0;
}

byte op_lda(MOS6502 *cpu) {
	fetch(cpu);
	cpu->a = cpu->fetched_data;
	cpu_set_zn_flags(cpu, cpu->a);
	return 1;
}

byte op_ldx(MOS6502 *cpu) {
	fetch(cpu);
	cpu->x = cpu->fetched_data;
	cpu_set_zn_flags(cpu, cpu->x);
	return 1;
}

byte op_ldy(MOS6502 *cpu) {
	fetch(cpu);
	cpu->y = cpu->fetched_data;
	cpu_set_zn_flags(cpu, cpu->y);
	return 1;
}

byte op_pha(MOS6502 *cpu) {
	cpu_write(cpu, STACK_BASE_ADDR + cpu->sp, cpu->a);
	cpu->sp--;
	return 0;
}

byte op_pla(MOS6502 *cpu) {
	cpu->sp++;
	cpu->a = cpu_read(cpu, STACK_BASE_ADDR + cpu->sp);

	cpu_set_zn_flags(cpu, cpu->a);
	return 0;
}

byte op_php(MOS6502 *cpu) {
	cpu_write(cpu, STACK_BASE_ADDR + cpu->sp, cpu->status | FLAG_B | FLAG_U);
	cpu->sp--;
	return 0;
}

byte op_plp(MOS6502 *cpu) {
	cpu->sp++;
	cpu->status = cpu_read(cpu, STACK_BASE_ADDR + cpu->sp);

	cpu->status |= FLAG_U;

	return 0;
}

byte op_sta(MOS6502 *cpu) {
	cpu_write(cpu, cpu->addr_abs, cpu->a);
	return 0;
}

byte op_stx(MOS6502 *cpu) {
	cpu_write(cpu, cpu->addr_abs, cpu->x);
	return 0;
}

byte op_sty(MOS6502 *cpu) {
	cpu_write(cpu, cpu->addr_abs, cpu->y);
	return 0;
}

byte op_nop(MOS6502 *cpu) {
	(void)cpu;
	return 0;
}

byte op_brk(MOS6502 *cpu) {
	cpu->status |= FLAG_B;
	printf("BRK Executed\n");
	return 0;
}

byte op_xxx(MOS6502 *cpu) {
	printf("Illegal Opcode: %02X\n", cpu->opcode);
	return 0;
}
