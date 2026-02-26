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
#define LOW_RES_HEIGHT 48
#define LOW_RES_WIDTH 40

// High-Res Mode Defines
#define HGR_WIDTH_MONO 280
#define HGR_WIDTH_COLOR 140
#define HGR_HEIGHT 192

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
    u8 txt_rows;
    u8 txt_cols;

    // Low-Res
    bool mixed_mode;
    u8 low_width;
    u8 low_height;

    // High-Res
    bool color_screen;
    u8 high_width;
    u8 high_height;
} interface_t;

bool init_interface(interface_t *interface);
void poll_keyboard(interface_t *interface, cpu_t *cpu);
void render_text_screen(interface_t *interface, cpu_t *cpu, int start_rows);
void render_lowres_screen(interface_t *interface, cpu_t *cpu, int num_rows);
void render_hires_screen(interface_t *interface, cpu_t *cpu, int num_rows);
void run_display(interface_t *interface, cpu_t *cpu);
void end_interface(interface_t *interface);

#endif