#ifndef CPU_H
#define CPU_H

#include "utils/util.h"

typedef struct
{
    // CPU-Related Variables
    u8 A;   
    u16 PC; 
    u8 SP;  
    u8 X;   
    u8 Y;   
    u8 memory[MEMORY_SIZE];
    u8 N; 
    u8 V; 
    u8 B; 
    u8 D; 
    u8 I; 
    u8 Z; 
    u8 C; 

    // State-Related Variables
    u8 key_value;
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