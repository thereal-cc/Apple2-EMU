#ifndef INTERFACE_H
#define INTERFACE_H

#include "utils/util.h"
#include "cpu/cpu.h"
#include "SDL3/SDL.h"

// Text Mode Defines
#define CHAR_WIDTH 7
#define CHAR_HEIGHT 8
#define STD_COL 40
#define EXT_COL 80
#define TXT_ROW 24

// Low-Res Mode Defines
#define LOW_RES_WIDTH 48
#define LOW_RES_HEIGHT 40

// High-Res Mode Defines
#define HIGH_RES_WIDTH 280
#define HIGH_RES_HEIGHT 192

enum RENDER_MODE
{
    TEXT,
    LOW,
    HIGH,
};


typedef struct 
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    u8 render_mode;
    
    // Text Mode
    bool cursor_visible;
    u8 num_rows;
    u8 num_cols;

    // Low-Res

    // High-Res

} interface_t;

bool init_interface(interface_t *interface);
void poll_keyboard(interface_t *interface, cpu_t *cpu);
void render_text_screen(interface_t *interface, cpu_t *cpu);
void run_display(interface_t *interface, cpu_t *cpu);
void end_interface(interface_t *interface);

#endif