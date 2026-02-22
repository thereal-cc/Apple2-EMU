#include "cpu/cpu.h"
#include "interface/interface.h"

int main(int argc, char *argv[])
{
    // Initialize CPU & Interface
    cpu_t cpu;
    interface_t interface;

    cpu_init(&cpu);
    if (!init_interface(&interface))
        return EXIT_FAILURE;

    // Load Applesoft
    u8 status = load_program(&cpu, "./roms/Applesoft", 0xD000);
    if (status)
    {
        fprintf(stderr, "Error: Could not load Applesoft\n");
        return EXIT_FAILURE;
    }

    // Load System Monitor
    status = load_program(&cpu, "./roms/OrigF8ROM", 0xF800);
    if (status)
    {
        fprintf(stderr, "Error: Could not load System Monitor\n");
        return EXIT_FAILURE;
    }

    while (cpu.running)
    {
        for (int i = 0; i < 17030; i++)
        {
            cpu_cycle(&cpu);
        }
        
        poll_keyboard(&interface, &cpu);
        render_text_screen(&interface, &cpu);
        SDL_Delay(16);
    }

    end_interface(&interface);
    return EXIT_SUCCESS;
}