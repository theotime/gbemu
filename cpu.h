#ifndef CPU_H
#define CPU_H

#include "utils.h"

#define CPU_FREQ 4194304 //Hz
#define MEM_SIZE 0x10000 // 64k bytes
#define BOOT_SIZE 0x100
#define BOOT_ENTRY 0x0 // Entry to the bootcode
#define ENTRY_POINT 0x100

#define IOPORT_ADDR 0xFF00

#define __BOOTROM_PATH__ "DMG_ROM.bin"


// Syntactic sugar...
// Don't abuse of them.
#define REG_A (cpu->R.b.A)
#define REG_F (cpu->R.b.F)
#define REG_B (cpu->R.b.B)
#define REG_C (cpu->R.b.C)
#define REG_D (cpu->R.b.D)
#define REG_E (cpu->R.b.E)
#define REG_H (cpu->R.b.H)
#define REG_L (cpu->R.b.L)

#define REG_AF (cpu->R.w.AF)
#define REG_BC (cpu->R.w.BC)
#define REG_DE (cpu->R.w.DE)
#define REG_HL (cpu->R.w.HL)

#define IO_IF *(cpu->I.IF)
#define IO_IE *(cpu->I.IE)


#define SETFLAG(F) SET_BIT(REG_F, F)
#define RESFLAG(F) RES_BIT(REG_F, F)
#define GETFLAG(F) GET_BIT(REG_F, F)
#define VALFLAG(F, V) VAL_BIT(REG_F, F, V)

//operations
#define ADD 0
#define SUB 1

#define NO_CARRY 0
#define CARRY 1

#define AND 0
#define OR 1
#define XOR 2

#define INC 0
#define DEC 1

#define SLA 0
#define SRA 1
#define SRL 2

#define RLC 0
#define RL 1
#define RRC 2
#define RR 3

#define NO_ACC 0
#define USE_ACC 1


typedef enum {
    IO_IF_ADDR    = 0xFF0F,
    IO_IE_ADDR    = 0xFFFF,
} INTRegs_addr;

typedef struct {
    uint8_t *IF;
    uint8_t *IE;
} INTRegs;

typedef enum {
    INT_VBLANK = 0x40,/* Highest priority */
    INT_LCDC = 0x48,
    INT_TIMER = 0x50,
    INT_SERIAL = 0x58,
    INT_INPUT = 0x60/* Lowest priority */
} GBInterrupts;

typedef enum {
    _NA1_ = BIT0, // not used
    _NA2_ = BIT1, // not used
    _NA3_ = BIT2, // not used
    _NA4_ = BIT3, // not used
    F_C = BIT4, //Carry cy
    F_H = BIT5, // Half carry h
    F_N = BIT6, // Sub/add n
    F_Z = BIT7, // Zero zf
} GBFlags;

typedef union {

    struct {
        uint16_t AF, BC, DE, HL;
    } w;

    struct {
        uint8_t F, A, C, B, E, D, L, H;
    } b;

} GBRegs;

typedef struct {
    GBRegs R;
    INTRegs I;
    uint16_t SP;
    uint16_t PC;
    uint8_t IME;

    uint8_t *memory; //virtual memory, it will be mapped to real memory (ROM, WRAM, VRAM)
    uint8_t *boot;
    
    unsigned int halted;
    unsigned int t_clock;
    
    unsigned int exit;
} GBContext;


uint8_t      read8(GBContext *, uint16_t);
uint16_t     read16(GBContext *, uint16_t);
void         write8(GBContext *, uint16_t, uint8_t);
void         write16(GBContext *, uint16_t, uint16_t);
uint8_t      do_arithmetic8(GBContext *, uint8_t, int, int);
void         do_arithmetic16(GBContext *, uint16_t);
uint16_t     do_ADDSPe(GBContext *, int8_t);
uint8_t      do_incdec8(GBContext *, uint8_t, int);
uint16_t     do_incdec16(GBContext *, uint16_t, int);
void         do_logic(GBContext *, uint8_t, int);
void         do_bit(GBContext *, int, uint8_t);
void         do_res(GBContext *, int, uint8_t);
void         do_set(GBContext *, int, uint8_t);
uint8_t      do_shift(GBContext *, uint8_t, int);
uint8_t      do_rotate(GBContext *, uint8_t, int, int);
uint8_t      do_SWAP(GBContext *, uint8_t);
void         do_DAA(void);
void         do_CPL(GBContext *);
void         do_CCF(GBContext *);
void         do_SCF(GBContext *);
void         do_push(GBContext *, uint16_t);
uint16_t     do_pop(GBContext *);

void         handle_interrupt(GBContext *);
void         generate_interrupt(GBContext *, GBInterrupts);

void         cpu_ctx_update(GBContext *);
GBContext   *cpu_init(char []);
void         cpu_destroy(GBContext *);

void         dump_ctx(GBContext *);
void         dump_memory(GBContext *, uint16_t, uint16_t);


#endif // CPU_H