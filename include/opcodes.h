#ifndef __OPCODES_H
#define __OPCODES_H

#define OP_BRK 0x00
#define OP_NOP 0xEA

#define OP_LDA_IMM 0xA9
#define OP_LDA_ZP0 0xA5
#define OP_LDA_ZP0_X 0xB5
#define OP_LDA_ABS 0xAD
#define OP_LDA_ABS_X 0xBD
#define OP_LDA_ABS_Y 0xB9
#define OP_LDA_IND_X 0xA1
#define OP_LDA_IND_Y 0xB1

#define OP_LDX_IMM 0xA2
#define OP_LDX_ZP0 0xA6
#define OP_LDX_ZP0_Y 0xB6
#define OP_LDX_ABS 0xAE
#define OP_LDX_ABS_Y 0xBE

#define OP_LDY_IMM 0xA0
#define OP_LDY_ZP0 0xA4
#define OP_LDY_ZP0_X 0xB4
#define OP_LDY_ABS 0xAC
#define OP_LDY_ABS_X 0xBC

#define OP_PHP 0x08
#define OP_PLP 0x28
#define OP_PHA 0x48
#define OP_PLA 0x68

#define OP_STA_ZP0 0x85
#define OP_STA_ZP0_X 0x95
#define OP_STA_ABS 0x8D
#define OP_STA_ABS_X 0x9D
#define OP_STA_ABS_Y 0x99
#define OP_STA_IND_X 0x81
#define OP_STA_IND_Y 0x91

#define OP_STX_ZP0 0x86
#define OP_STX_ZP0_Y 0x96
#define OP_STX_ABS 0x8E

#define OP_STY_ZP0 0x84
#define OP_STY_ZP0_X 0x94
#define OP_STY_ABS 0x8C

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
#define OP_ADC_ZP0_X 0x75
#define OP_ADC_ABS 0x6D
#define OP_ADC_ABS_X 0x7D
#define OP_ADC_ABS_Y 0x79
#define OP_ADC_IND_X 0x61
#define OP_ADC_IND_Y 0x71

#define OP_SBC_IMM 0xE9
#define OP_SBC_ZP0 0xE5
#define OP_SBC_ZP0_X 0xF5
#define OP_SBC_ABS 0xED
#define OP_SBC_ABS_X 0xFD
#define OP_SBC_ABS_Y 0xF9
#define OP_SBC_IND_X 0xE1
#define OP_SBC_IND_Y 0xF1

#define OP_CLC 0x18
#define OP_SEC 0x38
#define OP_CLI 0x58
#define OP_SEI 0x78
#define OP_CLV 0xB8
#define OP_CLD 0xD8
#define OP_SED 0xF8

#define OP_AND_IMM 0x29
#define OP_AND_ZP0 0x25
#define OP_AND_ZP0_X 0x35
#define OP_AND_ABS 0x2D
#define OP_AND_ABS_X 0x3D
#define OP_AND_ABS_Y 0x39
#define OP_AND_IND_X 0x21
#define OP_AND_IND_Y 0x31

#define OP_ORA_IMM 0x09
#define OP_ORA_ZP0 0x05
#define OP_ORA_ZP0_X 0x15
#define OP_ORA_ABS 0x0D
#define OP_ORA_ABS_X 0x1D
#define OP_ORA_ABS_Y 0x19
#define OP_ORA_IND_X 0x01
#define OP_ORA_IND_Y 0x11

#define OP_EOR_IMM 0x49
#define OP_EOR_ZP0 0x45
#define OP_EOR_ZP0_X 0x55
#define OP_EOR_ABS 0x4D
#define OP_EOR_ABS_X 0x5D
#define OP_EOR_ABS_Y 0x59
#define OP_EOR_IND_X 0x41
#define OP_EOR_IND_Y 0x51

#define OP_ASL_ACC 0x0A
#define OP_ASL_ZP0 0x06
#define OP_ASL_ZP0_X 0x16
#define OP_ASL_ABS 0x0E
#define OP_ASL_ABS_X 0x1E

#define OP_LSR_ACC 0x4A
#define OP_LSR_ZP0 0x46
#define OP_LSR_ZP0_X 0x56
#define OP_LSR_ABS 0x4E
#define OP_LSR_ABS_X 0x5E

#define OP_ROL_ACC 0x2A
#define OP_ROL_ZP0 0x26
#define OP_ROL_ZP0_X 0x36
#define OP_ROL_ABS 0x2E
#define OP_ROL_ABS_X 0x3E

#define OP_ROR_ACC 0x6A
#define OP_ROR_ZP0 0x66
#define OP_ROR_ZP0_X 0x76
#define OP_ROR_ABS 0x6E
#define OP_ROR_ABS_X 0x7E

#define OP_CMP_IMM 0xC9
#define OP_CMP_ZP0 0xC5
#define OP_CMP_ZP0_X 0xD5
#define OP_CMP_ABS 0xCD
#define OP_CMP_ABS_X 0xDD
#define OP_CMP_ABS_Y 0xD9
#define OP_CMP_IND_X 0xC1
#define OP_CMP_IND_Y 0xD1

#define OP_CPX_IMM 0xE0
#define OP_CPX_ZP0 0xE4
#define OP_CPX_ABS 0xEC

#define OP_CPY_IMM 0xC0
#define OP_CPY_ZP0 0xC4
#define OP_CPY_ABS 0xCC

#define OP_INC_ZP0 0xE6
#define OP_INC_ZP0_X 0xF6
#define OP_INC_ABS 0xEE
#define OP_INC_ABS_X 0xFE

#define OP_DEC_ZP0 0xC6
#define OP_DEC_ZP0_X 0xD6
#define OP_DEC_ABS 0xCE
#define OP_DEC_ABS_X 0xDE

#define OP_INX 0xE8
#define OP_DEX 0xCA
#define OP_INY 0xC8
#define OP_DEY 0x88

#endif	// !__OPCODES_H
