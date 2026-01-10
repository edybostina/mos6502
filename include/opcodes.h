#ifndef __OPCODES_H
#define __OPCODES_H

#define OP_BRK 0x00
#define OP_NOP 0xEA

#define OP_LDA_IMM 0xA9
#define OP_LDA_ZP0 0xA5
#define OP_LDA_ABS 0xAD

#define OP_LDX_IMM 0xA2

#define OP_LDY_IMM 0xA0

#define OP_PHP 0x08
#define OP_PLP 0x28
#define OP_PHA 0x48
#define OP_PLA 0x68

#define OP_STA_ABS 0x8D
#define OP_STA_ZP0 0x85

#define OP_STX_ABS 0x8E
#define OP_STX_ZP0 0x86

#define OP_STY_ABS 0x8C
#define OP_STY_ZP0 0x84

#endif	// !__OPCODES_H
