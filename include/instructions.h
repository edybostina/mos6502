#ifndef __INSTRUCTIONS_H
#define __INSTRUCTIONS_H

#include "mos6502.h"

struct Instruction {
	char *name;
	byte (*operate)(MOS6502 *);
	byte (*mode)(MOS6502 *);
	byte cycles;
};

extern struct Instruction lookup[256];

byte op_lda(MOS6502 *cpu);
byte op_ldx(MOS6502 *cpu);
byte op_ldy(MOS6502 *cpu);
byte op_nop(MOS6502 *cpu);
byte op_brk(MOS6502 *cpu);
byte op_xxx(MOS6502 *cpu);	// :) illegal opcodes

byte addr_imp(MOS6502 *cpu);  // implied
byte addr_imm(MOS6502 *cpu);  // immediate
byte addr_zp0(MOS6502 *cpu);  // zero page
byte addr_abs(MOS6502 *cpu);  // absolute

#endif	// !__INSTRUCTIONS_H
