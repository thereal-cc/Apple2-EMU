#ifndef CPU_H
#define CPU_H

#include "utils/util.h"

typedef struct
{
    u8 A;   // 8-bit Accumlator
    u16 PC; // 16 bit Program Counter
    u8 SP;  // Stack Pointer
    u8 X;   // 'X' Index Register
    u8 Y;   // 'Y' Index Register
    u8 memory[MEMORY_SIZE];
    u8 key_value;
    u8 N; // Negative Flag
    u8 V; // Overflow Flag
    u8 B; // B Flag
    u8 D; // Decimal Flag
    u8 I; // Interrupt Disable
    u8 Z; // Zero Flag
    u8 C; // Carry Flag
    bool running;
    bool key_ready;
    u64 global_cycles;
} cpu_t;

void cpu_init(cpu_t *cpu);
void cpu_cycle(cpu_t *cpu);
u8 load_program(cpu_t *cpu, const char* rom_path, u16 address);
u8 read_memory(cpu_t *cpu, u16 address);
void write_memory(cpu_t *cpu, u16 address, u8 value);

// Displaying Register & Memory
void cpu_display_registers(cpu_t *cpu);
void print_memory(cpu_t *cpu, u16 start, u16 end);

#endif