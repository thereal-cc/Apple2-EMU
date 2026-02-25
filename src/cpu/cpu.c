#include "cpu.h"
#include "instruction.h"

static FILE *log = NULL;

void cpu_init(cpu_t *cpu)
{
    // Clear Memory
    memset(cpu->memory, 0, sizeof(cpu->memory));
    memset(cpu->memory + 0x0400, 0xA0, 0x0400);

    // Registers
    cpu->SP = 0xFF;
    cpu->A = cpu->X = cpu->Y = 0;

    // Status flags
    cpu->N = cpu->V = cpu->D = cpu->C = 0;
    cpu->B = cpu->I = cpu->Z = 1;

    // Keyboard State
    cpu->key_ready = false;
    cpu->key_value = 0;

    // Global State
    cpu->running = true;
    cpu->global_cycles = 0;

    // Rendering State
    cpu->text_mode = true;
    cpu->low_res = false;
    cpu->high_res = false;
    cpu->mixed_mode = false;
}

void cpu_cycle(cpu_t *cpu)
{
    // Debug Function (Prints CPU State to file)
    // cpu_display_registers(cpu);
    u8 opcode_byte = read_memory(cpu, cpu->PC++);
    opcode_t opcode = opcodes[opcode_byte];
    u16 addr = 0;

    switch (opcode.addr_mode) {
        case IMM: addr = imm_address(cpu); break;
        case ZP:  addr = zp_address(cpu);  break;
        case ZPX: addr = zpx_address(cpu); break;
        case ZPY: addr = zpy_address(cpu); break;
        case ABS: addr = abs_address(cpu); break;
        case ABX: addr = abx_address(cpu); break;
        case ABY: addr = aby_address(cpu); break;
        case IND: addr = ind_address(cpu); break;
        case IDX: addr = indx_address(cpu); break;
        case IDY: addr = indy_address(cpu); break;
        case IMP: addr = imp_address(cpu); break;
        case REL: addr = rel_address(cpu); break;
    }

    opcode.operation(cpu, addr);
    //usleep(1 * opcode.cycles);
    cpu->global_cycles += opcode.cycles;
}

bool load_program(cpu_t *cpu, const char* rom_path, u16 address)
{
    // Load File
    FILE *fptr = fopen(rom_path, "rb");
    if (fptr == NULL) return false;

    if (fseek(fptr, 0, SEEK_END) != 0) { 
        fprintf(stderr, "Error seeking to end of file\n");
        fclose(fptr);
        return false;
    }

    long size = ftell(fptr);
    if (size == -1) {
        fprintf(stderr, "Error getting file position\n");
        fclose(fptr);
        return false;
    }
    fseek(fptr, 0, SEEK_SET);

    // Load File into CPU Memory
    size_t bytes_read = fread(cpu->memory + address, sizeof(u8), size, fptr);
    fclose(fptr);

    // Nothing Loaded
    if (!bytes_read) {
        fprintf(stderr, "Nothing was loaded\n");
        return false;
    }

    return true;
}

bool init_software(cpu_t *cpu)
{
    // Load Apple2_Plus rom
    if (!load_program(cpu, "./roms/Apple2_Plus.Rom", 0xD000))
    {
        fprintf(stderr, "Error: Could not load ROM\n");
        return false;
    }

    // Set NMI, Reset, & BRK Locations
    cpu->NMI_LOC = (cpu->memory[NMI_HIGH_ADDR] << 8) | cpu->memory[NMI_LOW_ADDR];
    cpu->RESET_LOC = (cpu->memory[RESET_HIGH_ADDR] << 8) | cpu->memory[RESET_LOW_ADDR];
    cpu->BRK_LOC = (cpu->memory[BRK_HIGH_ADDR] << 8) | cpu->memory[BRK_LOW_ADDR];

    cpu->PC = cpu->RESET_LOC;
    return true;
}

u8 read_memory(cpu_t *cpu, u16 address)
{
    // Read IO Space
    if (address >= 0xC000 && address <= 0xCFFF) {
        switch (address) {
            // Return Key Value
            case 0xC000:
                if (cpu->key_ready) {
                    return cpu->key_value | NEGATIVE_FLAG;
                }
                return 0;
            // Clear Key Press
            case 0xC010:
                cpu->key_ready = false;
                return 0;
            // Clear Text Mode
            case 0xC050:
                cpu->text_mode = false; 
                return 0;
            // Set Text Mode
            case 0xC051:
                cpu->text_mode = true; 
                return 0;
            // Set Full Screen
            case 0xC052:
                cpu->mixed_mode = false; 
                return 0; 
            // Set Mixed Mode
            case 0xC053:
                cpu->mixed_mode = true;  
                return 0;
            // Set Page 1
            case 0xC054:
                return 0;
            // Set Page 2
            case 0xC055:
                return 0;
            // Set Low-Res
            case 0xC056:
                cpu->low_res = true;  
                cpu->high_res = false; 
                return 0; 
            // Set High-Res
            case 0xC057:
                cpu->low_res = false; 
                cpu->high_res = true;  
                return 0;
        }

        return 0;
    }

    return cpu->memory[address];
}

void write_memory(cpu_t *cpu, u16 address, u8 value)
{
    // Read IO Space
    if (address >= 0xC000 && address <= 0xCFFF) {
        switch (address) {
            // Clear Key Press
            case 0xC010:
                cpu->key_ready = false;
                return;
            // Clear Text Mode
            case 0xC050:
                cpu->text_mode = false; 
                return;
            // Set Text Mode
            case 0xC051:
                cpu->text_mode = true; 
                return;
            // Set Full Screen
            case 0xC052:
                cpu->mixed_mode = false; 
                return; 
            // Set Mixed Mode
            case 0xC053:
                cpu->mixed_mode = true;  
                return;
            // Set Page 1
            case 0xC054:
                return;
            // Set Page 2
            case 0xC055:
                return;
            // Set Low-Res
            case 0xC056:
                cpu->low_res = true;  
                cpu->high_res = false; 
                return; 
            // Set High-Res
            case 0xC057:
                cpu->low_res = false; 
                cpu->high_res = true;  
                return;
        }

        return;
    }

    // Protect Writing to ROM
    if (address >= 0xD000) return;

    cpu->memory[address] = value;
}

void cpu_display_registers(cpu_t *cpu) {
    u8 value = 0;

    value |= cpu->C ? CARRY_FLAG : 0;  
    value |= cpu->Z ? ZERO_FLAG : 0;  
    value |= cpu->I ? INTERRUPT_FLAG : 0;  
    value |= cpu->D ? DECIMAL_FLAG : 0;  
    value |= BREAK_FLAG;
    value |= 0x20;               
    value |= cpu->V ? OVERFLOW_FLAG : 0;  
    value |= cpu->N ? NEGATIVE_FLAG : 0;  

    if (!log) log = fopen("trace.log", "w");
    fprintf(log, "A: %02X, X: %02X, Y: %02X, PC: %04X, SP: %02X, SR: %02X\n",
               cpu->A, cpu->X, cpu->Y, cpu->PC, cpu->SP, value);
}