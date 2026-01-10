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
