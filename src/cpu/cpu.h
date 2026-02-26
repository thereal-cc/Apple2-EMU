#ifndef CPU_H
#define CPU_H

#include "utils/util.h"
#include "disk/disk.h"

#define CYCLES_PER_FRAME 17030 // 1.023 MHz / 60 FPS

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
    
    // BRK/RESET/NMI Locations
    u16 BRK_LOC;
    u16 RESET_LOC;
    u16 NMI_LOC;

    // Rendering Related Variables
    bool text_mode;
    bool low_res;
    bool mixed_mode;
    bool high_res;

    // Disks (Emulate 2 Drives)
    disk_t drive1;
    disk_t drive2;

    // State-Related Variables
    u8 key_value;
    bool key_ready;
    bool running;
    u64 global_cycles;
} cpu_t;

void cpu_init(cpu_t *cpu);
void cpu_cycle(cpu_t *cpu);
bool load_program(cpu_t *cpu, const char* rom_path, u16 address);
bool init_software(cpu_t *cpu_);
u8 read_memory(cpu_t *cpu, u16 address);
void write_memory(cpu_t *cpu, u16 address, u8 value);

// Displaying Register & Memory
void cpu_display_registers(cpu_t *cpu);
void print_memory(cpu_t *cpu, u16 start, u16 end);

#endif