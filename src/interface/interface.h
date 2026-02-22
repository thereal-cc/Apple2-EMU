#ifndef INTERFACE_H
#define INTERFACE_H

#include "utils/util.h"
#include "cpu/cpu.h"
#include "SDL3/SDL.h"

#define CHAR_WIDTH 7
#define CHAR_HEIGHT 8

typedef struct 
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} interface_t;

bool init_interface(interface_t *interface);
void poll_keyboard(interface_t *interface, cpu_t *cpu);
void render_text_screen(interface_t *interface, cpu_t *cpu);
void run_display(interface_t *interface, cpu_t *cpu);
void end_interface(interface_t *interface);

#endif