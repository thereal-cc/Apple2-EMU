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
    if (!init_software(&cpu))
    {
        fprintf(stderr, "There was an error loading the Apple II Rom\n");
        cpu.running = false;
    }

    /*
    // Check if there's a disk loaded
    cpu.drive1 = load_disk();
    if (!cpu.drive1.loaded) cpu.running = false;
    */

    u32 last_flash = SDL_GetTicks();

    while (cpu.running)
    {
        u64 frame_start = SDL_GetPerformanceCounter();

        for (int i = 0; i < CYCLES_PER_FRAME; i++)
        {
            cpu_cycle(&cpu);
        }

        poll_keyboard(&interface, &cpu);

        // Determine if Cursor should be flashing (300 ms)
        u32 now = SDL_GetTicks();
        if (now - last_flash >= 300)
        {
            interface.cursor_visible = !interface.cursor_visible;
            last_flash = now;
        }

        run_display(&interface, &cpu);
        SDL_Delay(16);
    }

    end_interface(&interface);
    return EXIT_SUCCESS;
}