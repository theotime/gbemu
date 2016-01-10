#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "gbemu.h"
#include "cpu.h"
#include "opcodes.h"
#include "video.h"
#include "input.h"

static void  usage(char []);

int main(int argc, char * argv[]) {
    if (argc != 2) {
        usage(argv[0]);
        exit(1);
    }
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Echec init : %s\n", SDL_GetError());
        exit(1);
    }

    GBContext   *cpu = cpu_init(argv[1]);
    GPUContext  *gpu = gpu_init(cpu);
    JOYPContext *joyp = joyp_init(cpu);
    
    uint8_t op;
    unsigned int start_time, last_time, elapsed_time;
    while (!cpu->exit) {
        start_time = SDL_GetTicks();

        while (cpu->t_clock < VSYNC_CYCLES) {
            op = read8(cpu, cpu->PC++);
            execute(cpu, op);
            
            //"realtime" operations must be here..
            cpu_ctx_update(cpu);
            gpu_ctx_update(cpu, gpu);
            joyp_ctx_update(cpu, joyp);
            handle_interrupt(cpu);
        }
        cpu->t_clock = 0;

        last_time = SDL_GetTicks();
        elapsed_time = last_time - start_time;
        if(elapsed_time < VSYNC_TIME_MS) {// just a basic frame rate wait loop
            SDL_Delay(VSYNC_TIME_MS - elapsed_time);
        }
        
        gpu_render(cpu, gpu);
//        printf("elapsed: %d\n", elapsed_time);
    }

    
    joyp_destroy(joyp);
    gpu_destroy(gpu);
    cpu_destroy(cpu);
    SDL_Quit();

    return 0;
}

static void usage(char prog_name[]) {
    printf("%s rom.gb\n", prog_name);
}