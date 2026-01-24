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
byte op_pha(MOS6502 *cpu);
byte op_pla(MOS6502 *cpu);
byte op_php(MOS6502 *cpu);
byte op_plp(MOS6502 *cpu);
byte op_sta(MOS6502 *cpu);
byte op_stx(MOS6502 *cpu);
byte op_sty(MOS6502 *cpu);
byte op_jmp(MOS6502 *cpu);
byte op_bpl(MOS6502 *cpu);
byte op_bmi(MOS6502 *cpu);
byte op_bvc(MOS6502 *cpu);
byte op_bvs(MOS6502 *cpu);
byte op_bcc(MOS6502 *cpu);
byte op_bcs(MOS6502 *cpu);
byte op_bne(MOS6502 *cpu);
byte op_beq(MOS6502 *cpu);
byte op_adc(MOS6502 *cpu);
byte op_sbc(MOS6502 *cpu);
byte op_clc(MOS6502 *cpu);
byte op_sec(MOS6502 *cpu);
byte op_cli(MOS6502 *cpu);
byte op_sei(MOS6502 *cpu);
byte op_clv(MOS6502 *cpu);
byte op_cld(MOS6502 *cpu);
byte op_sed(MOS6502 *cpu);
byte op_and(MOS6502 *cpu);
byte op_ora(MOS6502 *cpu);
byte op_eor(MOS6502 *cpu);
byte op_asl(MOS6502 *cpu);
byte op_lsr(MOS6502 *cpu);
byte op_rol(MOS6502 *cpu);
byte op_ror(MOS6502 *cpu);
byte op_cmp(MOS6502 *cpu);
byte op_cpx(MOS6502 *cpu);
byte op_cpy(MOS6502 *cpu);
byte op_inc(MOS6502 *cpu);
byte op_dec(MOS6502 *cpu);
byte op_inx(MOS6502 *cpu);
byte op_dex(MOS6502 *cpu);
byte op_iny(MOS6502 *cpu);
byte op_jsr(MOS6502 *cpu);
byte op_rts(MOS6502 *cpu);
byte op_rti(MOS6502 *cpu);
byte op_dey(MOS6502 *cpu);
byte op_bit(MOS6502 *cpu);
byte op_nop(MOS6502 *cpu);
byte op_brk(MOS6502 *cpu);
byte op_xxx(MOS6502 *cpu);	// :) illegal opcodes

byte addr_imp(MOS6502 *cpu);  // implied
byte addr_imm(MOS6502 *cpu);  // immediate
byte addr_zp0(MOS6502 *cpu);  // zero page
byte addr_abs(MOS6502 *cpu);  // absolute
byte addr_rel(MOS6502 *cpu);  // relative
byte addr_ind(MOS6502 *cpu);  // indirect
byte addr_zp0x(MOS6502 *cpu);
byte addr_zp0y(MOS6502 *cpu);
byte addr_absx(MOS6502 *cpu);
byte addr_absy(MOS6502 *cpu);
byte addr_indx(MOS6502 *cpu);
byte addr_indy(MOS6502 *cpu);
#endif	// !__INSTRUCTIONS_H
