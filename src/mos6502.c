#include "../include/mos6502.h"
#include "../include/instructions.h"
#include "../include/opcodes.h"

struct Instruction lookup[256];

void init_lookup_table()
{
    for (int i = 0; i < 256; i++) {
        lookup[i] = (struct Instruction){"XXX", op_xxx, addr_imp, 2};
    }

    lookup[OP_NOP] = (struct Instruction){"NOP", op_nop, addr_imp, 2};

    // access
    lookup[OP_LDA_IMM] = (struct Instruction){"LDA", op_lda, addr_imm, 2};
    lookup[OP_LDA_ZP0] = (struct Instruction){"LDA", op_lda, addr_zp0, 3};
    lookup[OP_LDA_ZP0_X] = (struct Instruction){"LDA", op_lda, addr_zp0x, 4};
    lookup[OP_LDA_ABS] = (struct Instruction){"LDA", op_lda, addr_abs, 4};
    lookup[OP_LDA_ABS_X] = (struct Instruction){"LDA", op_lda, addr_absx, 4};
    lookup[OP_LDA_ABS_Y] = (struct Instruction){"LDA", op_lda, addr_absy, 4};
    lookup[OP_LDA_IND_X] = (struct Instruction){"LDA", op_lda, addr_indx, 6};
    lookup[OP_LDA_IND_Y] = (struct Instruction){"LDA", op_lda, addr_indy, 5};

    lookup[OP_STA_ZP0] = (struct Instruction){"STA", op_sta, addr_zp0, 3};
    lookup[OP_STA_ZP0_X] = (struct Instruction){"STA", op_sta, addr_zp0x, 4};
    lookup[OP_STA_ABS] = (struct Instruction){"STA", op_sta, addr_abs, 4};
    lookup[OP_STA_ABS_X] = (struct Instruction){"STA", op_sta, addr_absx, 5};
    lookup[OP_STA_ABS_Y] = (struct Instruction){"STA", op_sta, addr_absy, 5};
    lookup[OP_STA_IND_X] = (struct Instruction){"STA", op_sta, addr_indx, 6};
    lookup[OP_STA_IND_Y] = (struct Instruction){"STA", op_sta, addr_indy, 6};

    lookup[OP_LDX_IMM] = (struct Instruction){"LDX", op_ldx, addr_imm, 2};
    lookup[OP_LDX_ZP0] = (struct Instruction){"LDX", op_ldx, addr_zp0, 3};
    lookup[OP_LDX_ZP0_Y] = (struct Instruction){"LDX", op_ldx, addr_zp0y, 4};
    lookup[OP_LDX_ABS] = (struct Instruction){"LDX", op_ldx, addr_abs, 4};
    lookup[OP_LDX_ABS_Y] = (struct Instruction){"LDX", op_ldx, addr_absy, 4};

    lookup[OP_STX_ZP0] = (struct Instruction){"STX", op_stx, addr_zp0, 3};
    lookup[OP_STX_ZP0_Y] = (struct Instruction){"STX", op_stx, addr_zp0y, 4};
    lookup[OP_STX_ABS] = (struct Instruction){"STX", op_stx, addr_abs, 4};

    lookup[OP_LDY_IMM] = (struct Instruction){"LDY", op_ldy, addr_imm, 2};
    lookup[OP_LDY_ZP0] = (struct Instruction){"LDY", op_ldy, addr_zp0, 3};
    lookup[OP_LDY_ZP0_X] = (struct Instruction){"LDY", op_ldy, addr_zp0x, 4};
    lookup[OP_LDY_ABS] = (struct Instruction){"LDY", op_ldy, addr_abs, 4};
    lookup[OP_LDY_ABS_X] = (struct Instruction){"LDY", op_ldy, addr_absx, 4};

    lookup[OP_STY_ZP0] = (struct Instruction){"STY", op_sty, addr_zp0, 3};
    lookup[OP_STY_ZP0_X] = (struct Instruction){"STY", op_sty, addr_zp0x, 4};
    lookup[OP_STY_ABS] = (struct Instruction){"STY", op_sty, addr_abs, 4};

    // transfer
    lookup[OP_TAX] = (struct Instruction){"TAX", op_tax, addr_imp, 2};
    lookup[OP_TXA] = (struct Instruction){"TXA", op_txa, addr_imp, 2};

    lookup[OP_TAY] = (struct Instruction){"TAY", op_tay, addr_imp, 2};
    lookup[OP_TYA] = (struct Instruction){"TYA", op_tya, addr_imp, 2};

    // arithmetic
    lookup[OP_ADC_IMM] = (struct Instruction){"ADC", op_adc, addr_imm, 2};
    lookup[OP_ADC_ZP0] = (struct Instruction){"ADC", op_adc, addr_zp0, 3};
    lookup[OP_ADC_ZP0_X] = (struct Instruction){"ADC", op_adc, addr_zp0x, 4};
    lookup[OP_ADC_ABS] = (struct Instruction){"ADC", op_adc, addr_abs, 4};
    lookup[OP_ADC_ABS_X] = (struct Instruction){"ADC", op_adc, addr_absx, 4};
    lookup[OP_ADC_ABS_Y] = (struct Instruction){"ADC", op_adc, addr_absy, 4};
    lookup[OP_ADC_IND_X] = (struct Instruction){"ADC", op_adc, addr_indx, 6};
    lookup[OP_ADC_IND_Y] = (struct Instruction){"ADC", op_adc, addr_indy, 5};

    lookup[OP_SBC_IMM] = (struct Instruction){"SBC", op_sbc, addr_imm, 2};
    lookup[OP_SBC_ZP0] = (struct Instruction){"SBC", op_sbc, addr_zp0, 3};
    lookup[OP_SBC_ZP0_X] = (struct Instruction){"SBC", op_sbc, addr_zp0x, 4};
    lookup[OP_SBC_ABS] = (struct Instruction){"SBC", op_sbc, addr_abs, 4};
    lookup[OP_SBC_ABS_X] = (struct Instruction){"SBC", op_sbc, addr_absx, 4};
    lookup[OP_SBC_ABS_Y] = (struct Instruction){"SBC", op_sbc, addr_absy, 4};
    lookup[OP_SBC_IND_X] = (struct Instruction){"SBC", op_sbc, addr_indx, 6};
    lookup[OP_SBC_IND_Y] = (struct Instruction){"SBC", op_sbc, addr_indy, 5};

    lookup[OP_INC_ZP0] = (struct Instruction){"INC", op_inc, addr_zp0, 5};
    lookup[OP_INC_ZP0_X] = (struct Instruction){"INC", op_inc, addr_zp0x, 6};
    lookup[OP_INC_ABS] = (struct Instruction){"INC", op_inc, addr_abs, 6};
    lookup[OP_INC_ABS_X] = (struct Instruction){"INC", op_inc, addr_absx, 7};

    lookup[OP_DEC_ZP0] = (struct Instruction){"DEC", op_dec, addr_zp0, 5};
    lookup[OP_DEC_ZP0_X] = (struct Instruction){"DEC", op_dec, addr_zp0x, 6};
    lookup[OP_DEC_ABS] = (struct Instruction){"DEC", op_dec, addr_abs, 6};
    lookup[OP_DEC_ABS_X] = (struct Instruction){"DEC", op_dec, addr_absx, 7};

    lookup[OP_INX] = (struct Instruction){"INX", op_inx, addr_imp, 2};
    lookup[OP_DEX] = (struct Instruction){"DEX", op_dex, addr_imp, 2};
    lookup[OP_INY] = (struct Instruction){"INY", op_iny, addr_imp, 2};
    lookup[OP_DEY] = (struct Instruction){"DEY", op_dey, addr_imp, 2};

    // shift
    lookup[OP_ASL_ACC] = (struct Instruction){"ASL", op_asl, addr_imp, 2};
    lookup[OP_ASL_ZP0] = (struct Instruction){"ASL", op_asl, addr_zp0, 5};
    lookup[OP_ASL_ZP0_X] = (struct Instruction){"ASL", op_asl, addr_zp0x, 6};
    lookup[OP_ASL_ABS] = (struct Instruction){"ASL", op_asl, addr_abs, 6};
    lookup[OP_ASL_ABS_X] = (struct Instruction){"ASL", op_asl, addr_absx, 7};

    lookup[OP_LSR_ACC] = (struct Instruction){"LSR", op_lsr, addr_imp, 2};
    lookup[OP_LSR_ZP0] = (struct Instruction){"LSR", op_lsr, addr_zp0, 5};
    lookup[OP_LSR_ZP0_X] = (struct Instruction){"LSR", op_lsr, addr_zp0x, 6};
    lookup[OP_LSR_ABS] = (struct Instruction){"LSR", op_lsr, addr_abs, 6};
    lookup[OP_LSR_ABS_X] = (struct Instruction){"LSR", op_lsr, addr_absx, 7};

    lookup[OP_ROL_ACC] = (struct Instruction){"ROL", op_rol, addr_imp, 2};
    lookup[OP_ROL_ZP0] = (struct Instruction){"ROL", op_rol, addr_zp0, 5};
    lookup[OP_ROL_ZP0_X] = (struct Instruction){"ROL", op_rol, addr_zp0x, 6};
    lookup[OP_ROL_ABS] = (struct Instruction){"ROL", op_rol, addr_abs, 6};
    lookup[OP_ROL_ABS_X] = (struct Instruction){"ROL", op_rol, addr_absx, 7};

    lookup[OP_ROR_ACC] = (struct Instruction){"ROR", op_ror, addr_imp, 2};
    lookup[OP_ROR_ZP0] = (struct Instruction){"ROR", op_ror, addr_zp0, 5};
    lookup[OP_ROR_ZP0_X] = (struct Instruction){"ROR", op_ror, addr_zp0x, 6};
    lookup[OP_ROR_ABS] = (struct Instruction){"ROR", op_ror, addr_abs, 6};
    lookup[OP_ROR_ABS_X] = (struct Instruction){"ROR", op_ror, addr_absx, 7};

    // bitwise
    lookup[OP_AND_IMM] = (struct Instruction){"AND", op_and, addr_imm, 2};
    lookup[OP_AND_ZP0] = (struct Instruction){"AND", op_and, addr_zp0, 3};
    lookup[OP_AND_ZP0_X] = (struct Instruction){"AND", op_and, addr_zp0x, 4};
    lookup[OP_AND_ABS] = (struct Instruction){"AND", op_and, addr_abs, 4};
    lookup[OP_AND_ABS_X] = (struct Instruction){"AND", op_and, addr_absx, 4};
    lookup[OP_AND_ABS_Y] = (struct Instruction){"AND", op_and, addr_absy, 4};
    lookup[OP_AND_IND_X] = (struct Instruction){"AND", op_and, addr_indx, 6};
    lookup[OP_AND_IND_Y] = (struct Instruction){"AND", op_and, addr_indy, 5};

    lookup[OP_ORA_IMM] = (struct Instruction){"ORA", op_ora, addr_imm, 2};
    lookup[OP_ORA_ZP0] = (struct Instruction){"ORA", op_ora, addr_zp0, 3};
    lookup[OP_ORA_ZP0_X] = (struct Instruction){"ORA", op_ora, addr_zp0x, 4};
    lookup[OP_ORA_ABS] = (struct Instruction){"ORA", op_ora, addr_abs, 4};
    lookup[OP_ORA_ABS_X] = (struct Instruction){"ORA", op_ora, addr_absx, 4};
    lookup[OP_ORA_ABS_Y] = (struct Instruction){"ORA", op_ora, addr_absy, 4};
    lookup[OP_ORA_IND_X] = (struct Instruction){"ORA", op_ora, addr_indx, 6};
    lookup[OP_ORA_IND_Y] = (struct Instruction){"ORA", op_ora, addr_indy, 5};

    lookup[OP_EOR_IMM] = (struct Instruction){"EOR", op_eor, addr_imm, 2};
    lookup[OP_EOR_ZP0] = (struct Instruction){"EOR", op_eor, addr_zp0, 3};
    lookup[OP_EOR_ZP0_X] = (struct Instruction){"EOR", op_eor, addr_zp0x, 4};
    lookup[OP_EOR_ABS] = (struct Instruction){"EOR", op_eor, addr_abs, 4};
    lookup[OP_EOR_ABS_X] = (struct Instruction){"EOR", op_eor, addr_absx, 4};
    lookup[OP_EOR_ABS_Y] = (struct Instruction){"EOR", op_eor, addr_absy, 4};
    lookup[OP_EOR_IND_X] = (struct Instruction){"EOR", op_eor, addr_indx, 6};
    lookup[OP_EOR_IND_Y] = (struct Instruction){"EOR", op_eor, addr_indy, 5};

    lookup[OP_BIT_ZP0] = (struct Instruction){"BIT", op_bit, addr_zp0, 3};
    lookup[OP_BIT_ABS] = (struct Instruction){"BIT", op_bit, addr_abs, 4};

    // compare
    lookup[OP_CMP_IMM] = (struct Instruction){"CMP", op_cmp, addr_imm, 2};
    lookup[OP_CMP_ZP0] = (struct Instruction){"CMP", op_cmp, addr_zp0, 3};
    lookup[OP_CMP_ZP0_X] = (struct Instruction){"CMP", op_cmp, addr_zp0x, 4};
    lookup[OP_CMP_ABS] = (struct Instruction){"CMP", op_cmp, addr_abs, 4};
    lookup[OP_CMP_ABS_X] = (struct Instruction){"CMP", op_cmp, addr_absx, 4};
    lookup[OP_CMP_ABS_Y] = (struct Instruction){"CMP", op_cmp, addr_absy, 4};
    lookup[OP_CMP_IND_X] = (struct Instruction){"CMP", op_cmp, addr_indx, 6};
    lookup[OP_CMP_IND_Y] = (struct Instruction){"CMP", op_cmp, addr_indy, 5};

    lookup[OP_CPX_IMM] = (struct Instruction){"CPX", op_cpx, addr_imm, 2};
    lookup[OP_CPX_ZP0] = (struct Instruction){"CPX", op_cpx, addr_zp0, 3};
    lookup[OP_CPX_ABS] = (struct Instruction){"CPX", op_cpx, addr_abs, 4};

    lookup[OP_CPY_IMM] = (struct Instruction){"CPY", op_cpy, addr_imm, 2};
    lookup[OP_CPY_ZP0] = (struct Instruction){"CPY", op_cpy, addr_zp0, 3};
    lookup[OP_CPY_ABS] = (struct Instruction){"CPY", op_cpy, addr_abs, 4};

    // branch
    lookup[OP_BPL] = (struct Instruction){"BPL", op_bpl, addr_rel, 2};
    lookup[OP_BMI] = (struct Instruction){"BMI", op_bmi, addr_rel, 2};
    lookup[OP_BVC] = (struct Instruction){"BVC", op_bvc, addr_rel, 2};
    lookup[OP_BVS] = (struct Instruction){"BVS", op_bvs, addr_rel, 2};
    lookup[OP_BCC] = (struct Instruction){"BCC", op_bcc, addr_rel, 2};
    lookup[OP_BCS] = (struct Instruction){"BCS", op_bcs, addr_rel, 2};
    lookup[OP_BNE] = (struct Instruction){"BNE", op_bne, addr_rel, 2};
    lookup[OP_BEQ] = (struct Instruction){"BEQ", op_beq, addr_rel, 2};

    // jump
    lookup[OP_JMP_ABS] = (struct Instruction){"JMP", op_jmp, addr_abs, 3};
    lookup[OP_JMP_IND] = (struct Instruction){"JMP", op_jmp, addr_ind, 5};

    lookup[OP_JSR] = (struct Instruction){"JSR", op_jsr, addr_abs, 6};
    lookup[OP_RTS] = (struct Instruction){"RTS", op_rts, addr_imp, 6};

    lookup[OP_BRK] = (struct Instruction){"BRK", op_brk, addr_imp, 7};
    lookup[OP_RTI] = (struct Instruction){"RTI", op_rti, addr_imp, 6};

    // stack
    lookup[OP_PHP] = (struct Instruction){"PHP", op_php, addr_imp, 3};
    lookup[OP_PLP] = (struct Instruction){"PLP", op_plp, addr_imp, 4};
    lookup[OP_PHA] = (struct Instruction){"PHA", op_pha, addr_imp, 3};
    lookup[OP_PLA] = (struct Instruction){"PLA", op_pla, addr_imp, 4};

    lookup[OP_TSX] = (struct Instruction){"TSX", op_tsx, addr_imp, 2};
    lookup[OP_TXS] = (struct Instruction){"TXS", op_txs, addr_imp, 2};

    // flags
    lookup[OP_CLC] = (struct Instruction){"CLC", op_clc, addr_imp, 2};
    lookup[OP_SEC] = (struct Instruction){"SEC", op_sec, addr_imp, 2};
    lookup[OP_CLI] = (struct Instruction){"CLI", op_cli, addr_imp, 2};
    lookup[OP_SEI] = (struct Instruction){"SEI", op_sei, addr_imp, 2};
    lookup[OP_CLV] = (struct Instruction){"CLV", op_clv, addr_imp, 2};
    lookup[OP_CLD] = (struct Instruction){"CLD", op_cld, addr_imp, 2};
    lookup[OP_SED] = (struct Instruction){"SED", op_sed, addr_imp, 2};
}

void cpu_reset(MOS6502 *cpu)
{
    static int lookup_initialized = 0;

    if (!lookup_initialized) {
        init_lookup_table();
        lookup_initialized = 1;
    }

    cpu->a = 0x00;
    cpu->x = 0x00;
    cpu->y = 0x00;
    cpu->sp = 0xFD;
    cpu->status = FLAG_U | FLAG_I;

    cpu->pc = cpu_read_word(cpu, ADDR_RESET_VECTOR);

    cpu->addr_rel = 0x0000;
    cpu->addr_abs = 0x0000;
    cpu->fetched_data = 0x00;
    cpu->opcode = 0x00;
    cpu->cycles = 0;
    cpu->total_cycles = 7; // reset sequence cost (an interrupt sequence with writes turned off)
}

void cpu_connect_bus(MOS6502 *cpu, Bus *bus)
{
    cpu->bus = bus;
}

void cpu_execute(MOS6502 *cpu, int cycles)
{
    int remaining = cycles;

    while (remaining > 0) {
        int c = cpu_step(cpu);

        bus_tick(cpu->bus, c);

        remaining -= c;
    }
}

int cpu_step(MOS6502 *cpu)
{
    cpu->opcode = cpu_read(cpu, cpu->pc++);

    cpu->cycles = lookup[cpu->opcode].cycles;

    byte addr_cycles = lookup[cpu->opcode].mode(cpu);
    byte op_cycles = lookup[cpu->opcode].operate(cpu);

    cpu->cycles += (addr_cycles & op_cycles);
    cpu->total_cycles += cpu->cycles;

    return cpu->cycles;
}
