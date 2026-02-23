#include "interface.h"
#include "font.h"

u16 row_addresses[24] = {
    0x0400, 0x0480, 0x0500, 0x0580,
    0x0600, 0x0680, 0x0700, 0x0780,
    0x0428, 0x04A8, 0x0528, 0x05A8,
    0x0628, 0x06A8, 0x0728, 0x07A8,
    0x0450, 0x04D0, 0x0550, 0x05D0,
    0x0650, 0x06D0, 0x0750, 0x07D0
};

bool init_interface(interface_t *interface)
{
    // Initialize Video
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return false;
    }

    // Initialize Window
    interface->window = SDL_CreateWindow("Apple2-EMU", 560, 384, SDL_WINDOW_OPENGL);
    if (interface->window == NULL) {
        SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // Initialize Renderer
    interface->renderer = SDL_CreateRenderer(interface->window, NULL);
    if (interface->renderer == NULL) {
        SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError());
        SDL_DestroyWindow(interface->window);
        SDL_Quit();
        return false;
    }

    interface->render_mode = TEXT;
    interface->cursor_visible = true;
    interface->num_cols = STD_COL;
    interface->num_rows = TXT_ROW;
    return true;
}

void poll_keyboard(interface_t *interface, cpu_t *cpu)
{
    SDL_Event event;
    SDL_KeyboardEvent current_key;
    int key_hit = 0;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                cpu->running = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                current_key = event.key;

                // Handle letters
                if (current_key.key >= SDLK_A && current_key.key <= SDLK_Z) {
                    if (current_key.mod & SDL_KMOD_CTRL)
                        key_hit = (current_key.key - SDLK_A) + 1;
                    else
                        key_hit = (current_key.key - SDLK_A) + 'A';
                }

                // Handle numbers
                if (current_key.key >= SDLK_0 && current_key.key <= SDLK_9) {
                    const char shifted[] = ")!@#$%^&*(";
                    if (current_key.mod & SDL_KMOD_SHIFT)
                        key_hit = shifted[current_key.key - SDLK_0];
                    else
                        key_hit = (current_key.key - SDLK_0) + '0';
                }

                // Handle Singular Characters
                switch (current_key.key) {
                    case SDLK_RETURN:    key_hit = '\r'; break;
                    case SDLK_BACKSPACE: key_hit = '\b'; break;
                    case SDLK_SPACE:     key_hit = ' ';  break;
                    case SDLK_PERIOD:    key_hit = current_key.mod & SDL_KMOD_SHIFT ? '>' : '.'; break;
                    case SDLK_COMMA:     key_hit = current_key.mod & SDL_KMOD_SHIFT ? '<' : ','; break;
                    case SDLK_SLASH:     key_hit = current_key.mod & SDL_KMOD_SHIFT ? '?' : '/'; break;
                    case SDLK_SEMICOLON: key_hit = current_key.mod & SDL_KMOD_SHIFT ? ':' : ';'; break;
                    case SDLK_MINUS:     key_hit = current_key.mod & SDL_KMOD_SHIFT ? '_' : '-'; break;
                    case SDLK_EQUALS:    key_hit = current_key.mod & SDL_KMOD_SHIFT ? '+' : '='; break;
                    case SDLK_APOSTROPHE:     key_hit = current_key.mod & SDL_KMOD_SHIFT ? '\"' : '\''; break;
                }

                if (key_hit) {
                    cpu->key_value = key_hit & 0x7F;
                    cpu->key_ready = true;
                    key_hit = 0;
                }
                break;

            case SDL_EVENT_KEY_UP:
                break;
        }
    }
}

void render_text_screen(interface_t *interface, cpu_t *cpu)
{
    SDL_SetRenderDrawColor(interface->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(interface->renderer);

    for (int row = 0; row < interface->num_rows; row++) {
        u16 base_addr = row_addresses[row];

        for (int col = 0; col < interface->num_cols; col++) {
            u8 video_byte = cpu->memory[base_addr + col];

            // Determine render mode from top 2 bits
            u8 mode;
            u8 top2 = video_byte & 0xC0;
            switch (top2) {
                case 0x00:
                    mode = 1; // Inverse
                    break;
                case 0x40:
                    mode = interface->cursor_visible ? 0 : 1;; // Flashing
                    break;
                default:
                    mode = 0; // Normal
                    break;
            }

            u8 char_index = video_byte & 0x3F;
            if (char_index >= 0x20)
                char_index -= 0x20;
            else
                char_index += 0x20;

            int x = col * CHAR_WIDTH;
            int y = row * CHAR_HEIGHT;

            for (int py = 0; py < CHAR_HEIGHT; py++) {
                u8 glyph_row = apple2_font[char_index][py];

                for (int px = 0; px < CHAR_WIDTH; px++) {
                    bool lit = (glyph_row >> px) & 1;

                    // inverse: flip pixels
                    if (mode == 1) lit = !lit;

                    SDL_SetRenderDrawColor(
                        interface->renderer,
                        lit ? 0   : 0,    // R  
                        lit ? 255 : 0,    // G  
                        lit ? 0   : 0,    // B
                        255
                    );

                    SDL_FRect rect = {
                        (float)(x + px) * 2,
                        (float)(y + py) * 2,
                        (float)2,
                        (float)2
                    };
                    SDL_RenderFillRect(interface->renderer, &rect);
                }
            }
        }
    }

    SDL_RenderPresent(interface->renderer);
}

void run_display(interface_t *interface, cpu_t *cpu)
{ 
    SDL_SetRenderDrawColor(interface->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(interface->renderer);
    SDL_RenderPresent(interface->renderer);
}

void end_interface(interface_t *interface)
{
    SDL_DestroyRenderer(interface->renderer);
    SDL_DestroyWindow(interface->window);
    SDL_Quit();
}