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

    // Load Software
    if (!init_software(&cpu)) {
        fprintf(stderr, "There was an error loading the Apple II Rom\n");
        cpu.running = false;
    }

    while (cpu.running)
    {
        for (int i = 0; i < CYCLES_PER_FRAME; i++)
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