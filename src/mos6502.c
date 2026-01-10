#include "../include/mos6502.h"
#include "../include/instructions.h"
#include "../include/opcodes.h"

struct Instruction lookup[256];

void init_lookup_table() {
	for (int i = 0; i < 256; i++) {
		lookup[i] = (struct Instruction){"XXX", op_xxx, addr_imp, 2};
	}

	lookup[OP_BRK] = (struct Instruction){"BRK", op_brk, addr_imp, 7};
	lookup[OP_NOP] = (struct Instruction){"NOP", op_nop, addr_imp, 2};

	lookup[OP_LDA_IMM] = (struct Instruction){"LDA", op_lda, addr_imm, 2};
	lookup[OP_LDA_ZP0] = (struct Instruction){"LDA", op_lda, addr_zp0, 3};
	lookup[OP_LDA_ABS] = (struct Instruction){"LDA", op_lda, addr_abs, 4};

	lookup[OP_LDX_IMM] = (struct Instruction){"LDX", op_ldx, addr_imm, 2};
	lookup[OP_LDY_IMM] = (struct Instruction){"LDY", op_ldy, addr_imm, 2};

	lookup[OP_PHP] = (struct Instruction){"PHP", op_php, addr_imp, 3};
	lookup[OP_PLP] = (struct Instruction){"PLP", op_plp, addr_imp, 4};
	lookup[OP_PHA] = (struct Instruction){"PHA", op_pha, addr_imp, 3};
	lookup[OP_PLA] = (struct Instruction){"PLA", op_pla, addr_imp, 4};

	lookup[OP_STA_ABS] = (struct Instruction){"STA", op_sta, addr_abs, 4};
	lookup[OP_STA_ZP0] = (struct Instruction){"STA", op_sta, addr_zp0, 3};

	lookup[OP_STX_ABS] = (struct Instruction){"STX", op_stx, addr_abs, 4};
	lookup[OP_STX_ZP0] = (struct Instruction){"STX", op_stx, addr_zp0, 3};

	lookup[OP_STY_ABS] = (struct Instruction){"STY", op_sty, addr_abs, 4};
	lookup[OP_STY_ZP0] = (struct Instruction){"STY", op_sty, addr_zp0, 3};

	lookup[OP_JMP_ABS] = (struct Instruction){"JMP", op_jmp, addr_abs, 3};
	lookup[OP_JMP_IND] = (struct Instruction){"JMP", op_jmp, addr_ind, 5};

	lookup[OP_BPL] = (struct Instruction){"BPL", op_bpl, addr_rel, 2};
	lookup[OP_BMI] = (struct Instruction){"BMI", op_bmi, addr_rel, 2};
	lookup[OP_BVC] = (struct Instruction){"BVC", op_bvc, addr_rel, 2};
	lookup[OP_BVS] = (struct Instruction){"BVS", op_bvs, addr_rel, 2};
	lookup[OP_BCC] = (struct Instruction){"BCC", op_bcc, addr_rel, 2};
	lookup[OP_BCS] = (struct Instruction){"BCS", op_bcs, addr_rel, 2};
	lookup[OP_BNE] = (struct Instruction){"BNE", op_bne, addr_rel, 2};
	lookup[OP_BEQ] = (struct Instruction){"BEQ", op_beq, addr_rel, 2};

	lookup[OP_ADC_IMM] = (struct Instruction){"ADC", op_adc, addr_imm, 2};
	lookup[OP_ADC_ZP0] = (struct Instruction){"ADC", op_adc, addr_zp0, 3};
	lookup[OP_ADC_ABS] = (struct Instruction){"ADC", op_adc, addr_abs, 4};

	lookup[OP_SBC_IMM] = (struct Instruction){"SBC", op_sbc, addr_imm, 2};
	lookup[OP_SBC_ZP0] = (struct Instruction){"SBC", op_sbc, addr_zp0, 3};
	lookup[OP_SBC_ABS] = (struct Instruction){"SBC", op_sbc, addr_abs, 4};

	lookup[OP_CLC] = (struct Instruction){"CLC", op_clc, addr_imp, 2};
	lookup[OP_SEC] = (struct Instruction){"SEC", op_sec, addr_imp, 2};
	lookup[OP_CLI] = (struct Instruction){"CLI", op_cli, addr_imp, 2};
	lookup[OP_SEI] = (struct Instruction){"SEI", op_sei, addr_imp, 2};
	lookup[OP_CLV] = (struct Instruction){"CLV", op_clv, addr_imp, 2};
	lookup[OP_CLD] = (struct Instruction){"CLD", op_cld, addr_imp, 2};
	lookup[OP_SED] = (struct Instruction){"SED", op_sed, addr_imp, 2};
}

// initialize cpu state
void cpu_reset(MOS6502 *cpu) {
	init_lookup_table();

	cpu->a = 0;
	cpu->x = 0;
	cpu->y = 0;
	cpu->sp = 0xFD;
	cpu->status = 0x00 | FLAG_U;

	cpu->addr_abs = 0xFFFC;
	word lo = cpu_read(cpu, cpu->addr_abs + 0);
	word hi = cpu_read(cpu, cpu->addr_abs + 1);
	cpu->pc = (hi << 8) | lo;

	cpu->addr_rel = 0;
	cpu->addr_abs = 0;
	cpu->fetched_data = 0;
	cpu->cycles = 0;
}

// read a byte from memory
byte cpu_read(MOS6502 *cpu, word addr) {
	// TODO: add memory mapping logic.
	// if (addr >= 0x2000 && addr < 0x4000) return read_ppu_register(); etc etc...
	return cpu->memory[addr];
}

// read a word from memory
word cpu_read_word(MOS6502 *cpu, word addr) {
	byte low = cpu_read(cpu, addr);
	byte high = cpu_read(cpu, addr + 1);

	return (high << 8) | low;
}

// write a byte to memory
void cpu_write(MOS6502 *cpu, word addr, byte data) {
	// TODO: add special addrs too:
	// ex: if (addr == 0x4016) handle_controller_input(data);
	cpu->memory[addr] = data;
}

// set zero and negative flags based on a result;
void cpu_set_zn_flags(MOS6502 *cpu, byte result) {
	if (result == 0)
		cpu->status |= FLAG_Z;
	else
		cpu->status &= ~FLAG_Z;

	if (result & 0x80)
		cpu->status |= FLAG_N;
	else
		cpu->status &= ~FLAG_N;
}

// execute given amount of cycles
void cpu_execute(MOS6502 *cpu, int cycles) {
	int remaining_cycles = cycles;
	while (remaining_cycles > 0) {
		cpu->opcode = cpu_read(cpu, cpu->pc);
		cpu->pc++;
		cpu->cycles = lookup[cpu->opcode].cycles;

		byte additional_cycle1 = lookup[cpu->opcode].mode(cpu);
		byte additional_cycle2 = lookup[cpu->opcode].operate(cpu);

		cpu->cycles += (additional_cycle1 & additional_cycle2);

		remaining_cycles -= cpu->cycles;

		printf("%s $%04X\n", lookup[cpu->opcode].name, cpu->fetched_data);
	}
}
