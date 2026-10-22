#include "../include/instructions.h"
#include "../include/mos6502.h"
#include <stdio.h>

// Klaus Dormann's 6502 functional test (tests/roms/README.txt). Every failing
// check jumps to itself, so the run is over when PC stops moving, and only the
// stock binary's success trap counts as a pass.

#define START_ADDR       0x0400
#define SUCCESS_ADDR     0x3469
#define TEST_CASE_ADDR   0x0200       // the test keeps the number of the running test here
#define MAX_INSTRUCTIONS 100000000ULL // a pass takes about 30.6M
#define HISTORY_LEN      16

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s 6502_functional_test.bin\n", argv[0]);
        return 2;
    }

    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror(argv[1]);
        return 2;
    }

    static byte image[MEM_SIZE];
    size_t size = fread(image, 1, sizeof(image), file);
    fclose(file);

    if (size != MEM_SIZE) {
        fprintf(stderr, "%s: expected a 64KB image, got %zu bytes\n", argv[1], size);
        return 2;
    }

    MOS6502 cpu;
    Bus bus;

    bus_init(&bus, NULL);
    cpu_connect_bus(&cpu, &bus);
    bus_load_data(&bus, 0x0000, image, MEM_SIZE);

    cpu_reset(&cpu);
    cpu.pc = START_ADDR;

    word history[HISTORY_LEN] = {0};
    unsigned long long instructions = 0;
    unsigned long long cycles = 0;
    word pc;

    do {
        pc = cpu.pc;
        history[instructions % HISTORY_LEN] = pc;
        cpu_execute(&cpu, 1);
        cycles += cpu.cycles;
        instructions++;
    } while (cpu.pc != pc && instructions < MAX_INSTRUCTIONS);

    int trapped = cpu.pc == pc;
    int passed = trapped && pc == SUCCESS_ADDR;

    if (passed) {
        printf("PASS  6502 functional test: reached $%04X after %llu instructions, %llu cycles\n",
               pc, instructions, cycles);
    } else {
        if (trapped) {
            printf(
                "FAIL  6502 functional test: trapped at $%04X in test $%02X (success is $%04X)\n",
                pc, cpu_read(&cpu, TEST_CASE_ADDR), SUCCESS_ADDR);
        } else {
            printf("FAIL  6502 functional test: no trap after %llu instructions (PC=$%04X, test "
                   "$%02X)\n",
                   instructions, cpu.pc, cpu_read(&cpu, TEST_CASE_ADDR));
        }

        printf("      %s  A=%02X X=%02X Y=%02X SP=%02X P=%02X\n", lookup[cpu.opcode].name, cpu.a,
               cpu.x, cpu.y, cpu.sp, cpu.status);
        printf("      last PCs:");
        for (int i = 0; i < HISTORY_LEN; i++) {
            printf(" %04X", history[(instructions + i) % HISTORY_LEN]);
        }
        printf("\n");
    }

    bus_destroy(&bus);
    return passed ? 0 : 1;
}
