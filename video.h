#ifndef VIDEO_H
#define VIDEO_H

#include "cpu.h"

#define LCD_WIDTH  160
#define LCD_HEIGHT 144

#define DISPLAY_WIDTH 256
#define DISPLAY_HEIGHT 256

#define VBLANK_LINES 10

#define VSYNC_TIME_MS 16 //ms
#define VSYNC_CYCLES 70224 //number of cycles for 16ms at 4.194 Mhz from pandocs
#define VLINE_CYCLES 456 // cycles for 1line including mod 0-2-3 (pandocs)

#define TILE_SIZE 8 // pixel
#define TILE_BANK_SIZE 16 // pixel
#define LCD_TILES_W 20//tiles
#define LCD_TILES_H 18
#define DISPLAY_TILES 32

#define OFFSET_X_SPRITE 8
#define OFFSET_Y_SPRITE 16
#define OAM_SPRITES     40

#define TILE_ADDR_BANK0     0x8800
#define TILE_ADDR_BANK1     0x8000
#define DISPLAY_ADDR_MODE0  0x9800
#define DISPLAY_ADDR_MODE1  0x9C00
#define OAM_ADDR            0xFE00 

#define BLACK       0x00
#define DARK_GREY   0x55
#define LIGHT_GREY  0xAA
#define WHITE       0xFF

#define BLACK_PALETTE       3
#define DARK_GREY_PALETTE   2
#define LIGHT_GREY_PALETTE  1
#define WHITE_PALETTE       0

#define BACKGROUND 0
#define SPRITE     1

#define IO_LCDC *(gpu->io.LCDC)
#define IO_STAT *(gpu->io.STAT)
#define IO_SCY  *(gpu->io.SCY)
#define IO_SCX  *(gpu->io.SCX)
#define IO_LY   *(gpu->io.LY)
#define IO_LYC  *(gpu->io.LYC)
#define IO_WY   *(gpu->io.WY)
#define IO_BGP  *(gpu->io.BGP)
#define IO_OBP0 *(gpu->io.OBP0)
#define IO_OBP1 *(gpu->io.OBP1)
#define IO_DMA  *(gpu->io.DMA)

typedef struct {
    uint8_t x; //2D, tile (32)
    uint8_t y; //2D, tile (32) 
    
    unsigned int i; //1D array, tile (32*32)
} GBCoordinates_t;

typedef struct {
    uint8_t x; //2D pixel (256)
    uint8_t y; //2D pixel (256)
    
    unsigned int i; //1D array, pixel (256*256)
} GBCoordinates_p;

typedef enum {
    MODE_PIXEL,
    MODE_TILE
} GBCoordinates_mode;

typedef enum {
    IO_LCDC_ADDR    = 0xFF40,
    IO_STAT_ADDR    = 0xFF41,
    IO_SCY_ADDR     = 0xFF42,
    IO_SCX_ADDR     = 0xFF43,
    IO_LY_ADDR      = 0xFF44,
    IO_LYC_ADDR     = 0xFF45,
    IO_WY_ADDR      = 0xFF4A,
    IO_BGP_ADDR     = 0xFF47,
    IO_OBP0_ADDR    = 0xFF48,
    IO_OBP1_ADDR    = 0xFF49,
    IO_DMA_ADDR     = 0xFF46
} GPURegs_addr;

typedef struct {
    uint8_t *LCDC;
    uint8_t *STAT;
    uint8_t *SCY;
    uint8_t *SCX;
    uint8_t *LY;
    uint8_t *LYC;
    uint8_t *WY;
    uint8_t *BGP;
    uint8_t *OBP0;
    uint8_t *OBP1;
    uint8_t *DMA;
} GPURegs;

typedef struct {
    uint8_t y;
    uint8_t x;
    uint8_t tile;
    uint8_t attr;
} Sprite;

typedef struct {
    GPURegs io;
    Sprite *oam; // oam[OAM_SPRITES]
    unsigned int vblank_state;
    
    // SDL display 
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;
    SDL_Texture *sdl_texture;
    uint32_t *sdl_pixels;

} GPUContext;


void         gpu_ctx_update(GBContext *, GPUContext *);
void         gpu_render(GBContext *, GPUContext *);
GPUContext  *gpu_init(GBContext *);
void         gpu_destroy(GPUContext *);

#endif // VIDEO_H