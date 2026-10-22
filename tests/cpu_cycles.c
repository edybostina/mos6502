#include "../include/bus.h"
#include "../include/mos6502.h"
#include "../include/opcodes.h"
#include <stdint.h>
#include <stdio.h>

static int failures = 0;
static int total_checks = 0;

#define check(name, got, expected)                                             \
    do {                                                                       \
        total_checks++;                                                        \
        long long _got = (long long)(got);                                     \
        long long _exp = (long long)(expected);                                \
        if (_got != _exp) {                                                    \
            printf("FAIL  %s: got %lld, expected %lld\n", (name), _got, _exp); \
            failures++;                                                        \
        }                                                                      \
    } while (0)

static void setup_case(MOS6502 *cpu, Bus *bus, word addr, const byte *prog, int size)
{
    bus_init(bus, NULL);
    cpu_connect_bus(cpu, bus);
    if (prog && size > 0) {
        bus_load_data(bus, addr, prog, size);
    }
    bus_write(bus, ADDR_RESET_VECTOR, addr & 0xFF);
    bus_write(bus, ADDR_RESET_VECTOR + 1, (addr >> 8) & 0xFF);
    cpu_reset(cpu);
}

static void test_right_after_cpu_reset(void)
{
    MOS6502 cpu;
    Bus bus;
    const byte prog[] = {OP_NOP};
    setup_case(&cpu, &bus, 0x8000, prog, sizeof(prog));
    check("Right after cpu_reset", cpu.total_cycles, 7);
    bus_destroy(&bus);
}

static void test_lda_imm(void)
{
    MOS6502 cpu;
    Bus bus;
    const byte prog[] = {OP_LDA_IMM, 0x01};
    setup_case(&cpu, &bus, 0x8000, prog, sizeof(prog));
    int ret = cpu_step(&cpu);
    check("LDA #$01 returns 2", ret, 2);
    check("LDA #$01 cpu.cycles == 2", cpu.cycles, 2);
    check("LDA #$01 total_cycles == 9", cpu.total_cycles, 9);
    bus_destroy(&bus);
}

static void test_lda_abs_x_no_page_cross(void)
{
    MOS6502 cpu;
    Bus bus;
    const byte prog[] = {OP_LDA_ABS_X, 0x00, 0x10};
    setup_case(&cpu, &bus, 0x8000, prog, sizeof(prog));
    cpu.x = 0;
    int ret = cpu_step(&cpu);
    check("LDA abs,X, no page cross", ret, 4);
    bus_destroy(&bus);
}

static void test_lda_abs_x_page_cross(void)
{
    MOS6502 cpu;
    Bus bus;
    const byte prog[] = {OP_LDA_ABS_X, 0xFF, 0x10};
    setup_case(&cpu, &bus, 0x8000, prog, sizeof(prog));
    cpu.x = 1;
    int ret = cpu_step(&cpu);
    check("LDA abs,X, page cross (X=1, LDA $10FF,X)", ret, 5);
    bus_destroy(&bus);
}

static void test_sta_abs_x_page_cross(void)
{
    MOS6502 cpu;
    Bus bus;
    const byte prog[] = {OP_STA_ABS_X, 0xFF, 0x10};
    setup_case(&cpu, &bus, 0x8000, prog, sizeof(prog));
    cpu.x = 1;
    int ret = cpu_step(&cpu);
    check("STA abs,X, page cross", ret, 5);
    bus_destroy(&bus);
}

static void test_branch_not_taken(void)
{
    MOS6502 cpu;
    Bus bus;
    const byte prog[] = {OP_BEQ, 0x05};
    setup_case(&cpu, &bus, 0x8000, prog, sizeof(prog));
    int ret = cpu_step(&cpu);
    check("Branch not taken", ret, 2);
    bus_destroy(&bus);
}

static void test_branch_taken_same_page(void)
{
    MOS6502 cpu;
    Bus bus;
    const byte prog[] = {OP_BNE, 0x05};
    setup_case(&cpu, &bus, 0x8000, prog, sizeof(prog));
    int ret = cpu_step(&cpu);
    check("Branch taken, same page", ret, 3);
    bus_destroy(&bus);
}

static void test_branch_taken_other_page(void)
{
    MOS6502 cpu;
    Bus bus;
    const byte prog[] = {OP_BNE, 0x10};
    setup_case(&cpu, &bus, 0x80F0, prog, sizeof(prog));
    int ret = cpu_step(&cpu);
    check("Branch taken, other page", ret, 4);
    bus_destroy(&bus);
}

static void test_jsr_rts_brk(void)
{
    MOS6502 cpu;
    Bus bus;
    const byte prog[] = {
        OP_JSR, 0x04, 0x80, // JSR $8004
        OP_BRK,             // BRK ($8003)
        OP_RTS              // RTS ($8004)
    };
    setup_case(&cpu, &bus, 0x8000, prog, sizeof(prog));
    int ret_jsr = cpu_step(&cpu);
    check("JSR", ret_jsr, 6);
    int ret_rts = cpu_step(&cpu);
    check("RTS", ret_rts, 6);
    int ret_brk = cpu_step(&cpu);
    check("BRK", ret_brk, 7);
    bus_destroy(&bus);
}

static void test_several_instructions_in_a_row(void)
{
    MOS6502 cpu;
    Bus bus;
    const byte prog[] = {OP_LDA_IMM, 0x01, OP_TAX, OP_INX, OP_LDA_ABS_X, 0x00, 0x20, OP_NOP};
    setup_case(&cpu, &bus, 0x8000, prog, sizeof(prog));
    uint64_t sum = 0;
    sum += cpu_step(&cpu);
    sum += cpu_step(&cpu);
    sum += cpu_step(&cpu);
    sum += cpu_step(&cpu);
    sum += cpu_step(&cpu);
    check("Several instructions in a row", cpu.total_cycles, 7 + sum);
    bus_destroy(&bus);
}

int main(void)
{
    test_right_after_cpu_reset();
    test_lda_imm();
    test_lda_abs_x_no_page_cross();
    test_lda_abs_x_page_cross();
    test_sta_abs_x_page_cross();
    test_branch_not_taken();
    test_branch_taken_same_page();
    test_branch_taken_other_page();
    test_jsr_rts_brk();
    test_several_instructions_in_a_row();

    if (failures != 0) {
        printf("FAIL  cpu cycles: %d of %d checks failed\n", failures, total_checks);
        return 1;
    }

    printf("PASS  cpu cycles: all %d checks passed\n", total_checks);
    return 0;
}
