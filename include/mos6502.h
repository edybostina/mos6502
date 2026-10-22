#ifndef __MOS6502_H
#define __MOS6502_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bus.h"

typedef uint8_t byte;
typedef uint16_t word;

#define MEM_SIZE 0x10000
#define STACK_BASE_ADDR 0x0100

typedef enum {
	FLAG_C = (1 << 0),	// carry
	FLAG_Z = (1 << 1),	// zero
	FLAG_I = (1 << 2),	// interrupt disable
	FLAG_D = (1 << 3),	// decimal mode
	FLAG_B = (1 << 4),	// break
	FLAG_U = (1 << 5),	// unused (always 1)
	FLAG_V = (1 << 6),	// overflow
	FLAG_N = (1 << 7),	// negative
} StatusFlag;

// macros
#define CPU_SET_FLAG(cpu, flag) ((cpu)->status |= (flag))
#define CPU_CLR_FLAG(cpu, flag) ((cpu)->status &= ~(flag))
#define CPU_GET_FLAG(cpu, flag) ((cpu)->status & (flag))
#define CPU_SET_FLAG_IF(cpu, flag, cond) \
	do {                                 \
		if (cond)                        \
			CPU_SET_FLAG(cpu, flag);     \
		else                             \
			CPU_CLR_FLAG(cpu, flag);     \
	} while (0)

typedef struct MOS6502 {
	word pc;  // program counter
	byte a, x, y;  // accumulator and index registers
	byte sp;  // stack pointer
	byte status;  // processor status register

	byte opcode;  // current instruction
	byte cycles;  // cycles remaining
	byte fetched_data;	// data fetched from memory

	word addr_abs;	// absolute address
	word addr_rel;	// relative address for branches

	Bus *bus;
} MOS6502;

void cpu_connect_bus(MOS6502 *cpu, Bus *bus);
void cpu_reset(MOS6502 *cpu);
void cpu_execute(MOS6502 *cpu, int cycles);

static inline byte cpu_read(MOS6502 *cpu, word addr) {
	return bus_read(cpu->bus, addr, 0);
}

static inline word cpu_read_word(MOS6502 *cpu, word addr) {
	byte lo = cpu_read(cpu, addr);
	byte hi = cpu_read(cpu, addr + 1);
	return (hi << 8) | lo;
}

static inline void cpu_write(MOS6502 *cpu, word addr, byte data) {
	bus_write(cpu->bus, addr, data);
}

static inline void cpu_set_zn_flags(MOS6502 *cpu, byte result) {
	CPU_SET_FLAG_IF(cpu, FLAG_Z, result == 0);
	CPU_SET_FLAG_IF(cpu, FLAG_N, result & 0x80);
}

static inline void cpu_push(MOS6502 *cpu, byte data) {
	cpu_write(cpu, STACK_BASE_ADDR + cpu->sp--, data);
}

static inline byte cpu_pop(MOS6502 *cpu) {
	return cpu_read(cpu, STACK_BASE_ADDR + ++cpu->sp);
}

static inline void cpu_push_word(MOS6502 *cpu, word data) {
	cpu_push(cpu, (data >> 8) & 0xFF);
	cpu_push(cpu, data & 0xFF);
}

static inline word cpu_pop_word(MOS6502 *cpu) {
	byte lo = cpu_pop(cpu);
	byte hi = cpu_pop(cpu);
	return (hi << 8) | lo;
}

#endif	// __MOS6502_H
