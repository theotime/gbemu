#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "gbemu.h"
#include "video.h"
#include "cpu.h"


static uint32_t          render_pixel(GPUContext *, uint8_t, uint8_t);
static void              render_tile(GBContext *, GPUContext *, uint8_t, GBCoordinates_p, unsigned int, uint8_t);

static void              coord_init(uint8_t , uint8_t, GBCoordinates_mode, void *);
static void              coord_init_i(unsigned int, GBCoordinates_mode, void *);
static GBCoordinates_p   coord_to_pixel(GBCoordinates_t);
static GBCoordinates_t   coord_to_tile(GBCoordinates_p);


// HW realtime operations
void gpu_ctx_update(GBContext *cpu, GPUContext *gpu) {
    IO_LY = cpu->t_clock / VLINE_CYCLES;
    
    if (cpu->t_clock >= VSYNC_CYCLES) {
        IO_LY = 0;
        gpu->vblank_state = 0;
    }
    
    if ((IO_LY == 144) && (gpu->vblank_state == 0)) {
        gpu->vblank_state = 1;
        generate_interrupt(cpu, INT_VBLANK);
    }
    
    if(IO_LY == IO_LYC) {
        //XXX todo
    }
    
    // IO_STAT // ignored for the moment.
}

static uint32_t render_pixel(GPUContext *gpu, uint8_t pixel, uint8_t palette) {
    uint32_t final_pixel;

    int b0, b1;
    uint8_t color = 0;
    b0 = GET_BIT(palette, 1 << (pixel * 2));
    b1 = GET_BIT(palette, 1 << ((pixel * 2) + 1));

    VAL_BIT(color, BIT1, b1);
    VAL_BIT(color, BIT0, b0);
    
    switch(color) {
        case WHITE_PALETTE:
            final_pixel = SDL_MapRGBA(SDL_GetWindowSurface(gpu->sdl_window)->format, WHITE, WHITE, WHITE, 255);
            break;
        case LIGHT_GREY_PALETTE:
            final_pixel = SDL_MapRGBA(SDL_GetWindowSurface(gpu->sdl_window)->format, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, 255);
            break;
        case DARK_GREY_PALETTE:
            final_pixel = SDL_MapRGBA(SDL_GetWindowSurface(gpu->sdl_window)->format, DARK_GREY, DARK_GREY, DARK_GREY, 255);
            break;
        case BLACK_PALETTE:
            final_pixel = SDL_MapRGBA(SDL_GetWindowSurface(gpu->sdl_window)->format, BLACK, BLACK, BLACK, 255);
            break;
    }
    
    return final_pixel;
}

static void render_tile(GBContext *cpu, GPUContext *gpu, uint8_t tile_id, GBCoordinates_p pos_p, unsigned int mode, uint8_t attr) {
    uint16_t tile_bank_addr = GET_BIT(IO_LCDC, BIT4) ? TILE_ADDR_BANK1 : TILE_ADDR_BANK0;// XXX handle TILE_ADDR_BANK0 as int8_t

    int i = 0;
    for (; i < (TILE_SIZE * 2); i+=2) {//access by line, 1 line is encoded in 2bytes
        int x, y = 0;
        int b0, b1;
        uint8_t pixel = 0;
        for (x = 7; x >= 0; x--) {//render one line
            //access 2bytes
            b0 = GET_BIT(read8(cpu, tile_bank_addr + (tile_id * TILE_BANK_SIZE) + i), 1 << x);
            b1 = GET_BIT(read8(cpu, tile_bank_addr + (tile_id * TILE_BANK_SIZE) + i + 1), 1 << x);

            VAL_BIT(pixel, BIT1, b1);
            VAL_BIT(pixel, BIT0, b0);


            if ((mode == SPRITE) && (pixel != 0)) {
                uint8_t palette = GET_BIT(attr, BIT4) ? IO_OBP1 : IO_OBP0;
                gpu->sdl_pixels[pos_p.i + y] = render_pixel(gpu, pixel, palette);
            }
            
            if(mode == BACKGROUND) {
                gpu->sdl_pixels[pos_p.i + y] = render_pixel(gpu, pixel, IO_BGP);
            }
            
            y++;
        }
        
        pos_p.i += (DISPLAY_TILES * TILE_SIZE);

    }
}

//execute at each frame refresh
void gpu_render(GBContext *cpu, GPUContext *gpu) {
    GBCoordinates_t pos;
    GBCoordinates_p obj;
    int pitch;
    
    SDL_LockTexture(gpu->sdl_texture, NULL, &gpu->sdl_pixels, &pitch);
    SDL_Rect screen = {0, 0, LCD_WIDTH, LCD_HEIGHT};
    
    if (GET_BIT(IO_LCDC, BIT7)) {
        uint16_t display_addr;
        int i = 0;
        screen.x = IO_SCX;
        screen.y = IO_SCY;

        //print BG
        if (GET_BIT(IO_LCDC, BIT0)) {
            display_addr = GET_BIT(IO_LCDC, BIT3) ? DISPLAY_ADDR_MODE1 : DISPLAY_ADDR_MODE0;

            for (; i < DISPLAY_TILES * DISPLAY_TILES; i++) {
                coord_init_i(i, MODE_TILE, &pos);
                render_tile(cpu, gpu, read8(cpu, display_addr + i), coord_to_pixel(pos), BACKGROUND, 0);
            }
        }
        
        //print window
        if (GET_BIT(IO_LCDC, BIT5)) {
            display_addr = GET_BIT(IO_LCDC, BIT6) ? DISPLAY_ADDR_MODE1 : DISPLAY_ADDR_MODE0;
            
            //XXX todo
        }
        
        //print sprites
        if (GET_BIT(IO_LCDC, BIT1)) {
            //handle only 8*8 sprite
            i = OAM_SPRITES - 1;
            for (; i >= 0; i--) {
                coord_init(IO_SCX + gpu->oam[i].x - OFFSET_X_SPRITE, IO_SCY + gpu->oam[i].y - OFFSET_Y_SPRITE, MODE_PIXEL, &obj);
                render_tile(cpu, gpu, gpu->oam[i].tile, obj, SPRITE, gpu->oam[i].attr);
            }
        }
        
    } else {
        memset(gpu->sdl_pixels, WHITE, sizeof (uint32_t) * DISPLAY_WIDTH * DISPLAY_HEIGHT);
    }

    SDL_UnlockTexture(gpu->sdl_texture);

    SDL_RenderClear(gpu->sdl_renderer);
    SDL_RenderCopy(gpu->sdl_renderer, gpu->sdl_texture, &screen, NULL);
    SDL_RenderPresent(gpu->sdl_renderer);
}

GPUContext *gpu_init(GBContext *cpu) {
    GPUContext *gpu;
    
    gpu = malloc(sizeof (GPUContext));
    memset(gpu, 0, sizeof (GPUContext));
    
    gpu->sdl_window = SDL_CreateWindow(__APP_NAME__, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LCD_WIDTH, LCD_HEIGHT, SDL_WINDOW_SHOWN);
    if (!gpu->sdl_window) {
        return NULL;
    }
    
    gpu->sdl_renderer = SDL_CreateRenderer(gpu->sdl_window, -1, 0);
    gpu->sdl_texture = SDL_CreateTexture(gpu->sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    

    // link register to the CPU memory
    gpu->io.LCDC    = cpu->memory + IO_LCDC_ADDR;
    gpu->io.STAT    = cpu->memory + IO_STAT_ADDR;
    gpu->io.SCY     = cpu->memory + IO_SCY_ADDR;
    gpu->io.SCX     = cpu->memory + IO_SCX_ADDR;
    gpu->io.LY      = cpu->memory + IO_LY_ADDR;
    gpu->io.LYC     = cpu->memory + IO_LYC_ADDR;
    gpu->io.WY      = cpu->memory + IO_WY_ADDR;
    gpu->io.BGP     = cpu->memory + IO_BGP_ADDR;
    gpu->io.OBP0    = cpu->memory + IO_OBP0_ADDR;
    gpu->io.OBP1    = cpu->memory + IO_OBP1_ADDR;
    gpu->io.DMA     = cpu->memory + IO_DMA_ADDR;

    // set all registers to zero.
    IO_LCDC = 0;
    IO_STAT = 0;
    IO_SCY  = 0;
    IO_SCX  = 0;
    IO_LY   = 0;
    IO_LYC  = 0;
    IO_WY   = 0;
    IO_BGP  = 0;
    IO_OBP0 = 0;
    IO_OBP1 = 0;
    IO_DMA  = 0;
    
    
    gpu->oam = (void *)(cpu->memory + OAM_ADDR);
    gpu->vblank_state = 0;
    
    return gpu;
    
}

void gpu_destroy(GPUContext *gpu) {
    SDL_DestroyTexture(gpu->sdl_texture);
    SDL_DestroyRenderer(gpu->sdl_renderer);
    SDL_DestroyWindow(gpu->sdl_window);
    free(gpu);
}

static void coord_init(uint8_t x, uint8_t y, GBCoordinates_mode mode, void *c) {
    if (mode == MODE_PIXEL) {
        ((GBCoordinates_p *) c)->x = x;
        ((GBCoordinates_p *) c)->y = y;
        ((GBCoordinates_p *) c)->i = (y * DISPLAY_WIDTH) + x;
    } else if (mode == MODE_TILE) {
        ((GBCoordinates_t *) c)->x = x;
        ((GBCoordinates_t *) c)->y = y;
        ((GBCoordinates_t *) c)->i = (y * DISPLAY_TILES) + x;
    }
}

static void coord_init_i(unsigned int i, GBCoordinates_mode mode, void *c) {
    if (mode == MODE_PIXEL) {
        ((GBCoordinates_p *) c)->i = i;
        ((GBCoordinates_p *) c)->x = i % DISPLAY_WIDTH;
        ((GBCoordinates_p *) c)->y = i / DISPLAY_WIDTH;
    } else if (mode == MODE_TILE) {
        ((GBCoordinates_t *) c)->i = i;
        ((GBCoordinates_t *) c)->x = i % DISPLAY_TILES;
        ((GBCoordinates_t *) c)->y = i / DISPLAY_TILES;
    }
}

static GBCoordinates_p coord_to_pixel(GBCoordinates_t pos_t) {
    GBCoordinates_p pos_p;

    pos_p.x = pos_t.x * TILE_SIZE;
    pos_p.y = pos_t.y * TILE_SIZE;
    pos_p.i = (pos_p.y * DISPLAY_WIDTH) + pos_p.x;

    return pos_p;
}

static GBCoordinates_t coord_to_tile(GBCoordinates_p pos_p) {
    GBCoordinates_t pos_t;

    pos_t.x = pos_p.x / TILE_SIZE; // convert to the floor and ignore the reminder offset;
    pos_t.y = pos_p.y / TILE_SIZE; // convert to the floor and ignore the reminder offset;
    pos_t.i = (pos_t.y * DISPLAY_TILES) + pos_t.x;

    return pos_t;
}