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

#define OP_JMP_ABS 0x4C
#define OP_JMP_IND 0x6C

#define OP_BPL 0x10
#define OP_BMI 0x30
#define OP_BVC 0x50
#define OP_BVS 0x70
#define OP_BCC 0x90
#define OP_BCS 0xB0
#define OP_BNE 0xD0
#define OP_BEQ 0xF0

#define OP_ADC_IMM 0x69	 // nice
#define OP_ADC_ZP0 0x65
#define OP_ADC_ABS 0x6D

#define OP_SBC_IMM 0xE9
#define OP_SBC_ZP0 0xE5
#define OP_SBC_ABS 0xED

#define OP_CLC 0x18
#define OP_SEC 0x38
#define OP_CLI 0x58
#define OP_SEI 0x78
#define OP_CLV 0xB8
#define OP_CLD 0xD8
#define OP_SED 0xF8
#endif	// !__OPCODES_H
