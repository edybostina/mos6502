#include "../include/instructions.h"

byte fetch(MOS6502 *cpu) {
	if (!(lookup[cpu->opcode].mode == addr_imp)) {
		cpu->fetched_data = cpu_read(cpu, cpu->addr_abs);
	}
	return cpu->fetched_data;
}

void branch(MOS6502 *cpu) {
	cpu->cycles++;

	cpu->addr_abs = cpu->pc + cpu->addr_rel;

	if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
		cpu->cycles++;

	cpu->pc = cpu->addr_abs;
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

byte addr_rel(MOS6502 *cpu) {
	cpu->addr_rel = cpu_read(cpu, cpu->pc);
	cpu->pc++;

	if (cpu->addr_rel & 0x80) {
		cpu->addr_rel |= 0xFF00;
	}

	return 0;
}

byte addr_ind(MOS6502 *cpu) {
	word ptr_lo = cpu_read(cpu, cpu->pc);
	cpu->pc++;
	word ptr_hi = cpu_read(cpu, cpu->pc);
	cpu->pc++;

	word ptr = (ptr_hi << 8) | ptr_lo;
	cpu->addr_abs = (cpu_read(cpu, ptr + 1) << 8) | cpu_read(cpu, ptr);
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

// ----------------------------------------------------------------
// ----------------------------------------------------------------

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

byte op_jmp(MOS6502 *cpu) {
	cpu->pc = cpu->addr_abs;
	return 0;
}

byte op_bpl(MOS6502 *cpu) {
	if ((cpu->status & FLAG_N) == 0)
		branch(cpu);
	return 0;
}

byte op_bmi(MOS6502 *cpu) {
	if (cpu->status & FLAG_N)
		branch(cpu);
	return 0;
}

byte op_bvc(MOS6502 *cpu) {
	if ((cpu->status & FLAG_V) == 0)
		branch(cpu);
	return 0;
}

byte op_bvs(MOS6502 *cpu) {
	if (cpu->status & FLAG_V)
		branch(cpu);
	return 0;
}

byte op_bcc(MOS6502 *cpu) {
	if ((cpu->status & FLAG_C) == 0)
		branch(cpu);
	return 0;
}

byte op_bcs(MOS6502 *cpu) {
	if (cpu->status & FLAG_C)
		branch(cpu);
	return 0;
}

byte op_bne(MOS6502 *cpu) {
	if ((cpu->status & FLAG_Z) == 0)
		branch(cpu);
	return 0;
}

byte op_beq(MOS6502 *cpu) {
	if (cpu->status & FLAG_Z)
		branch(cpu);
	return 0;
}

byte op_adc(MOS6502 *cpu) {
	fetch(cpu);

	word temp = (word)cpu->a + (word)cpu->fetched_data + (word)(cpu->status & FLAG_C);

	if (temp > 255)
		cpu->status |= FLAG_C;
	else
		cpu->status &= ~FLAG_C;

	cpu_set_zn_flags(cpu, (byte)(temp & 0x00FF));

	if ((~((word)cpu->a ^ (word)cpu->fetched_data) & ((word)cpu->a ^ temp)) & 0x0080) {
		cpu->status |= FLAG_V;
	} else {
		cpu->status &= ~FLAG_V;
	}

	cpu->a = temp & 0x00FF;

	return 1;
}

byte op_sbc(MOS6502 *cpu) {
	fetch(cpu);

	word val = (word)cpu->fetched_data ^ 0x00FF;

	word temp = (word)cpu->a + val + (word)(cpu->status & FLAG_C);

	if (temp > 255)
		cpu->status |= FLAG_C;
	else
		cpu->status &= ~FLAG_C;

	cpu_set_zn_flags(cpu, (byte)(temp & 0x00FF));

	if ((~((word)cpu->a ^ val) & ((word)cpu->a ^ temp)) & 0x0080) {
		cpu->status |= FLAG_V;
	} else {
		cpu->status &= ~FLAG_V;
	}

	cpu->a = temp & 0x00FF;

	return 1;
}

byte op_clc(MOS6502 *cpu) {
	cpu->status &= ~FLAG_C;
	return 0;
}
byte op_sec(MOS6502 *cpu) {
	cpu->status |= FLAG_C;
	return 0;
}

byte op_cli(MOS6502 *cpu) {
	cpu->status &= ~FLAG_I;
	return 0;
}
byte op_sei(MOS6502 *cpu) {
	cpu->status |= FLAG_I;
	return 0;
}

byte op_clv(MOS6502 *cpu) {
	cpu->status &= ~FLAG_V;
	return 0;
}

byte op_cld(MOS6502 *cpu) {
	cpu->status &= ~FLAG_D;
	return 0;
}
byte op_sed(MOS6502 *cpu) {
	cpu->status |= FLAG_D;
	return 0;
}

byte op_nop(MOS6502 *cpu) {
	(void)cpu;
	return 0;
}

byte op_brk(MOS6502 *cpu) {
	cpu->status |= FLAG_B;
	return 0;
}

byte op_xxx(MOS6502 *cpu) {
	(void)cpu;
	return 0;
}
