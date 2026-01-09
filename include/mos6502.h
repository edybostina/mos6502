#ifndef __MOS6502_H
#define __MOS6502_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t byte;
typedef uint16_t word;

#define MEM_SIZE 64 * 1024	// 64kb

typedef struct {
	word pc;  // program counter 16 bit
	byte sp;  // stack pointer 8 bit

	byte a;	 // accumulator 8 bit
	byte x;	 // index register x 8 bit
	byte y;	 // index register y 8 bit

	// processor status register
	// zero, negative, carry etc.
	byte status;

	// TODO: move this in the near future to a special class
	byte memory[MEM_SIZE];

} MOS6502;

typedef enum {
	FLAG_C = (1 << 0),	// carry
	FLAG_Z = (1 << 1),	// zero
	FLAG_I = (1 << 2),	// dis. interupt
	FLAG_D = (1 << 3),	// decimal mode
	FLAG_B = (1 << 4),	// break
	FLAG_U = (1 << 5),	// unused (always 1)
	FLAG_V = (1 << 6),	// overflow
	FLAG_N = (1 << 7),	// negative

} StatusFlag;

// initialize cpu state
void cpu_reset(MOS6502 *cpu);

// read a byte from memory
byte cpu_read(MOS6502 *cpu, word addr);

// read a word from memory
word cpu_read_word(MOS6502 *cpu, word addr);

// write a byte to memory
void cpu_write(MOS6502 *cpu, word addr, byte data);

// set zero and negative flags based on a result;
void cpu_set_zn_flags(MOS6502 *cpu, byte result);

// zero page
// read 1 byte from the first 256 bytes.
byte addr_zero_page(MOS6502 *cpu);

// absolute
// read 2 bytes from anywhere.
word addr_absolute(MOS6502 *cpu);

// execute given amount of cycles
void cpu_execute(MOS6502 *cpu, int cycles);
#endif	// __MOS6502_H
