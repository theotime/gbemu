#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"
#include "cpu.h"


static void  cpu_load_file(GBContext *, char [], uint16_t );
static void  print_regbin(char *, uint8_t);


uint8_t read8(GBContext *cpu, uint16_t addr) {
    return cpu->memory[addr];
}

uint16_t read16(GBContext *cpu, uint16_t addr) {
    uint8_t lsb = read8(cpu, addr);
    uint8_t msb = read8(cpu, addr + 1);
    return (msb << 8) | lsb;
}

void write8(GBContext *cpu, uint16_t addr, uint8_t data) {
    cpu->memory[addr] = data;
}

void write16(GBContext *cpu, uint16_t addr, uint16_t data) {
    write8(cpu, addr, data);
    write8(cpu, addr + 1, data >> 8);
}

// ALU

uint8_t do_arithmetic8(GBContext *cpu, uint8_t value, int is_sub, int carry) {

    uint16_t result;

    if (is_sub) {
        SETFLAG(F_N);
        VALFLAG(F_H, GET_BIT((REG_A & 0xf) - (value & 0xf), BIT4));

        result = REG_A - value;
        if (carry && GETFLAG(F_C)) {
            result--;
        }

    } else {
        RESFLAG(F_N);
        VALFLAG(F_H, GET_BIT((REG_A & 0xf) + (value & 0xf), BIT4));

        result = REG_A + value;
        if (carry && GETFLAG(F_C)) {
            result++;
        }
    }

    VALFLAG(F_Z, ((result & 0xFF) == 0));
    VALFLAG(F_C, GET_BIT(result, BIT8));

    return (uint8_t) (result & 0xFF);
}

void do_arithmetic16(GBContext *cpu, uint16_t value) {
    uint32_t result;
    
    RESFLAG(F_N);
    VALFLAG(F_H, GET_BIT((REG_HL & 0xfff) + (value & 0xfff), BIT12));
    result = REG_HL + value;
    VALFLAG(F_C, GET_BIT(result, BIT16));
    
    REG_HL = (uint16_t) (result & 0xFFFF);
    
    return;
}

// ADD SP,e
// LD HL, SP +e
uint16_t do_ADDSPe(GBContext *cpu, int8_t value) {
    uint32_t result;
    
    RESFLAG(F_Z);
    RESFLAG(F_N);
    VALFLAG(F_H, GET_BIT((cpu->SP & 0xfff) + value, BIT12));// Not sure ...
    
    result = cpu->SP + value;
    VALFLAG(F_C, GET_BIT(result, BIT16));
    
    return (uint16_t) (result & 0xFFFF);
}

uint8_t do_incdec8(GBContext *cpu, uint8_t value, int is_dec) {

    if (is_dec) {
        SETFLAG(F_N);
        value--;
        VALFLAG(F_H, (value & 0xf) == 0xf);
    } else {
        RESFLAG(F_N);
        value++;
        VALFLAG(F_H, (value & 0xf) == 0);
    }

    VALFLAG(F_Z, (value == 0));

    return value;
}

uint16_t do_incdec16(GBContext *cpu, uint16_t value, int is_dec) {
    if (is_dec) {
        value--;
    } else {
        value++;
    }
    
    return value;
}

void do_logic(GBContext *cpu, uint8_t value, int operation) {

    RESFLAG(F_N);
    RESFLAG(F_C);

    switch (operation) {
        case AND:
            SETFLAG(F_H);
            REG_A &= value;
            break;
        case OR:
            RESFLAG(F_H);
            REG_A |= value;
            break;
        case XOR:
            RESFLAG(F_H);
            REG_A ^= value;
            break;
    }

    VALFLAG(F_Z, (REG_A == 0));

    return;
}

void do_bit(GBContext *cpu, int b, uint8_t value) {
    VALFLAG(F_Z, !GET_BIT(value, b));
    SETFLAG(F_H);
    RESFLAG(F_N);
}

//don't wast time on search & replace....
void do_res(GBContext *cpu, int b, uint8_t value) {
    RES_BIT(value, b);
}

void do_set(GBContext *cpu, int b, uint8_t value) {
    RES_BIT(value, b);
}

uint8_t do_shift(GBContext *cpu, uint8_t value, int operation) {

    RESFLAG(F_N);
    RESFLAG(F_H);

    switch (operation) {
        case SLA:
            VALFLAG(F_C, GET_BIT(value, BIT7));
            value <<= 1;
            break;
        case SRA:
            VALFLAG(F_C, GET_BIT(value, BIT0));
            value >>= 1;
            VAL_BIT(value, BIT7, GET_BIT(value, BIT6));
            break;
        case SRL:
            VALFLAG(F_C, GET_BIT(value, BIT0));
            value >>= 1;
            break;
    }
    
    VALFLAG(F_Z, (value == 0));
    
    return value;
}



uint8_t do_rotate(GBContext *cpu, uint8_t value, int operation, int use_acc) {

    RESFLAG(F_N);
    RESFLAG(F_H);

    int CY = 0;

    switch (operation) {
        case RLC:
            VALFLAG(F_C, GET_BIT(value, BIT7));
            value <<= 1;
            VAL_BIT(value, BIT0, GETFLAG(F_C));
            break;
        case RL:
            CY = GETFLAG(F_C);
            VALFLAG(F_C, GET_BIT(value, BIT7));
            value <<= 1;
            VAL_BIT(value, BIT0, CY);
            break;
        case RRC:
            VALFLAG(F_C, GET_BIT(value, BIT0));
            value >>= 1;
            VAL_BIT(value, BIT7, GETFLAG(F_C));
            break;
        case RR:
            CY = GETFLAG(F_C);
            VALFLAG(F_C, GET_BIT(value, BIT0));
            value >>= 1;
            VAL_BIT(value, BIT7, CY);
            break;
    }

    if (use_acc) {
        RESFLAG(F_Z);
    } else {
        VALFLAG(F_Z, (value == 0));
    }
    
    return value;

}

uint8_t do_SWAP(GBContext *cpu, uint8_t value) {
    uint8_t hi = value & 0xf0;
    uint8_t lo = value & 0xf;

    value = (lo << 4) | (hi >> 4);
    VALFLAG(F_Z, (value == 0));
    
    RESFLAG(F_N);
    RESFLAG(F_H);
    RESFLAG(F_C);
    
    return value;
}

void do_DAA(void) {
    //XXX TODO
}

void do_CPL(GBContext *cpu) {
    REG_A = ~REG_A;
    
    SETFLAG(F_N);
    SETFLAG(F_H);
}

void do_CCF(GBContext *cpu) {
    RESFLAG(F_N);
    RESFLAG(F_H);
    VALFLAG(F_C, !GETFLAG(F_C));
}

void do_SCF(GBContext *cpu) {
    RESFLAG(F_N);
    RESFLAG(F_H);
    SETFLAG(F_C);
}

void do_push(GBContext *cpu, uint16_t value) {
    write16(cpu, cpu->SP-2, value);
    cpu->SP -= 2;
}

uint16_t do_pop(GBContext *cpu) {
    uint16_t value = read16(cpu, cpu->SP);
    cpu->SP += 2;
    return value;
}

static void cpu_load_file(GBContext *cpu, char filename[], uint16_t start_addr) {
    
    FILE *fp = fopen(filename, "rb");
    if(!fp) {
        printf("There is no such ROM file : %s\n", filename);
        exit(1);
    }

    int c;
    uint16_t addr = 0;
    while ((c = fgetc(fp)) != EOF) {
        if (addr >= start_addr) {
            write8(cpu, addr, (uint8_t) c);
        } else {
            cpu->boot[addr] = c;
        }
        addr++;
    }

    if (ferror(fp)) {
        printf("Read error with ROM file : %s\n", filename);
        exit(1);
    }
    
}

// HW realtime operations
void cpu_ctx_update(GBContext *cpu) {
    if (cpu->memory[IOPORT_ADDR + 0x50] == 1) {
        memcpy(cpu->memory, cpu->boot, BOOT_SIZE);
        cpu->memory[IOPORT_ADDR + 0x50] = 0; //I think on real hw, register is not reset.
    }
}

GBContext *cpu_init(char rom_name[]) {
    GBContext *cpu;
    
    cpu = malloc(sizeof (GBContext));
    memset(cpu, 0, sizeof (GBContext));
    cpu->memory = malloc(MEM_SIZE);
    memset(cpu->memory, 0x0, MEM_SIZE);

    cpu->I.IE = cpu->memory + IO_IE_ADDR;
    cpu->I.IF = cpu->memory + IO_IF_ADDR;
    
    IO_IE = 0;
    IO_IF = 0;
    
    cpu->boot = malloc(BOOT_SIZE);
    memset(cpu->boot, 0x0, BOOT_SIZE);
    
    cpu_load_file(cpu, __BOOTROM_PATH__, BOOT_ENTRY);
    cpu->PC = BOOT_ENTRY;
    cpu_load_file(cpu, rom_name, BOOT_SIZE);
    
    cpu->exit = 0;
    
    return cpu;
}

void cpu_destroy(GBContext *cpu) {
    free(cpu->memory);
    free(cpu->boot);
    free(cpu);
}

// must be called after each opcodes and after each device ctx update
void handle_interrupt(GBContext *cpu) {
    if(!cpu->IME) {
        return;
    }

    /* Highest priority */
    if (GET_BIT(IO_IF, BIT0) && GET_BIT(IO_IE, BIT0)) {
        cpu->IME = 0;
        RES_BIT(IO_IF, BIT0);
        do_push(cpu, cpu->PC);
        cpu->PC = INT_VBLANK;
        return;
    }
    
    if (GET_BIT(IO_IF, BIT1) && GET_BIT(IO_IE, BIT1)) {
        cpu->IME = 0;
        RES_BIT(IO_IF, BIT1);
        do_push(cpu, cpu->PC);
        cpu->PC = INT_LCDC;
        return;
    }
    
    if (GET_BIT(IO_IF, BIT2) && GET_BIT(IO_IE, BIT2)) {
        cpu->IME = 0;
        RES_BIT(IO_IF, BIT2);
        do_push(cpu, cpu->PC);
        cpu->PC = INT_TIMER;
        return;
    }
    
    if (GET_BIT(IO_IF, BIT3) && GET_BIT(IO_IE, BIT3)) {
        cpu->IME = 0;
        RES_BIT(IO_IF, BIT3);
        do_push(cpu, cpu->PC);
        cpu->PC = INT_SERIAL;
        return;
    }
    
    /* Lowest priority */
    if (GET_BIT(IO_IF, BIT4) && GET_BIT(IO_IE, BIT4)) {
        cpu->IME = 0;
        RES_BIT(IO_IF, BIT4);
        do_push(cpu, cpu->PC);
        cpu->PC = INT_INPUT;
        return;
    }
    
}

void generate_interrupt(GBContext *cpu, GBInterrupts i) {
    switch (i) {
        case INT_VBLANK:
            SET_BIT(IO_IF, BIT0);
            break;
        case INT_LCDC:
            SET_BIT(IO_IF, BIT1);
            break;
        case INT_TIMER:
            SET_BIT(IO_IF, BIT2);
            break;
        case INT_SERIAL:
            SET_BIT(IO_IF, BIT3);
            break;
        case INT_INPUT:
            SET_BIT(IO_IF, BIT4);
            break;
    }
}

void dump_ctx(GBContext *cpu) {
    printf("+------------------------ CPU STATE ------------------------------+\n");
    printf(" AF: %04x BC: %04x DE: %04x HL: %04x   \n", REG_AF, REG_BC, REG_DE, REG_HL);
    printf(" PC: %04x SP: %04x   \n", cpu->PC, cpu->SP);
    printf(" Flags:  ");
    printf(" Z: %d  N: %d  H: %d  C: %d  -: -  -: -  -: -  -: - ", GETFLAG(F_Z), GETFLAG(F_N), GETFLAG(F_H), GETFLAG(F_C));
    printf("\n\n");
    printf(" UNSIGNED   A: %d B: %d C: %d D: %d E: %d H: %d L: %d    \n", REG_A, REG_B, REG_C, REG_D, REG_E, REG_H, REG_L);
    printf("            AF: %d BC: %d DE: %d HL: %d    \n", REG_AF, REG_BC, REG_DE, REG_HL);
    printf(" SIGNED     A: %d B: %d C: %d D: %d E: %d H: %d L: %d    \n", (int8_t) REG_A, (int8_t) REG_B, (int8_t) REG_C, (int8_t) REG_D, (int8_t) REG_E, (int8_t) REG_H, (int8_t) REG_L);
    printf("            AF: %d BC: %d DE: %d HL: %d    \n", (int16_t) REG_AF, (int16_t) REG_BC, (int16_t) REG_DE, (int16_t) REG_HL);
    printf(" BINARY     ");

    print_regbin("A", REG_A);
    print_regbin("B", REG_B);
    print_regbin("C", REG_C);
    printf("\n            ");
    print_regbin("D", REG_D);
    print_regbin("E", REG_E);
    print_regbin("H", REG_H);
    print_regbin("L", REG_L);
    printf("\n");

    printf("+-----------------------------------------------------------------+\n");
}

// addressable space only

void dump_memory(GBContext *cpu, uint16_t start_addr, uint16_t end_addr) {
    uint16_t i = start_addr;
    uint16_t j = end_addr;

    if (i > j) {
        i = 0;
        j = MEM_SIZE - 1;
    }

    for (; i < (j + 1); i++) {
        if ((i % 16) == 0) {
            printf("\n%08x:", i);
        }
        printf(" %02x", cpu->memory[i]);

    }
    printf("\n");
}

static void print_regbin(char *str_register, uint8_t integer) {
    printf("%s: %d%d%d%d%d%d%d%d ", str_register,
            GET_BIT(integer, BIT7),
            GET_BIT(integer, BIT6),
            GET_BIT(integer, BIT5),
            GET_BIT(integer, BIT4),
            GET_BIT(integer, BIT3),
            GET_BIT(integer, BIT2),
            GET_BIT(integer, BIT1),
            GET_BIT(integer, BIT0)
            );

}