#include "../include/mos6502.h"

// initialize cpu state
void cpu_reset(MOS6502 *cpu) {
	cpu->a = 0;
	cpu->x = 0;
	cpu->y = 0;
	cpu->sp = 0xFD;
	cpu->status = 0x00 | FLAG_U;

	uint8_t lo = cpu->memory[0xFFFC];
	uint8_t hi = cpu->memory[0xFFFD];
	cpu->pc = (hi << 8) | lo;
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

// zero page
// read 1 byte from the first 256 bytes.
byte addr_zero_page(MOS6502 *cpu) {
	byte addr = cpu_read(cpu, cpu->pc);
	cpu->pc++;
	return (word)addr;
}

// absolute
// read 2 bytes from anywhere.
word addr_absolute(MOS6502 *cpu) {
	word addr = cpu_read_word(cpu, cpu->pc);
	cpu->pc += 2;
	return addr;
}

// execute given amount of cycles
void cpu_execute(MOS6502 *cpu, int cycles) {
	while (cycles > 0) {
		// fetch
		byte opcode = cpu_read(cpu, cpu->pc);
		cpu->pc++;

		int used_cycles = 0;

		switch (opcode) {
			case 0xA9:	// LDA immediate
			{
				byte value = cpu_read(cpu, cpu->pc);
				cpu->pc++;

				cpu->a = value;

				cpu_set_zn_flags(cpu, cpu->a);
				printf("LDA #$%02X (immediate)\n", value);
				used_cycles = 2;
			} break;
			case 0xA5:	// zero page: value at addr $00xx
			{
				word addr = addr_zero_page(cpu);
				byte value = cpu_read(cpu, addr);

				cpu->a = value;
				cpu_set_zn_flags(cpu, cpu->a);

				printf("LDA #$%02X (zero page)\n", addr);
				used_cycles = 3;
			} break;
			case 0xAD:	// absolute: value at addr $xxxx
			{
				word addr = addr_absolute(cpu);
				byte value = cpu_read(cpu, addr);

				cpu->a = value;
				cpu_set_zn_flags(cpu, cpu->a);

				printf("LDA #$%04X (absolute)\n", addr);
				used_cycles = 3;
			} break;
			case 0xA2:	// LDX immediate
			{
				byte value = cpu_read(cpu, cpu->pc);
				cpu->pc++;

				cpu->x = value;

				cpu_set_zn_flags(cpu, cpu->x);

				printf("LDX #$%02X\n", value);
				used_cycles = 2;
			} break;

			case 0xA0:	// LDY immediate
			{
				byte value = cpu_read(cpu, cpu->pc);
				cpu->pc++;

				cpu->y = value;

				cpu_set_zn_flags(cpu, cpu->y);

				printf("LDY #$%02X\n", value);
				used_cycles = 2;
			} break;

			case 0xEA:	// NOP no operation
				printf("NOP\n");
				used_cycles = 2;
				break;

			case 0x00:	// break
				printf("brk detected! halting!\n");
				return;

			default:
				printf("unknown opcode!! 0x%02X at PC: 0x%04X\n", opcode, cpu->pc - 1);
				return;
		}
		cycles -= used_cycles;
	}
}
