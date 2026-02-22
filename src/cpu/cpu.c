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

    // PIA State
    cpu->key_ready = false;
    cpu->key_value = 0;

    // Interrupt Handler
    cpu->memory[IRQ_LOW] = 0x86;
    cpu->memory[IRQ_HIGH] = 0xFA;

    // Set reset vector in ROM
    cpu->memory[RESET_LOW] = 0x59;
    cpu->memory[RESET_HIGH] = 0xFF;

    cpu->memory[BRK_LOW] = 0xFB;
    cpu->memory[BRK_HIGH] = 0x03;

    cpu->running = true;
    cpu->global_cycles = 0;

    cpu->PC = (cpu->memory[RESET_HIGH] << 8) | cpu->memory[RESET_LOW];
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

u8 load_program(cpu_t *cpu, const char* rom_path, u16 address)
{
    u8 status = 1;

    // Load File
    FILE *fptr = fopen(rom_path, "rb");
    if (fptr == NULL) return status;

    if (fseek(fptr, 0, SEEK_END) != 0) { 
        fprintf(stderr, "Error seeking to end of file\n");
        fclose(fptr);
        return status;
    }

    long size = ftell(fptr);
    if (size == -1) {
        fprintf(stderr, "Error getting file position\n");
        fclose(fptr);
        return status;
    }
    fseek(fptr, 0, SEEK_SET);

    // Load File into CPU Memory
    size_t bytes_read = fread(cpu->memory + address, sizeof(u8), size, fptr);
    fclose(fptr);

    // Nothing Loaded
    if (!bytes_read) {
        fprintf(stderr, "Nothing was loaded\n");
        return status;
    }

    status = 0;
    return status;
}

u8 read_memory(cpu_t *cpu, u16 address)
{
    // Check for key press
    if (address == 0xC000) {
        if (cpu->key_ready) {
            return cpu->key_value | 0x80;
        }

        return 0x00;
    }

    // Clear key press
    if (address == 0xC010) {
        cpu->key_ready = false;
        return 0;
    }

    return cpu->memory[address];
}

void write_memory(cpu_t *cpu, u16 address, u8 value)
{
    if (address == 0xC010) {
        cpu->key_ready = false;
        return;
    }

    if (address >= 0xD000) return; // ROM is read-only
    
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

void print_memory(cpu_t *cpu, u16 start, u16 end) {
    printf("Memory dump from $%04X to $%04X:\n", start, end);
    for (u16 addr = start; addr <= end; addr++) {
        // Print 16 bytes per line
        if ((addr - start) % 16 == 0) {
            printf("\n%04X: ", addr); // Print address label at the start of each line
        }
        printf("%02X ", read_memory(cpu, addr));
    }
    printf("\n\n");
}