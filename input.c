#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "video.h"
#include "input.h"
#include "utils.h"

void joyp_ctx_update(GBContext *cpu, JOYPContext *joyp) {
    SDL_Event ev;

    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
            case SDL_WINDOWEVENT:
                if (ev.window.event == SDL_WINDOWEVENT_CLOSE) {
                    cpu->exit = 1;
                }
                break;
            case SDL_KEYDOWN:
                switch (ev.key.keysym.sym) {
                    case SDLK_LEFT:
                        joyp->left = 1;
                        break;
                    case SDLK_RIGHT:
                        joyp->right = 1;
                        break;
                    case SDLK_UP:
                        joyp->up = 1;
                        break;
                    case SDLK_DOWN:
                        joyp->down = 1;
                        break;
                    case SDLK_RETURN:
                        joyp->start = 1;
                        break;
                    case SDLK_SPACE:
                        joyp->select = 1;
                        break;
                    case SDLK_j:
                        joyp->a = 1;
                        break;
                    case SDLK_k:
                        joyp->b = 1;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (ev.key.keysym.sym) {
                    case SDLK_LEFT:
                        joyp->left = 0;
                        break;
                    case SDLK_RIGHT:
                        joyp->right = 0;
                        break;
                    case SDLK_UP:
                        joyp->up = 0;
                        break;
                    case SDLK_DOWN:
                        joyp->down = 0;
                        break;
                    case SDLK_RETURN:
                        joyp->start = 0;
                        break;
                    case SDLK_SPACE:
                        joyp->select = 0;
                        break;
                    case SDLK_j:
                        joyp->a = 0;
                        break;
                    case SDLK_k:
                        joyp->b = 0;
                        break;
                }
                break;
        }
    }
    
    //update IO_P1
    if(!GET_BIT(IO_P1, BIT5)) {// select buttons
        VAL_BIT(IO_P1, BIT3, !joyp->start);
        VAL_BIT(IO_P1, BIT2, !joyp->select);
        VAL_BIT(IO_P1, BIT1, !joyp->b);
        VAL_BIT(IO_P1, BIT0, !joyp->a);
    }
    
    if(!GET_BIT(IO_P1, BIT4)) {// select directiosn
        VAL_BIT(IO_P1, BIT3, !joyp->down);
        VAL_BIT(IO_P1, BIT2, !joyp->up);
        VAL_BIT(IO_P1, BIT1, !joyp->left);
        VAL_BIT(IO_P1, BIT0, !joyp->right);
    }
}


JOYPContext *joyp_init(GBContext *cpu) {
    JOYPContext *joyp = malloc(sizeof(JOYPContext));
    memset(joyp, 0, sizeof(JOYPContext));
    
    joyp->io.P1 = cpu->memory + IO_P1_ADDR;
    IO_P1 = 0;

    return joyp;
}

void joyp_destroy(JOYPContext *joyp) {
    free(joyp);
}

