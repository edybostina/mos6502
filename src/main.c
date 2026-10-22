#include "../include/mos6502.h"
#include "../include/bus.h"
#include "../include/opcodes.h"
#include "../platforms/platform_basic.h"
#include <stdio.h>

int main(void)
{
    MOS6502 cpu;
    Bus bus;

    Platform *platform = platform_basic_create();

    bus_init(&bus, platform);
    cpu_connect_bus(&cpu, &bus);

    bus.rom_locked = 0;

    static const byte program[] = {
        0xA2, 0x00, // LDX #$00       ; X = 0
                    // LOOP:
        OP_LDA_ABS_X, 0x0F, 0x80, // LDA $800F,X    ; Load char from string
        OP_BEQ, 0x07,             // BEQ END        ; If 0, end
        OP_STA_ABS, 0x00, 0xFF,   // STA $FF00      ; Output char
        OP_INX,                   // INX            ; X++
        OP_JMP_ABS, 0x02, 0x80,   // JMP LOOP       ; Loop
        // END:
        OP_BRK, // BRK            ; Stop
        // String data at $800F
        'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', '\n', 0x00};

    bus_load_data(&bus, 0x8000, program, sizeof(program));

    static const byte reset_vector[] = {0x00, 0x80};
    bus_load_data(&bus, ADDR_RESET_VECTOR, reset_vector, sizeof(reset_vector));

    bus.rom_locked = 1;

    cpu_reset(&cpu);

    printf("--- Booting up... ---\n");

    while (cpu_read(&cpu, cpu.pc) != OP_BRK) {
        cpu_execute(&cpu, 1);
    }
    platform_basic_free(platform);

    printf("--- CPU Halt ---\n");

    bus_destroy(&bus);

    return 0;
}
