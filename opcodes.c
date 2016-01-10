#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "cpu.h"

static void ADCHL(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, read8(cpu, REG_HL), ADD, CARRY);
    cpu->t_clock += 8;
}

static void ADCn(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, read8(cpu, cpu->PC++), ADD, CARRY);
    cpu->t_clock += 8;
}

static void ADCr_a(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_A, ADD, CARRY);
    cpu->t_clock += 4;
}

static void ADCr_b(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_B, ADD, CARRY);
    cpu->t_clock += 4;
}

static void ADCr_c(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_C, ADD, CARRY);
    cpu->t_clock += 4;
}

static void ADCr_d(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_D, ADD, CARRY);
    cpu->t_clock += 4;
}

static void ADCr_e(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_E, ADD, CARRY);
    cpu->t_clock += 4;
}

static void ADCr_h(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_H, ADD, CARRY);
    cpu->t_clock += 4;
}

static void ADCr_l(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_L, ADD, CARRY);
    cpu->t_clock += 4;
}

static void ADDHL(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, read8(cpu, REG_HL), ADD, NO_CARRY);
    cpu->t_clock += 8;
}

static void ADDHLBC(GBContext *cpu) {
    do_arithmetic16(cpu, REG_BC);
    cpu->t_clock += 8;
}

static void ADDHLDE(GBContext *cpu) {
    do_arithmetic16(cpu, REG_DE);
    cpu->t_clock += 8;
}

static void ADDHLHL(GBContext *cpu) {
    do_arithmetic16(cpu, REG_HL);
    cpu->t_clock += 8;
}

static void ADDHLSP(GBContext *cpu) {
    do_arithmetic16(cpu, cpu->SP);
    cpu->t_clock += 8;
}

static void ADDn(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, read8(cpu, cpu->PC++), ADD, NO_CARRY);
    cpu->t_clock += 8;
}

static void ADDr_a(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_A, ADD, NO_CARRY);
    cpu->t_clock += 4;
}

static void ADDr_b(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_B, ADD, NO_CARRY);
    cpu->t_clock += 4;
}

static void ADDr_c(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_C, ADD, NO_CARRY);
    cpu->t_clock += 4;
}

static void ADDr_d(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_D, ADD, NO_CARRY);
    cpu->t_clock += 4;
}

static void ADDr_e(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_E, ADD, NO_CARRY);
    cpu->t_clock += 4;
}

static void ADDr_h(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_H, ADD, NO_CARRY);
    cpu->t_clock += 4;
}

static void ADDr_l(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_L, ADD, NO_CARRY);
    cpu->t_clock += 4;
}

static void ADDSPn(GBContext *cpu) {
    cpu->SP = do_ADDSPe(cpu, read8(cpu, cpu->PC++));
    cpu->t_clock += 16;
}

static void ANDHL(GBContext *cpu) {
    do_logic(cpu, read8(cpu, REG_HL), AND);
    cpu->t_clock += 8;
}

static void ANDn(GBContext *cpu) {
    do_logic(cpu, read8(cpu, cpu->PC++), AND);
    cpu->t_clock += 8;
}

static void ANDr_a(GBContext *cpu) {
    do_logic(cpu, REG_A, AND);
    cpu->t_clock += 4;
}

static void ANDr_b(GBContext *cpu) {
    do_logic(cpu, REG_B, AND);
    cpu->t_clock += 4;
}

static void ANDr_c(GBContext *cpu) {
    do_logic(cpu, REG_C, AND);
    cpu->t_clock += 4;
}

static void ANDr_d(GBContext *cpu) {
    do_logic(cpu, REG_D, AND);
    cpu->t_clock += 4;
}

static void ANDr_e(GBContext *cpu) {
    do_logic(cpu, REG_E, AND);
    cpu->t_clock += 4;
}

static void ANDr_h(GBContext *cpu) {
    do_logic(cpu, REG_H, AND);
    cpu->t_clock += 4;
}

static void ANDr_l(GBContext *cpu) {
    do_logic(cpu, REG_L, AND);
    cpu->t_clock += 4;
}

static void CALLnn(GBContext *cpu) {
    do_push(cpu, cpu->PC + 2);
    cpu->PC = read16(cpu, cpu->PC);
    cpu->t_clock += 24;
}

static void CALLCnn(GBContext *cpu) {
    if (GETFLAG(F_C)) {
        do_push(cpu, cpu->PC + 2);
        cpu->PC = read16(cpu, cpu->PC);
        cpu->t_clock += 24;
    } else {
        cpu->PC += 2;
        cpu->t_clock += 12;
    }
}

static void CALLNCnn(GBContext *cpu) {
    if (!GETFLAG(F_C)) {
        do_push(cpu, cpu->PC + 2);
        cpu->PC = read16(cpu, cpu->PC);
        cpu->t_clock += 24;
    } else {
        cpu->PC += 2;
        cpu->t_clock += 12;
    }
}

static void CALLZnn(GBContext *cpu) {
    if (GETFLAG(F_Z)) {
        do_push(cpu, cpu->PC + 2);
        cpu->PC = read16(cpu, cpu->PC);
        cpu->t_clock += 24;
    } else {
        cpu->PC += 2;
        cpu->t_clock += 12;
    }
}

static void CALLNZnn(GBContext *cpu) {
    if (!GETFLAG(F_Z)) {
        do_push(cpu, cpu->PC + 2);
        cpu->PC = read16(cpu, cpu->PC);
        cpu->t_clock += 24;
    } else {
        cpu->PC += 2;
        cpu->t_clock += 12;
    }
}

static void CCF(GBContext *cpu) {
    do_CCF(cpu);
    cpu->t_clock += 4;
}

static void CPL(GBContext *cpu) {
    do_CPL(cpu);
    cpu->t_clock += 4;
}

static void CPHL(GBContext *cpu) {
    do_arithmetic8(cpu, read8(cpu, REG_HL), SUB, NO_CARRY);
    cpu->t_clock += 8;
}

static void CPn(GBContext *cpu) {
    do_arithmetic8(cpu, read8(cpu, cpu->PC++), SUB, NO_CARRY);
    cpu->t_clock += 8;
}

static void CPr_a(GBContext *cpu) {
    do_arithmetic8(cpu, REG_A, SUB, NO_CARRY);
    cpu->t_clock += 4;
}

static void CPr_b(GBContext *cpu) {
    do_arithmetic8(cpu, REG_B, SUB, NO_CARRY);
    cpu->t_clock += 4;
}

static void CPr_c(GBContext *cpu) {
    do_arithmetic8(cpu, REG_C, SUB, NO_CARRY);
    cpu->t_clock += 4;
}

static void CPr_d(GBContext *cpu) {
    do_arithmetic8(cpu, REG_D, SUB, NO_CARRY);
    cpu->t_clock += 4;
}

static void CPr_e(GBContext *cpu) {
    do_arithmetic8(cpu, REG_E, SUB, NO_CARRY);
    cpu->t_clock += 4;
}

static void CPr_h(GBContext *cpu) {
    do_arithmetic8(cpu, REG_H, SUB, NO_CARRY);
    cpu->t_clock += 4;
}

static void CPr_l(GBContext *cpu) {
    do_arithmetic8(cpu, REG_L, SUB, NO_CARRY);
    cpu->t_clock += 4;
}

static void DECBC(GBContext *cpu) {
    REG_BC = do_incdec16(cpu, REG_BC, DEC);
    cpu->t_clock += 8;
}

static void DECDE(GBContext *cpu) {
    REG_DE = do_incdec16(cpu, REG_DE, DEC);
    cpu->t_clock += 8;
}

static void DECHL(GBContext *cpu) {
    REG_HL = do_incdec16(cpu, REG_HL, DEC);
    cpu->t_clock += 8;
}

static void DECHLm(GBContext *cpu) {
    write8(cpu, REG_HL, do_incdec8(cpu, read8(cpu, REG_HL), DEC));
    cpu->t_clock += 12;
}

static void DECr_a(GBContext *cpu) {
    REG_A = do_incdec8(cpu, REG_A, DEC);
    cpu->t_clock += 4;
}

static void DECr_b(GBContext *cpu) {
    REG_B = do_incdec8(cpu, REG_B, DEC);
    cpu->t_clock += 4;
}

static void DECr_c(GBContext *cpu) {
    REG_C = do_incdec8(cpu, REG_C, DEC);
    cpu->t_clock += 4;
}

static void DECr_d(GBContext *cpu) {
    REG_D = do_incdec8(cpu, REG_D, DEC);
    cpu->t_clock += 4;
}

static void DECr_e(GBContext *cpu) {
    REG_E = do_incdec8(cpu, REG_E, DEC);
    cpu->t_clock += 4;
}

static void DECr_h(GBContext *cpu) {
    REG_H = do_incdec8(cpu, REG_H, DEC);
    cpu->t_clock += 4;
}

static void DECr_l(GBContext *cpu) {
    REG_L = do_incdec8(cpu, REG_L, DEC);
    cpu->t_clock += 4;
}

static void DECSP(GBContext *cpu) {
    cpu->SP = do_incdec16(cpu, cpu->SP, DEC);
    cpu->t_clock += 8;
}

static void STOP(GBContext *cpu) {
    printf("**** ERROR : %s not implemented. ****\n", __func__);
    exit(0);
}

static void EI(GBContext *cpu) {
    cpu->IME = 1;
    cpu->t_clock += 4;
}

static void DI(GBContext *cpu) {
    cpu->IME = 0;
    cpu->t_clock += 4;
}

static void HALT(GBContext *cpu) {
    //    cpu->halted = 1;
    //    cpu->PC--;
    //    cpu->t_clock += 4;
    printf("**** ERROR : %s not implemented. ****\n", __func__);
    exit(0);
}

static void INCBC(GBContext *cpu) {
    REG_BC = do_incdec16(cpu, REG_BC, INC);
    cpu->t_clock += 8;
}

static void INCDE(GBContext *cpu) {
    REG_DE = do_incdec16(cpu, REG_DE, INC);
    cpu->t_clock += 8;
}

static void INCHL(GBContext *cpu) {
    REG_HL = do_incdec16(cpu, REG_HL, INC);
    cpu->t_clock += 8;
}

static void INCHLm(GBContext *cpu) {
    write8(cpu, REG_HL, do_incdec8(cpu, read8(cpu, REG_HL), INC));
    cpu->t_clock += 12;
}

static void INCr_a(GBContext *cpu) {
    REG_A = do_incdec8(cpu, REG_A, INC);
    cpu->t_clock += 4;
}

static void INCr_b(GBContext *cpu) {
    REG_B = do_incdec8(cpu, REG_B, INC);
    cpu->t_clock += 4;
}

static void INCr_c(GBContext *cpu) {
    REG_C = do_incdec8(cpu, REG_C, INC);
    cpu->t_clock += 4;
}

static void INCr_d(GBContext *cpu) {
    REG_D = do_incdec8(cpu, REG_D, INC);
    cpu->t_clock += 4;
}

static void INCr_e(GBContext *cpu) {
    REG_E = do_incdec8(cpu, REG_E, INC);
    cpu->t_clock += 4;
}

static void INCr_h(GBContext *cpu) {
    REG_H = do_incdec8(cpu, REG_H, INC);
    cpu->t_clock += 4;
}

static void INCr_l(GBContext *cpu) {
    REG_L = do_incdec8(cpu, REG_L, INC);
    cpu->t_clock += 4;
}

static void INCSP(GBContext *cpu) {
    cpu->SP = do_incdec16(cpu, cpu->SP, INC);
    cpu->t_clock += 8;
}

static void JPnn(GBContext *cpu) {
    cpu->PC = read16(cpu, cpu->PC);
    cpu->t_clock += 16;
}

static void JPCnn(GBContext *cpu) {
    if (GETFLAG(F_C)) {
        cpu->PC = read16(cpu, cpu->PC);
        cpu->t_clock += 16;
    } else {
        cpu->PC += 2;
        cpu->t_clock += 12;
    }
}

static void JPNCnn(GBContext *cpu) {
    if (!GETFLAG(F_C)) {
        cpu->PC = read16(cpu, cpu->PC);
        cpu->t_clock += 16;
    } else {
        cpu->PC += 2;
        cpu->t_clock += 12;
    }
}

static void JPNZnn(GBContext *cpu) {
    if (GETFLAG(F_Z)) {
        cpu->PC = read16(cpu, cpu->PC);
        cpu->t_clock += 16;
    } else {
        cpu->PC += 2;
        cpu->t_clock += 12;
    }
}

static void JPZnn(GBContext *cpu) {
    if (!GETFLAG(F_Z)) {
        cpu->PC = read16(cpu, cpu->PC);
        cpu->t_clock += 16;
    } else {
        cpu->PC += 2;
        cpu->t_clock += 12;
    }
}

static void JRn(GBContext *cpu) {
    cpu->PC += (int8_t) read8(cpu, cpu->PC) + 1;
    cpu->t_clock += 12;
}

static void JRCn(GBContext *cpu) {
    if (GETFLAG(F_C)) {
        cpu->PC += (int8_t) read8(cpu, cpu->PC) + 1;
        cpu->t_clock += 12;
    } else {
        cpu->PC++;
        cpu->t_clock += 8;
    }
}

static void JRNCn(GBContext *cpu) {
    if (!GETFLAG(F_C)) {
        cpu->PC += (int8_t) read8(cpu, cpu->PC) + 1;
        cpu->t_clock += 12;
    } else {
        cpu->PC++;
        cpu->t_clock += 8;
    }
}

static void JRZn(GBContext *cpu) {
    if (GETFLAG(F_Z)) {
        cpu->PC += (int8_t) read8(cpu, cpu->PC) + 1;
        cpu->t_clock += 12;
    } else {
        cpu->PC++;
        cpu->t_clock += 8;
    }
}

static void JRNZn(GBContext *cpu) {
    if (!GETFLAG(F_Z)) {
        cpu->PC += (int8_t) read8(cpu, cpu->PC) + 1;
        cpu->t_clock += 12;
    } else {
        cpu->PC++;
        cpu->t_clock += 8;
    }
}

static void JPHL(GBContext *cpu) {
    cpu->PC = REG_HL;
    cpu->t_clock += 12;
}

static void LDABCm(GBContext *cpu) {
    REG_A = read8(cpu, REG_BC);
    cpu->t_clock += 8;
}

static void LDADEm(GBContext *cpu) {
    REG_A = read8(cpu, REG_DE);
    cpu->t_clock += 8;
}

static void LDBCmA(GBContext *cpu) {
    write8(cpu, REG_BC, REG_A);
    cpu->t_clock += 8;
}

static void LDDEmA(GBContext *cpu) {
    write8(cpu, REG_DE, REG_A);
    cpu->t_clock += 8;
}

static void LDAmm(GBContext *cpu) {
    REG_A = read8(cpu, read16(cpu, cpu->PC));
    cpu->PC += 2;
    cpu->t_clock += 16;
}

static void LDmmA(GBContext *cpu) {
    write8(cpu, read16(cpu, cpu->PC), REG_A);
    cpu->PC += 2;
    cpu->t_clock += 16;
}

static void LDAHLI(GBContext *cpu) {
    REG_A = read8(cpu, REG_HL++);
    cpu->t_clock += 8;
}

static void LDAHLD(GBContext *cpu) {
    REG_A = read8(cpu, REG_HL--);
    cpu->t_clock += 8;
}

static void LDHLIA(GBContext *cpu) {
    write8(cpu, REG_HL++, REG_A);
    cpu->t_clock += 8;
}

static void LDHLDA(GBContext *cpu) {
    write8(cpu, REG_HL--, REG_A);
    cpu->t_clock += 8;
}

static void LDHLmr_a(GBContext *cpu) {
    write8(cpu, REG_HL, REG_A);
    cpu->t_clock += 8;
}

static void LDHLmr_b(GBContext *cpu) {
    write8(cpu, REG_HL, REG_B);
    cpu->t_clock += 8;
}

static void LDHLmr_c(GBContext *cpu) {
    write8(cpu, REG_HL, REG_C);
    cpu->t_clock += 8;
}

static void LDHLmr_d(GBContext *cpu) {
    write8(cpu, REG_HL, REG_D);
    cpu->t_clock += 8;
}

static void LDHLmr_e(GBContext *cpu) {
    write8(cpu, REG_HL, REG_E);
    cpu->t_clock += 8;
}

static void LDHLmr_h(GBContext *cpu) {
    write8(cpu, REG_HL, REG_H);
    cpu->t_clock += 8;
}

static void LDHLmr_l(GBContext *cpu) {
    write8(cpu, REG_HL, REG_L);
    cpu->t_clock += 8;
}

static void LDHLmn(GBContext *cpu) {
    write8(cpu, REG_HL, read8(cpu, cpu->PC++));
    cpu->t_clock += 12;
}

static void LDAIOC(GBContext *cpu) {
    REG_A = read8(cpu, IOPORT_ADDR + REG_C);
    cpu->t_clock += 8;
}

static void LDIOCA(GBContext *cpu) {
    write8(cpu, IOPORT_ADDR + REG_C, REG_A);
    cpu->t_clock += 8;
}

static void LDAIOn(GBContext *cpu) {
    REG_A = read8(cpu, IOPORT_ADDR + read8(cpu, cpu->PC++));
    cpu->t_clock += 12;
}

static void LDIOnA(GBContext *cpu) {
    write8(cpu, IOPORT_ADDR + read8(cpu, cpu->PC++), REG_A);
    cpu->t_clock += 12;
}

static void LDrHLm_a(GBContext *cpu) {
    REG_A = read8(cpu, REG_HL);
    cpu->t_clock += 8;
}

static void LDrHLm_b(GBContext *cpu) {
    REG_B = read8(cpu, REG_HL);
    cpu->t_clock += 8;
}

static void LDrHLm_c(GBContext *cpu) {
    REG_C = read8(cpu, REG_HL);
    cpu->t_clock += 8;
}

static void LDrHLm_d(GBContext *cpu) {
    REG_D = read8(cpu, REG_HL);
    cpu->t_clock += 8;
}

static void LDrHLm_e(GBContext *cpu) {
    REG_E = read8(cpu, REG_HL);
    cpu->t_clock += 8;
}

static void LDrHLm_h(GBContext *cpu) {
    REG_H = read8(cpu, REG_HL);
    cpu->t_clock += 8;
}

static void LDrHLm_l(GBContext *cpu) {
    REG_L = read8(cpu, REG_HL);
    cpu->t_clock += 8;
}

static void LDrn_a(GBContext *cpu) {
    REG_A = read8(cpu, cpu->PC++);
    cpu->t_clock += 8;
}

static void LDrn_b(GBContext *cpu) {
    REG_B = read8(cpu, cpu->PC++);
    cpu->t_clock += 8;
}

static void LDrn_c(GBContext *cpu) {
    REG_C = read8(cpu, cpu->PC++);
    cpu->t_clock += 8;
}

static void LDrn_d(GBContext *cpu) {
    REG_D = read8(cpu, cpu->PC++);
    cpu->t_clock += 8;
}

static void LDrn_e(GBContext *cpu) {
    REG_E = read8(cpu, cpu->PC++);
    cpu->t_clock += 8;
}

static void LDrn_h(GBContext *cpu) {
    REG_H = read8(cpu, cpu->PC++);
    cpu->t_clock += 8;
}

static void LDrn_l(GBContext *cpu) {
    REG_L = read8(cpu, cpu->PC++);
    cpu->t_clock += 8;
}

static void LDrr_aa(GBContext *cpu) {
    REG_A = REG_A;
    cpu->t_clock += 4;
}

static void LDrr_ab(GBContext *cpu) {
    REG_A = REG_B;
    cpu->t_clock += 4;
}

static void LDrr_ac(GBContext *cpu) {
    REG_A = REG_C;
    cpu->t_clock += 4;
}

static void LDrr_ad(GBContext *cpu) {
    REG_A = REG_D;
    cpu->t_clock += 4;
}

static void LDrr_ae(GBContext *cpu) {
    REG_A = REG_E;
    cpu->t_clock += 4;
}

static void LDrr_ah(GBContext *cpu) {
    REG_A = REG_H;
    cpu->t_clock += 4;
}

static void LDrr_al(GBContext *cpu) {
    REG_A = REG_L;
    cpu->t_clock += 4;
}

static void LDrr_ba(GBContext *cpu) {
    REG_B = REG_A;
    cpu->t_clock += 4;
}

static void LDrr_bb(GBContext *cpu) {
    REG_B = REG_B;
    cpu->t_clock += 4;
}

static void LDrr_bc(GBContext *cpu) {
    REG_B = REG_C;
    cpu->t_clock += 4;
}

static void LDrr_bd(GBContext *cpu) {
    REG_B = REG_D;
    cpu->t_clock += 4;
}

static void LDrr_be(GBContext *cpu) {
    REG_B = REG_E;
    cpu->t_clock += 4;
}

static void LDrr_bh(GBContext *cpu) {
    REG_B = REG_H;
    cpu->t_clock += 4;
}

static void LDrr_bl(GBContext *cpu) {
    REG_B = REG_L;
    cpu->t_clock += 4;
}

static void LDrr_ca(GBContext *cpu) {
    REG_C = REG_A;
    cpu->t_clock += 4;
}

static void LDrr_cb(GBContext *cpu) {
    REG_C = REG_B;
    cpu->t_clock += 4;
}

static void LDrr_cc(GBContext *cpu) {
    REG_C = REG_C;
    cpu->t_clock += 4;
}

static void LDrr_cd(GBContext *cpu) {
    REG_C = REG_D;
    cpu->t_clock += 4;
}

static void LDrr_ce(GBContext *cpu) {
    REG_C = REG_E;
    cpu->t_clock += 4;
}

static void LDrr_ch(GBContext *cpu) {
    REG_C = REG_H;
    cpu->t_clock += 4;
}

static void LDrr_cl(GBContext *cpu) {
    REG_C = REG_L;
    cpu->t_clock += 4;
}

static void LDrr_da(GBContext *cpu) {
    REG_D = REG_A;
    cpu->t_clock += 4;
}

static void LDrr_db(GBContext *cpu) {
    REG_D = REG_B;
    cpu->t_clock += 4;
}

static void LDrr_dc(GBContext *cpu) {
    REG_D = REG_C;
    cpu->t_clock += 4;
}

static void LDrr_dd(GBContext *cpu) {
    REG_D = REG_D;
    cpu->t_clock += 4;
}

static void LDrr_de(GBContext *cpu) {
    REG_D = REG_E;
    cpu->t_clock += 4;
}

static void LDrr_dh(GBContext *cpu) {
    REG_D = REG_H;
    cpu->t_clock += 4;
}

static void LDrr_dl(GBContext *cpu) {
    REG_D = REG_L;
    cpu->t_clock += 4;
}

static void LDrr_ea(GBContext *cpu) {
    REG_E = REG_A;
    cpu->t_clock += 4;
}

static void LDrr_eb(GBContext *cpu) {
    REG_E = REG_B;
    cpu->t_clock += 4;
}

static void LDrr_ec(GBContext *cpu) {
    REG_E = REG_C;
    cpu->t_clock += 4;
}

static void LDrr_ed(GBContext *cpu) {
    REG_E = REG_D;
    cpu->t_clock += 4;
}

static void LDrr_ee(GBContext *cpu) {
    REG_E = REG_E;
    cpu->t_clock += 4;
}

static void LDrr_eh(GBContext *cpu) {
    REG_E = REG_H;
    cpu->t_clock += 4;
}

static void LDrr_el(GBContext *cpu) {
    REG_E = REG_L;
    cpu->t_clock += 4;
}

static void LDrr_ha(GBContext *cpu) {
    REG_H = REG_A;
    cpu->t_clock += 4;
}

static void LDrr_hb(GBContext *cpu) {
    REG_H = REG_B;
    cpu->t_clock += 4;
}

static void LDrr_hc(GBContext *cpu) {
    REG_H = REG_C;
    cpu->t_clock += 4;
}

static void LDrr_hd(GBContext *cpu) {
    REG_H = REG_D;
    cpu->t_clock += 4;
}

static void LDrr_he(GBContext *cpu) {
    REG_H = REG_E;
    cpu->t_clock += 4;
}

static void LDrr_hh(GBContext *cpu) {
    REG_H = REG_H;
    cpu->t_clock += 4;
}

static void LDrr_hl(GBContext *cpu) {
    REG_H = REG_L;
    cpu->t_clock += 4;
}

static void LDrr_la(GBContext *cpu) {
    REG_L = REG_A;
    cpu->t_clock += 4;
}

static void LDrr_lb(GBContext *cpu) {
    REG_L = REG_B;
    cpu->t_clock += 4;
}

static void LDrr_lc(GBContext *cpu) {
    REG_L = REG_C;
    cpu->t_clock += 4;
}

static void LDrr_ld(GBContext *cpu) {
    REG_L = REG_D;
    cpu->t_clock += 4;
}

static void LDrr_le(GBContext *cpu) {
    REG_L = REG_E;
    cpu->t_clock += 4;
}

static void LDrr_lh(GBContext *cpu) {
    REG_L = REG_H;
    cpu->t_clock += 4;
}

static void LDrr_ll(GBContext *cpu) {
    REG_L = REG_L;
    cpu->t_clock += 4;
}

static void LDBCnn(GBContext *cpu) {
    REG_BC = read16(cpu, cpu->PC);
    cpu->PC += 2;
    cpu->t_clock += 12;
}

static void LDDEnn(GBContext *cpu) {
    REG_DE = read16(cpu, cpu->PC);
    cpu->PC += 2;
    cpu->t_clock += 12;
}

static void LDHLnn(GBContext *cpu) {
    REG_HL = read16(cpu, cpu->PC);
    cpu->PC += 2;
    cpu->t_clock += 12;
}

static void LDSPnn(GBContext *cpu) {
    cpu->SP = read16(cpu, cpu->PC);
    cpu->PC += 2;
    cpu->t_clock += 12;
}

static void LDSPHL(GBContext *cpu) {
    cpu->SP = REG_HL;
    cpu->t_clock += 12;
}

static void LDHLSPn(GBContext *cpu) {
    REG_HL = do_ADDSPe(cpu, (int8_t) read8(cpu, cpu->PC++));
    cpu->t_clock += 12;
}

static void LDmmSP(GBContext *cpu) {
    write16(cpu, read16(cpu, cpu->PC), cpu->SP);
    cpu->PC += 2;
    cpu->t_clock += 20;
}

static void NOP(GBContext *cpu) {
    cpu->t_clock += 4;
}

static void ORHL(GBContext *cpu) {
    do_logic(cpu, read8(cpu, REG_HL), OR);
    cpu->t_clock += 8;
}

static void ORn(GBContext *cpu) {
    do_logic(cpu, read8(cpu, cpu->PC++), OR);
    cpu->t_clock += 8;
}

static void ORr_a(GBContext *cpu) {
    do_logic(cpu, REG_A, OR);
    cpu->t_clock += 4;
}

static void ORr_b(GBContext *cpu) {
    do_logic(cpu, REG_B, OR);
    cpu->t_clock += 4;
}

static void ORr_c(GBContext *cpu) {
    do_logic(cpu, REG_C, OR);
    cpu->t_clock += 4;
}

static void ORr_d(GBContext *cpu) {
    do_logic(cpu, REG_D, OR);
    cpu->t_clock += 4;
}

static void ORr_e(GBContext *cpu) {
    do_logic(cpu, REG_E, OR);
    cpu->t_clock += 4;
}

static void ORr_h(GBContext *cpu) {
    do_logic(cpu, REG_H, OR);
    cpu->t_clock += 4;
}

static void ORr_l(GBContext *cpu) {
    do_logic(cpu, REG_L, OR);
    cpu->t_clock += 4;
}

static void PUSHAF(GBContext *cpu) {
    do_push(cpu, REG_AF);
    cpu->t_clock += 16;
}

static void PUSHBC(GBContext *cpu) {
    do_push(cpu, REG_BC);
    cpu->t_clock += 16;
}

static void PUSHDE(GBContext *cpu) {
    do_push(cpu, REG_DE);
    cpu->t_clock += 16;
}

static void PUSHHL(GBContext *cpu) {
    do_push(cpu, REG_HL);
    cpu->t_clock += 16;
}

static void POPAF(GBContext *cpu) {
    REG_AF = do_pop(cpu);
    cpu->t_clock += 12;
}

static void POPBC(GBContext *cpu) {
    REG_BC = do_pop(cpu);
    cpu->t_clock += 12;
}

static void POPDE(GBContext *cpu) {
    REG_DE = do_pop(cpu);
    cpu->t_clock += 12;
}

static void POPHL(GBContext *cpu) {
    REG_HL = do_pop(cpu);
    cpu->t_clock += 12;
}

static void RET(GBContext *cpu) {
    cpu->PC = do_pop(cpu);
    cpu->t_clock += 16;
}

static void RETI(GBContext *cpu) {
    cpu->PC = do_pop(cpu);
    cpu->IME = 1;
    cpu->t_clock += 16;
}

static void RETC(GBContext *cpu) {
    if (GETFLAG(F_C)) {
        cpu->PC = do_pop(cpu);
        cpu->t_clock += 20;
    } else {
        cpu->t_clock += 8;
    }
}

static void RETNC(GBContext *cpu) {
    if (!GETFLAG(F_C)) {
        cpu->PC = do_pop(cpu);
        cpu->t_clock += 20;
    } else {
        cpu->t_clock += 8;
    }
}

static void RETZ(GBContext *cpu) {
    if (GETFLAG(F_Z)) {
        cpu->PC = do_pop(cpu);
        cpu->t_clock += 20;
    } else {
        cpu->t_clock += 8;
    }
}

static void RETNZ(GBContext *cpu) {
    if (!GETFLAG(F_Z)) {
        cpu->PC = do_pop(cpu);
        cpu->t_clock += 20;
    } else {
        cpu->t_clock += 8;
    }
}

static void RLA(GBContext *cpu) {
    REG_A = do_rotate(cpu, REG_A, RL, USE_ACC);
    cpu->t_clock += 4;
}

static void RLCA(GBContext *cpu) {
    REG_A = do_rotate(cpu, REG_A, RLC, USE_ACC);
    cpu->t_clock += 4;
}

static void RRA(GBContext *cpu) {
    REG_A = do_rotate(cpu, REG_A, RR, USE_ACC);
    cpu->t_clock += 4;
}

static void RRCA(GBContext *cpu) {
    REG_A = do_rotate(cpu, REG_A, RRC, USE_ACC);
    cpu->t_clock += 4;
}

static void RST00(GBContext *cpu) {
    do_push(cpu, cpu->PC);
    cpu->PC = 0x0000;
    cpu->t_clock += 16;
}

static void RST08(GBContext *cpu) {
    do_push(cpu, cpu->PC);
    cpu->PC = 0x0008;
    cpu->t_clock += 16;
}

static void RST10(GBContext *cpu) {
    do_push(cpu, cpu->PC);
    cpu->PC = 0x0010;
    cpu->t_clock += 16;
}

static void RST18(GBContext *cpu) {
    do_push(cpu, cpu->PC);
    cpu->PC = 0x0018;
    cpu->t_clock += 16;
}

static void RST20(GBContext *cpu) {
    do_push(cpu, cpu->PC);
    cpu->PC = 0x0020;
    cpu->t_clock += 16;
}

static void RST28(GBContext *cpu) {
    do_push(cpu, cpu->PC);
    cpu->PC = 0x0028;
    cpu->t_clock += 16;
}

static void RST30(GBContext *cpu) {
    do_push(cpu, cpu->PC);
    cpu->PC = 0x0030;
    cpu->t_clock += 16;
}

static void RST38(GBContext *cpu) {
    do_push(cpu, cpu->PC);
    cpu->PC = 0x0038;
    cpu->t_clock += 16;
}

static void SBCHL(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, read8(cpu, REG_HL), SUB, CARRY);
    cpu->t_clock += 8;
}

static void SBCn(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, read8(cpu, cpu->PC++), SUB, CARRY);
    cpu->t_clock += 8;
}

static void SBCr_a(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_A, SUB, CARRY);
    cpu->t_clock += 4;
}

static void SBCr_b(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_B, SUB, CARRY);
    cpu->t_clock += 4;
}

static void SBCr_c(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_C, SUB, CARRY);
    cpu->t_clock += 4;
}

static void SBCr_d(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_D, SUB, CARRY);
    cpu->t_clock += 4;
}

static void SBCr_e(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_E, SUB, CARRY);
    cpu->t_clock += 4;
}

static void SBCr_h(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_H, SUB, CARRY);
    cpu->t_clock += 4;
}

static void SBCr_l(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_L, SUB, CARRY);
    cpu->t_clock += 4;
}

static void SCF(GBContext *cpu) {
    do_SCF(cpu);
    cpu->t_clock += 4;
}

static void SUBHL(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, read8(cpu, REG_HL), SUB, NO_CARRY);
    cpu->t_clock += 8;
}

static void SUBn(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, read8(cpu, cpu->PC++), SUB, NO_CARRY);
    cpu->t_clock += 8;
}

static void SUBr_a(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_A, SUB, NO_CARRY);
    cpu->t_clock += 4;
}

static void SUBr_b(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_B, SUB, NO_CARRY);
    cpu->t_clock += 4;
}

static void SUBr_c(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_C, SUB, NO_CARRY);
    cpu->t_clock += 4;
}

static void SUBr_d(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_D, SUB, NO_CARRY);
    cpu->t_clock += 4;
}

static void SUBr_e(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_E, SUB, NO_CARRY);
    cpu->t_clock += 4;
}

static void SUBr_h(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_H, SUB, NO_CARRY);
    cpu->t_clock += 4;
}

static void SUBr_l(GBContext *cpu) {
    REG_A = do_arithmetic8(cpu, REG_L, SUB, NO_CARRY);
    cpu->t_clock += 4;
}

static void XORHL(GBContext *cpu) {
    do_logic(cpu, read8(cpu, REG_HL), XOR);
    cpu->t_clock += 8;
}

static void XORn(GBContext *cpu) {
    do_logic(cpu, read8(cpu, cpu->PC++), XOR);
    cpu->t_clock += 8;
    ;
}

static void XORr_a(GBContext *cpu) {
    do_logic(cpu, REG_A, XOR);
    cpu->t_clock += 4;
}

static void XORr_b(GBContext *cpu) {
    do_logic(cpu, REG_B, XOR);
    cpu->t_clock += 4;
}

static void XORr_c(GBContext *cpu) {
    do_logic(cpu, REG_C, XOR);
    cpu->t_clock += 4;
}

static void XORr_d(GBContext *cpu) {
    do_logic(cpu, REG_D, XOR);
    cpu->t_clock += 4;
}

static void XORr_e(GBContext *cpu) {
    do_logic(cpu, REG_E, XOR);
    cpu->t_clock += 4;
}

static void XORr_h(GBContext *cpu) {
    do_logic(cpu, REG_H, XOR);
    cpu->t_clock += 4;
}

static void XORr_l(GBContext *cpu) {
    do_logic(cpu, REG_L, XOR);
    cpu->t_clock += 4;
}

static void DAA(GBContext *cpu) {
    printf("**** ERROR : %s not implemented. ****\n", __func__);
    exit(0);
}

static void RLCr_b(GBContext *cpu) {
    REG_B = do_rotate(cpu, REG_B, RLC, NO_ACC);
    cpu->t_clock += 8;
}

static void RLCr_c(GBContext *cpu) {
    REG_C = do_rotate(cpu, REG_C, RLC, NO_ACC);
    cpu->t_clock += 8;
}

static void RLCr_d(GBContext *cpu) {
    REG_D = do_rotate(cpu, REG_D, RLC, NO_ACC);
    cpu->t_clock += 8;
}

static void RLCr_e(GBContext *cpu) {
    REG_E = do_rotate(cpu, REG_E, RLC, NO_ACC);
    cpu->t_clock += 8;
}

static void RLCr_h(GBContext *cpu) {
    REG_H = do_rotate(cpu, REG_H, RLC, NO_ACC);
    cpu->t_clock += 8;
}

static void RLCr_l(GBContext *cpu) {
    REG_L = do_rotate(cpu, REG_L, RLC, NO_ACC);
    cpu->t_clock += 8;
}

static void RLCHL(GBContext *cpu) {
    write8(cpu, REG_HL, do_rotate(cpu, read8(cpu, REG_HL), RLC, NO_ACC));
    cpu->t_clock += 16;
}

static void RLCr_a(GBContext *cpu) {
    REG_A = do_rotate(cpu, REG_A, RLC, NO_ACC);
    cpu->t_clock += 8;
}

static void RRCr_b(GBContext *cpu) {
    REG_B = do_rotate(cpu, REG_B, RRC, NO_ACC);
    cpu->t_clock += 8;
}

static void RRCr_c(GBContext *cpu) {
    REG_C = do_rotate(cpu, REG_C, RRC, NO_ACC);
    cpu->t_clock += 8;
}

static void RRCr_d(GBContext *cpu) {
    REG_D = do_rotate(cpu, REG_D, RRC, NO_ACC);
    cpu->t_clock += 8;
}

static void RRCr_e(GBContext *cpu) {
    REG_E = do_rotate(cpu, REG_E, RRC, NO_ACC);
    cpu->t_clock += 8;
}

static void RRCr_h(GBContext *cpu) {
    REG_H = do_rotate(cpu, REG_H, RRC, NO_ACC);
    cpu->t_clock += 8;
}

static void RRCr_l(GBContext *cpu) {
    REG_L = do_rotate(cpu, REG_L, RRC, NO_ACC);
    cpu->t_clock += 8;
}

static void RRCHL(GBContext *cpu) {
    write8(cpu, REG_HL, do_rotate(cpu, read8(cpu, REG_HL), RRC, NO_ACC));
    cpu->t_clock += 16;
}

static void RRCr_a(GBContext *cpu) {
    REG_A = do_rotate(cpu, REG_A, RRC, NO_ACC);
    cpu->t_clock += 8;
}

static void RLr_b(GBContext *cpu) {
    REG_B = do_rotate(cpu, REG_B, RL, NO_ACC);
    cpu->t_clock += 8;
}

static void RLr_c(GBContext *cpu) {
    REG_C = do_rotate(cpu, REG_C, RL, NO_ACC);
    cpu->t_clock += 8;
}

static void RLr_d(GBContext *cpu) {
    REG_D = do_rotate(cpu, REG_D, RL, NO_ACC);
    cpu->t_clock += 8;
}

static void RLr_e(GBContext *cpu) {
    REG_E = do_rotate(cpu, REG_E, RL, NO_ACC);
    cpu->t_clock += 8;
}

static void RLr_h(GBContext *cpu) {
    REG_H = do_rotate(cpu, REG_H, RL, NO_ACC);
    cpu->t_clock += 8;
}

static void RLr_l(GBContext *cpu) {
    REG_L = do_rotate(cpu, REG_L, RL, NO_ACC);
    cpu->t_clock += 8;
}

static void RLHL(GBContext *cpu) {
    write8(cpu, REG_HL, do_rotate(cpu, read8(cpu, REG_HL), RL, NO_ACC));
    cpu->t_clock += 16;
}

static void RLr_a(GBContext *cpu) {
    REG_A = do_rotate(cpu, REG_A, RL, NO_ACC);
    cpu->t_clock += 8;
}

static void RRr_b(GBContext *cpu) {
    REG_B = do_rotate(cpu, REG_B, RR, NO_ACC);
    cpu->t_clock += 8;
}

static void RRr_c(GBContext *cpu) {
    REG_C = do_rotate(cpu, REG_C, RR, NO_ACC);
    cpu->t_clock += 8;
}

static void RRr_d(GBContext *cpu) {
    REG_D = do_rotate(cpu, REG_D, RR, NO_ACC);
    cpu->t_clock += 8;
}

static void RRr_e(GBContext *cpu) {
    REG_E = do_rotate(cpu, REG_E, RR, NO_ACC);
    cpu->t_clock += 8;
    ;
}

static void RRr_h(GBContext *cpu) {
    REG_H = do_rotate(cpu, REG_H, RR, NO_ACC);
    cpu->t_clock += 8;
}

static void RRr_l(GBContext *cpu) {
    REG_L = do_rotate(cpu, REG_L, RR, NO_ACC);
    cpu->t_clock += 8;
}

static void RRHL(GBContext *cpu) {
    write8(cpu, REG_HL, do_rotate(cpu, read8(cpu, REG_HL), RR, NO_ACC));
    cpu->t_clock += 16;
}

static void RRr_a(GBContext *cpu) {
    REG_A = do_rotate(cpu, REG_A, RR, NO_ACC);
    cpu->t_clock += 8;
}

static void SLAHL(GBContext *cpu) {
    write8(cpu, REG_HL, do_shift(cpu, read8(cpu, REG_HL), SLA));
    cpu->t_clock += 16;
}

static void SLAr_a(GBContext *cpu) {
    REG_A = do_shift(cpu, REG_A, SLA);
    cpu->t_clock += 8;
}

static void SLAr_b(GBContext *cpu) {
    REG_B = do_shift(cpu, REG_B, SLA);
    cpu->t_clock += 8;
}

static void SLAr_c(GBContext *cpu) {
    REG_C = do_shift(cpu, REG_C, SLA);
    cpu->t_clock += 8;
}

static void SLAr_d(GBContext *cpu) {
    REG_D = do_shift(cpu, REG_D, SLA);
    cpu->t_clock += 8;
}

static void SLAr_e(GBContext *cpu) {
    REG_E = do_shift(cpu, REG_E, SLA);
    cpu->t_clock += 8;
}

static void SLAr_h(GBContext *cpu) {
    REG_H = do_shift(cpu, REG_H, SLA);
    cpu->t_clock += 8;
}

static void SLAr_l(GBContext *cpu) {
    REG_L = do_shift(cpu, REG_L, SLA);
    cpu->t_clock += 8;
}

static void SRAHL(GBContext *cpu) {
    write8(cpu, REG_HL, do_shift(cpu, read8(cpu, REG_HL), SRA));
    cpu->t_clock += 16;
}

static void SRAr_a(GBContext *cpu) {
    REG_A = do_shift(cpu, REG_A, SRA);
    cpu->t_clock += 8;
}

static void SRAr_b(GBContext *cpu) {
    REG_B = do_shift(cpu, REG_B, SRA);
    cpu->t_clock += 8;
}

static void SRAr_c(GBContext *cpu) {
    REG_C = do_shift(cpu, REG_C, SRA);
    cpu->t_clock += 8;
}

static void SRAr_d(GBContext *cpu) {
    REG_D = do_shift(cpu, REG_D, SRA);
    cpu->t_clock += 8;
}

static void SRAr_e(GBContext *cpu) {
    REG_E = do_shift(cpu, REG_E, SRA);
    cpu->t_clock += 8;
}

static void SRAr_h(GBContext *cpu) {
    REG_H = do_shift(cpu, REG_H, SRA);
    cpu->t_clock += 8;
}

static void SRAr_l(GBContext *cpu) {
    REG_L = do_shift(cpu, REG_L, SRA);
    cpu->t_clock += 8;
}

static void SWAPHL(GBContext *cpu) {
    write8(cpu, REG_HL, do_SWAP(cpu, read8(cpu, REG_HL)));
    cpu->t_clock += 16;
}

static void SWAPr_a(GBContext *cpu) {
    REG_A = do_SWAP(cpu, REG_A);
    cpu->t_clock += 8;
}

static void SWAPr_b(GBContext *cpu) {
    REG_B = do_SWAP(cpu, REG_B);
    cpu->t_clock += 8;
}

static void SWAPr_c(GBContext *cpu) {
    REG_C = do_SWAP(cpu, REG_C);
    cpu->t_clock += 8;
}

static void SWAPr_d(GBContext *cpu) {
    REG_D = do_SWAP(cpu, REG_D);
    cpu->t_clock += 8;
}

static void SWAPr_e(GBContext *cpu) {
    REG_E = do_SWAP(cpu, REG_E);
    cpu->t_clock += 8;
}

static void SWAPr_h(GBContext *cpu) {
    REG_H = do_SWAP(cpu, REG_H);
    cpu->t_clock += 8;
}

static void SWAPr_l(GBContext *cpu) {
    REG_L = do_SWAP(cpu, REG_L);
    cpu->t_clock += 8;
}

static void SRLr_a(GBContext *cpu) {
    REG_A = do_shift(cpu, REG_A, SRL);
    cpu->t_clock += 8;
}

static void SRLHL(GBContext *cpu) {
    write8(cpu, REG_HL, do_shift(cpu, read8(cpu, REG_HL), SRL));
    cpu->t_clock += 16;
}

static void SRLr_b(GBContext *cpu) {
    REG_B = do_shift(cpu, REG_B, SRL);
    cpu->t_clock += 8;
}

static void SRLr_c(GBContext *cpu) {
    REG_C = do_shift(cpu, REG_C, SRL);
    cpu->t_clock += 8;
}

static void SRLr_d(GBContext *cpu) {
    REG_D = do_shift(cpu, REG_D, SRL);
    cpu->t_clock += 8;
}

static void SRLr_e(GBContext *cpu) {
    REG_E = do_shift(cpu, REG_E, SRL);
    cpu->t_clock += 8;
}

static void SRLr_h(GBContext *cpu) {
    REG_H = do_shift(cpu, REG_H, SRL);
    cpu->t_clock += 8;
}

static void SRLr_l(GBContext *cpu) {
    REG_L = do_shift(cpu, REG_L, SRL);
    cpu->t_clock += 8;
}

static void BIT0b(GBContext *cpu) {
    do_bit(cpu, BIT0, REG_B);
    cpu->t_clock += 8;
}

static void BIT0c(GBContext *cpu) {
    do_bit(cpu, BIT0, REG_C);
    cpu->t_clock += 8;
}

static void BIT0d(GBContext *cpu) {
    do_bit(cpu, BIT0, REG_D);
    cpu->t_clock += 8;
}

static void BIT0e(GBContext *cpu) {
    do_bit(cpu, BIT0, REG_E);
    cpu->t_clock += 8;
}

static void BIT0h(GBContext *cpu) {
    do_bit(cpu, BIT0, REG_H);
    cpu->t_clock += 8;
}

static void BIT0l(GBContext *cpu) {
    do_bit(cpu, BIT0, REG_L);
    cpu->t_clock += 8;
}

static void BIT0m(GBContext *cpu) {
    do_bit(cpu, BIT0, read8(cpu, REG_HL));
    cpu->t_clock += 12;
}

static void BIT0a(GBContext *cpu) {
    do_bit(cpu, BIT0, REG_A);
    cpu->t_clock += 8;
}

static void BIT1b(GBContext *cpu) {
    do_bit(cpu, BIT1, REG_B);
    cpu->t_clock += 8;
}

static void BIT1c(GBContext *cpu) {
    do_bit(cpu, BIT1, REG_C);
    cpu->t_clock += 8;
}

static void BIT1d(GBContext *cpu) {
    do_bit(cpu, BIT1, REG_D);
    cpu->t_clock += 8;
}

static void BIT1e(GBContext *cpu) {
    do_bit(cpu, BIT1, REG_E);
    cpu->t_clock += 8;
}

static void BIT1h(GBContext *cpu) {
    do_bit(cpu, BIT1, REG_H);
    cpu->t_clock += 8;
}

static void BIT1l(GBContext *cpu) {
    do_bit(cpu, BIT1, REG_L);
    cpu->t_clock += 8;
}

static void BIT1m(GBContext *cpu) {
    do_bit(cpu, BIT1, read8(cpu, REG_HL));
    cpu->t_clock += 12;
}

static void BIT1a(GBContext *cpu) {
    do_bit(cpu, BIT1, REG_A);
    cpu->t_clock += 8;
}

static void BIT2b(GBContext *cpu) {
    do_bit(cpu, BIT2, REG_B);
    cpu->t_clock += 8;
}

static void BIT2c(GBContext *cpu) {
    do_bit(cpu, BIT2, REG_C);
    cpu->t_clock += 8;
}

static void BIT2d(GBContext *cpu) {
    do_bit(cpu, BIT2, REG_D);
    cpu->t_clock += 8;
}

static void BIT2e(GBContext *cpu) {
    do_bit(cpu, BIT2, REG_E);
    cpu->t_clock += 8;
}

static void BIT2h(GBContext *cpu) {
    do_bit(cpu, BIT2, REG_H);
    cpu->t_clock += 8;
}

static void BIT2l(GBContext *cpu) {
    do_bit(cpu, BIT2, REG_L);
    cpu->t_clock += 8;
}

static void BIT2m(GBContext *cpu) {
    do_bit(cpu, BIT2, read8(cpu, REG_HL));
    cpu->t_clock += 12;
}

static void BIT2a(GBContext *cpu) {
    do_bit(cpu, BIT2, REG_A);
    cpu->t_clock += 8;
}

static void BIT3b(GBContext *cpu) {
    do_bit(cpu, BIT3, REG_B);
    cpu->t_clock += 8;
}

static void BIT3c(GBContext *cpu) {
    do_bit(cpu, BIT3, REG_C);
    cpu->t_clock += 8;
}

static void BIT3d(GBContext *cpu) {
    do_bit(cpu, BIT3, REG_D);
    cpu->t_clock += 8;
}

static void BIT3e(GBContext *cpu) {
    do_bit(cpu, BIT3, REG_E);
    cpu->t_clock += 8;
}

static void BIT3h(GBContext *cpu) {
    do_bit(cpu, BIT3, REG_H);
    cpu->t_clock += 8;
}

static void BIT3l(GBContext *cpu) {
    do_bit(cpu, BIT3, REG_L);
    cpu->t_clock += 8;
}

static void BIT3m(GBContext *cpu) {
    do_bit(cpu, BIT3, read8(cpu, REG_HL));
    cpu->t_clock += 12;
}

static void BIT3a(GBContext *cpu) {
    do_bit(cpu, BIT3, REG_A);
    cpu->t_clock += 8;
}

static void BIT4b(GBContext *cpu) {
    do_bit(cpu, BIT4, REG_B);
    cpu->t_clock += 8;
}

static void BIT4c(GBContext *cpu) {
    do_bit(cpu, BIT4, REG_C);
    cpu->t_clock += 8;
}

static void BIT4d(GBContext *cpu) {
    do_bit(cpu, BIT4, REG_D);
    cpu->t_clock += 8;
}

static void BIT4e(GBContext *cpu) {
    do_bit(cpu, BIT4, REG_E);
    cpu->t_clock += 8;
}

static void BIT4h(GBContext *cpu) {
    do_bit(cpu, BIT4, REG_H);
    cpu->t_clock += 8;
}

static void BIT4l(GBContext *cpu) {
    do_bit(cpu, BIT4, REG_L);
    cpu->t_clock += 8;
}

static void BIT4m(GBContext *cpu) {
    do_bit(cpu, BIT4, read8(cpu, REG_HL));
    cpu->t_clock += 12;
}

static void BIT4a(GBContext *cpu) {
    do_bit(cpu, BIT4, REG_A);
    cpu->t_clock += 8;
}

static void BIT5b(GBContext *cpu) {
    do_bit(cpu, BIT5, REG_B);
    cpu->t_clock += 8;
}

static void BIT5c(GBContext *cpu) {
    do_bit(cpu, BIT5, REG_C);
    cpu->t_clock += 8;
}

static void BIT5d(GBContext *cpu) {
    do_bit(cpu, BIT5, REG_D);
    cpu->t_clock += 8;
}

static void BIT5e(GBContext *cpu) {
    do_bit(cpu, BIT5, REG_E);
    cpu->t_clock += 8;
}

static void BIT5h(GBContext *cpu) {
    do_bit(cpu, BIT5, REG_H);
    cpu->t_clock += 8;
}

static void BIT5l(GBContext *cpu) {
    do_bit(cpu, BIT5, REG_L);
    cpu->t_clock += 8;
}

static void BIT5m(GBContext *cpu) {
    do_bit(cpu, BIT5, read8(cpu, REG_HL));
    cpu->t_clock += 12;
}

static void BIT5a(GBContext *cpu) {
    do_bit(cpu, BIT5, REG_A);
    cpu->t_clock += 8;
}

static void BIT6b(GBContext *cpu) {
    do_bit(cpu, BIT6, REG_B);
    cpu->t_clock += 8;
}

static void BIT6c(GBContext *cpu) {
    do_bit(cpu, BIT6, REG_C);
    cpu->t_clock += 8;
}

static void BIT6d(GBContext *cpu) {
    do_bit(cpu, BIT6, REG_D);
    cpu->t_clock += 8;
}

static void BIT6e(GBContext *cpu) {
    do_bit(cpu, BIT6, REG_E);
    cpu->t_clock += 8;
}

static void BIT6h(GBContext *cpu) {
    do_bit(cpu, BIT6, REG_H);
    cpu->t_clock += 8;
}

static void BIT6l(GBContext *cpu) {
    do_bit(cpu, BIT6, REG_L);
    cpu->t_clock += 8;
}

static void BIT6m(GBContext *cpu) {
    do_bit(cpu, BIT6, read8(cpu, REG_HL));
    cpu->t_clock += 12;
}

static void BIT6a(GBContext *cpu) {
    do_bit(cpu, BIT6, REG_A);
    cpu->t_clock += 8;
}

static void BIT7b(GBContext *cpu) {
    do_bit(cpu, BIT7, REG_B);
    cpu->t_clock += 8;
}

static void BIT7c(GBContext *cpu) {
    do_bit(cpu, BIT7, REG_C);
    cpu->t_clock += 8;
}

static void BIT7d(GBContext *cpu) {
    do_bit(cpu, BIT7, REG_D);
    cpu->t_clock += 8;
}

static void BIT7e(GBContext *cpu) {
    do_bit(cpu, BIT7, REG_E);
    cpu->t_clock += 8;
}

static void BIT7h(GBContext *cpu) {
    do_bit(cpu, BIT7, REG_H);
    cpu->t_clock += 8;
}

static void BIT7l(GBContext *cpu) {
    do_bit(cpu, BIT7, REG_L);
    cpu->t_clock += 8;
}

static void BIT7m(GBContext *cpu) {
    do_bit(cpu, BIT7, read8(cpu, REG_HL));
    cpu->t_clock += 12;
}

static void BIT7a(GBContext *cpu) {
    do_bit(cpu, BIT7, REG_A);
    cpu->t_clock += 8;
}

static void RES0b(GBContext *cpu) {
    do_res(cpu, BIT0, REG_B);
    cpu->t_clock += 8;
}

static void RES0c(GBContext *cpu) {
    do_res(cpu, BIT0, REG_C);
    cpu->t_clock += 8;
}

static void RES0d(GBContext *cpu) {
    do_res(cpu, BIT0, REG_D);
    cpu->t_clock += 8;
}

static void RES0e(GBContext *cpu) {
    do_res(cpu, BIT0, REG_E);
    cpu->t_clock += 8;
}

static void RES0h(GBContext *cpu) {
    do_res(cpu, BIT0, REG_H);
    cpu->t_clock += 8;
}

static void RES0l(GBContext *cpu) {
    do_res(cpu, BIT0, REG_L);
    cpu->t_clock += 8;
}

static void RES0m(GBContext *cpu) {
    do_res(cpu, BIT0, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void RES0a(GBContext *cpu) {
    do_res(cpu, BIT0, REG_A);
    cpu->t_clock += 8;
}

static void RES1b(GBContext *cpu) {
    do_res(cpu, BIT1, REG_B);
    cpu->t_clock += 8;
}

static void RES1c(GBContext *cpu) {
    do_res(cpu, BIT1, REG_C);
    cpu->t_clock += 8;
}

static void RES1d(GBContext *cpu) {
    do_res(cpu, BIT1, REG_D);
    cpu->t_clock += 8;
}

static void RES1e(GBContext *cpu) {
    do_res(cpu, BIT1, REG_E);
    cpu->t_clock += 8;
}

static void RES1h(GBContext *cpu) {
    do_res(cpu, BIT1, REG_H);
    cpu->t_clock += 8;
}

static void RES1l(GBContext *cpu) {
    do_res(cpu, BIT1, REG_L);
    cpu->t_clock += 8;
}

static void RES1m(GBContext *cpu) {
    do_res(cpu, BIT1, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void RES1a(GBContext *cpu) {
    do_res(cpu, BIT1, REG_A);
    cpu->t_clock += 8;
}

static void RES2b(GBContext *cpu) {
    do_res(cpu, BIT2, REG_B);
    cpu->t_clock += 8;
}

static void RES2c(GBContext *cpu) {
    do_res(cpu, BIT2, REG_C);
    cpu->t_clock += 8;
}

static void RES2d(GBContext *cpu) {
    do_res(cpu, BIT2, REG_D);
    cpu->t_clock += 8;
}

static void RES2e(GBContext *cpu) {
    do_res(cpu, BIT2, REG_E);
    cpu->t_clock += 8;
}

static void RES2h(GBContext *cpu) {
    do_res(cpu, BIT2, REG_H);
    cpu->t_clock += 8;
}

static void RES2l(GBContext *cpu) {
    do_res(cpu, BIT2, REG_L);
    cpu->t_clock += 8;
}

static void RES2m(GBContext *cpu) {
    do_res(cpu, BIT2, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void RES2a(GBContext *cpu) {
    do_res(cpu, BIT2, REG_A);
    cpu->t_clock += 8;
}

static void RES3b(GBContext *cpu) {
    do_res(cpu, BIT3, REG_B);
    cpu->t_clock += 8;
}

static void RES3c(GBContext *cpu) {
    do_res(cpu, BIT3, REG_C);
    cpu->t_clock += 8;
}

static void RES3d(GBContext *cpu) {
    do_res(cpu, BIT3, REG_D);
    cpu->t_clock += 8;
}

static void RES3e(GBContext *cpu) {
    do_res(cpu, BIT3, REG_E);
    cpu->t_clock += 8;
}

static void RES3h(GBContext *cpu) {
    do_res(cpu, BIT3, REG_H);
    cpu->t_clock += 8;
}

static void RES3l(GBContext *cpu) {
    do_res(cpu, BIT3, REG_L);
    cpu->t_clock += 8;
}

static void RES3m(GBContext *cpu) {
    do_res(cpu, BIT3, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void RES3a(GBContext *cpu) {
    do_res(cpu, BIT3, REG_A);
    cpu->t_clock += 8;
}

static void RES4b(GBContext *cpu) {
    do_res(cpu, BIT4, REG_B);
    cpu->t_clock += 8;
}

static void RES4c(GBContext *cpu) {
    do_res(cpu, BIT4, REG_C);
    cpu->t_clock += 8;
}

static void RES4d(GBContext *cpu) {
    do_res(cpu, BIT4, REG_D);
    cpu->t_clock += 8;
}

static void RES4e(GBContext *cpu) {
    do_res(cpu, BIT4, REG_E);
    cpu->t_clock += 8;
}

static void RES4h(GBContext *cpu) {
    do_res(cpu, BIT4, REG_H);
    cpu->t_clock += 8;
}

static void RES4l(GBContext *cpu) {
    do_res(cpu, BIT4, REG_L);
    cpu->t_clock += 8;
}

static void RES4m(GBContext *cpu) {
    do_res(cpu, BIT4, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void RES4a(GBContext *cpu) {
    do_res(cpu, BIT4, REG_A);
    cpu->t_clock += 8;
}

static void RES5b(GBContext *cpu) {
    do_res(cpu, BIT5, REG_B);
    cpu->t_clock += 8;
}

static void RES5c(GBContext *cpu) {
    do_res(cpu, BIT5, REG_C);
    cpu->t_clock += 8;
}

static void RES5d(GBContext *cpu) {
    do_res(cpu, BIT5, REG_D);
    cpu->t_clock += 8;
}

static void RES5e(GBContext *cpu) {
    do_res(cpu, BIT5, REG_E);
    cpu->t_clock += 8;
}

static void RES5h(GBContext *cpu) {
    do_res(cpu, BIT5, REG_H);
    cpu->t_clock += 8;
}

static void RES5l(GBContext *cpu) {
    do_res(cpu, BIT5, REG_L);
    cpu->t_clock += 8;
}

static void RES5m(GBContext *cpu) {
    do_res(cpu, BIT5, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void RES5a(GBContext *cpu) {
    do_res(cpu, BIT5, REG_A);
    cpu->t_clock += 8;
}

static void RES6b(GBContext *cpu) {
    do_res(cpu, BIT6, REG_B);
    cpu->t_clock += 8;
}

static void RES6c(GBContext *cpu) {
    do_res(cpu, BIT6, REG_C);
    cpu->t_clock += 8;
}

static void RES6d(GBContext *cpu) {
    do_res(cpu, BIT6, REG_D);
    cpu->t_clock += 8;
}

static void RES6e(GBContext *cpu) {
    do_res(cpu, BIT6, REG_E);
    cpu->t_clock += 8;
}

static void RES6h(GBContext *cpu) {
    do_res(cpu, BIT6, REG_H);
    cpu->t_clock += 8;
}

static void RES6l(GBContext *cpu) {
    do_res(cpu, BIT6, REG_L);
    cpu->t_clock += 8;
}

static void RES6m(GBContext *cpu) {
    do_res(cpu, BIT6, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void RES6a(GBContext *cpu) {
    do_res(cpu, BIT6, REG_A);
    cpu->t_clock += 8;
}

static void RES7b(GBContext *cpu) {
    do_res(cpu, BIT7, REG_B);
    cpu->t_clock += 8;
}

static void RES7c(GBContext *cpu) {
    do_res(cpu, BIT7, REG_C);
    cpu->t_clock += 8;
}

static void RES7d(GBContext *cpu) {
    do_res(cpu, BIT7, REG_D);
    cpu->t_clock += 8;
}

static void RES7e(GBContext *cpu) {
    do_res(cpu, BIT7, REG_E);
    cpu->t_clock += 8;
}

static void RES7h(GBContext *cpu) {
    do_res(cpu, BIT7, REG_H);
    cpu->t_clock += 8;
}

static void RES7l(GBContext *cpu) {
    do_res(cpu, BIT7, REG_L);
    cpu->t_clock += 8;
}

static void RES7m(GBContext *cpu) {
    do_res(cpu, BIT7, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void RES7a(GBContext *cpu) {
    do_res(cpu, BIT7, REG_A);
    cpu->t_clock += 8;
}

static void SET0b(GBContext *cpu) {
    do_set(cpu, BIT0, REG_B);
    cpu->t_clock += 8;
}

static void SET0c(GBContext *cpu) {
    do_set(cpu, BIT0, REG_C);
    cpu->t_clock += 8;
}

static void SET0d(GBContext *cpu) {
    do_set(cpu, BIT0, REG_D);
    cpu->t_clock += 8;
}

static void SET0e(GBContext *cpu) {
    do_set(cpu, BIT0, REG_E);
    cpu->t_clock += 8;
}

static void SET0h(GBContext *cpu) {
    do_set(cpu, BIT0, REG_H);
    cpu->t_clock += 8;
}

static void SET0l(GBContext *cpu) {
    do_set(cpu, BIT0, REG_L);
    cpu->t_clock += 8;
}

static void SET0m(GBContext *cpu) {
    do_set(cpu, BIT0, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void SET0a(GBContext *cpu) {
    do_set(cpu, BIT0, REG_A);
    cpu->t_clock += 8;
}

static void SET1b(GBContext *cpu) {
    do_set(cpu, BIT1, REG_B);
    cpu->t_clock += 8;
}

static void SET1c(GBContext *cpu) {
    do_set(cpu, BIT1, REG_C);
    cpu->t_clock += 8;
}

static void SET1d(GBContext *cpu) {
    do_set(cpu, BIT1, REG_D);
    cpu->t_clock += 8;
}

static void SET1e(GBContext *cpu) {
    do_set(cpu, BIT1, REG_E);
    cpu->t_clock += 8;
}

static void SET1h(GBContext *cpu) {
    do_set(cpu, BIT1, REG_H);
    cpu->t_clock += 8;
}

static void SET1l(GBContext *cpu) {
    do_set(cpu, BIT1, REG_L);
    cpu->t_clock += 8;
}

static void SET1m(GBContext *cpu) {
    do_set(cpu, BIT1, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void SET1a(GBContext *cpu) {
    do_set(cpu, BIT1, REG_A);
    cpu->t_clock += 8;
}

static void SET2b(GBContext *cpu) {
    do_set(cpu, BIT2, REG_B);
    cpu->t_clock += 8;
}

static void SET2c(GBContext *cpu) {
    do_set(cpu, BIT2, REG_C);
    cpu->t_clock += 8;
}

static void SET2d(GBContext *cpu) {
    do_set(cpu, BIT2, REG_D);
    cpu->t_clock += 8;
}

static void SET2e(GBContext *cpu) {
    do_set(cpu, BIT2, REG_E);
    cpu->t_clock += 8;
}

static void SET2h(GBContext *cpu) {
    do_set(cpu, BIT2, REG_H);
    cpu->t_clock += 8;
}

static void SET2l(GBContext *cpu) {
    do_set(cpu, BIT2, REG_L);
    cpu->t_clock += 8;
}

static void SET2m(GBContext *cpu) {
    do_set(cpu, BIT2, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void SET2a(GBContext *cpu) {
    do_set(cpu, BIT2, REG_A);
    cpu->t_clock += 8;
}

static void SET3b(GBContext *cpu) {
    do_set(cpu, BIT3, REG_B);
    cpu->t_clock += 8;
}

static void SET3c(GBContext *cpu) {
    do_set(cpu, BIT3, REG_C);
    cpu->t_clock += 8;
}

static void SET3d(GBContext *cpu) {
    do_set(cpu, BIT3, REG_D);
    cpu->t_clock += 8;
}

static void SET3e(GBContext *cpu) {
    do_set(cpu, BIT3, REG_E);
    cpu->t_clock += 8;
}

static void SET3h(GBContext *cpu) {
    do_set(cpu, BIT3, REG_H);
    cpu->t_clock += 8;
}

static void SET3l(GBContext *cpu) {
    do_set(cpu, BIT3, REG_L);
    cpu->t_clock += 8;
}

static void SET3m(GBContext *cpu) {
    do_set(cpu, BIT3, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void SET3a(GBContext *cpu) {
    do_set(cpu, BIT3, REG_A);
    cpu->t_clock += 8;
}

static void SET4b(GBContext *cpu) {
    do_set(cpu, BIT4, REG_B);
    cpu->t_clock += 8;
}

static void SET4c(GBContext *cpu) {
    do_set(cpu, BIT4, REG_C);
    cpu->t_clock += 8;
}

static void SET4d(GBContext *cpu) {
    do_set(cpu, BIT4, REG_D);
    cpu->t_clock += 8;
}

static void SET4e(GBContext *cpu) {
    do_set(cpu, BIT4, REG_E);
    cpu->t_clock += 8;
}

static void SET4h(GBContext *cpu) {
    do_set(cpu, BIT4, REG_H);
    cpu->t_clock += 8;
}

static void SET4l(GBContext *cpu) {
    do_set(cpu, BIT4, REG_L);
    cpu->t_clock += 8;
}

static void SET4m(GBContext *cpu) {
    do_set(cpu, BIT4, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void SET4a(GBContext *cpu) {
    do_set(cpu, BIT4, REG_A);
    cpu->t_clock += 8;
}

static void SET5b(GBContext *cpu) {
    do_set(cpu, BIT5, REG_B);
    cpu->t_clock += 8;
}

static void SET5c(GBContext *cpu) {
    do_set(cpu, BIT5, REG_C);
    cpu->t_clock += 8;
}

static void SET5d(GBContext *cpu) {
    do_set(cpu, BIT5, REG_D);
    cpu->t_clock += 8;
}

static void SET5e(GBContext *cpu) {
    do_set(cpu, BIT5, REG_E);
    cpu->t_clock += 8;
}

static void SET5h(GBContext *cpu) {
    do_set(cpu, BIT5, REG_H);
    cpu->t_clock += 8;
}

static void SET5l(GBContext *cpu) {
    do_set(cpu, BIT5, REG_L);
    cpu->t_clock += 8;
}

static void SET5m(GBContext *cpu) {
    do_set(cpu, BIT5, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void SET5a(GBContext *cpu) {
    do_set(cpu, BIT5, REG_A);
    cpu->t_clock += 8;
}

static void SET6b(GBContext *cpu) {
    do_set(cpu, BIT6, REG_B);
    cpu->t_clock += 8;
}

static void SET6c(GBContext *cpu) {
    do_set(cpu, BIT6, REG_C);
    cpu->t_clock += 8;
}

static void SET6d(GBContext *cpu) {
    do_set(cpu, BIT6, REG_D);
    cpu->t_clock += 8;
}

static void SET6e(GBContext *cpu) {
    do_set(cpu, BIT6, REG_E);
    cpu->t_clock += 8;
}

static void SET6h(GBContext *cpu) {
    do_set(cpu, BIT6, REG_H);
    cpu->t_clock += 8;
}

static void SET6l(GBContext *cpu) {
    do_set(cpu, BIT6, REG_L);
    cpu->t_clock += 8;
}

static void SET6m(GBContext *cpu) {
    do_set(cpu, BIT6, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void SET6a(GBContext *cpu) {
    do_set(cpu, BIT6, REG_A);
    cpu->t_clock += 8;
}

static void SET7b(GBContext *cpu) {
    do_set(cpu, BIT7, REG_B);
    cpu->t_clock += 8;
}

static void SET7c(GBContext *cpu) {
    do_set(cpu, BIT7, REG_C);
    cpu->t_clock += 8;
}

static void SET7d(GBContext *cpu) {
    do_set(cpu, BIT7, REG_D);
    cpu->t_clock += 8;
}

static void SET7e(GBContext *cpu) {
    do_set(cpu, BIT7, REG_E);
    cpu->t_clock += 8;
}

static void SET7h(GBContext *cpu) {
    do_set(cpu, BIT7, REG_H);
    cpu->t_clock += 8;
}

static void SET7l(GBContext *cpu) {
    do_set(cpu, BIT7, REG_L);
    cpu->t_clock += 8;
}

static void SET7m(GBContext *cpu) {
    do_set(cpu, BIT7, read8(cpu, REG_HL));
    cpu->t_clock += 16;
}

static void SET7a(GBContext *cpu) {
    do_set(cpu, BIT7, REG_A);
    cpu->t_clock += 8;
}

static void _UNDEFINED(GBContext *cpu) {
    printf("illegal instruction\n");
    dump_ctx(cpu);
    dump_memory(cpu, cpu->PC - 0x10, cpu->PC + 0x100);
    exit(1);
}

static void (*_cb[]) (GBContext *cpu) = {
    RLCr_b,
    RLCr_c,
    RLCr_d,
    RLCr_e,
    RLCr_h,
    RLCr_l,
    RLCHL,
    RLCr_a,
    RRCr_b,
    RRCr_c,
    RRCr_d,
    RRCr_e,
    RRCr_h,
    RRCr_l,
    RRCHL,
    RRCr_a,
    RLr_b,
    RLr_c,
    RLr_d,
    RLr_e,
    RLr_h,
    RLr_l,
    RLHL,
    RLr_a,
    RRr_b,
    RRr_c,
    RRr_d,
    RRr_e,
    RRr_h,
    RRr_l,
    RRHL,
    RRr_a,
    SLAr_b,
    SLAr_c,
    SLAr_d,
    SLAr_e,
    SLAr_h,
    SLAr_l,
    SLAHL,
    SLAr_a,
    SRAr_b,
    SRAr_c,
    SRAr_d,
    SRAr_e,
    SRAr_h,
    SRAr_l,
    SRAHL,
    SRAr_a,
    SWAPr_b,
    SWAPr_c,
    SWAPr_d,
    SWAPr_e,
    SWAPr_h,
    SWAPr_l,
    SWAPHL,
    SWAPr_a,
    SRLr_b,
    SRLr_c,
    SRLr_d,
    SRLr_e,
    SRLr_h,
    SRLr_l,
    SRLHL,
    SRLr_a,
    BIT0b,
    BIT0c,
    BIT0d,
    BIT0e,
    BIT0h,
    BIT0l,
    BIT0m,
    BIT0a,
    BIT1b,
    BIT1c,
    BIT1d,
    BIT1e,
    BIT1h,
    BIT1l,
    BIT1m,
    BIT1a,
    BIT2b,
    BIT2c,
    BIT2d,
    BIT2e,
    BIT2h,
    BIT2l,
    BIT2m,
    BIT2a,
    BIT3b,
    BIT3c,
    BIT3d,
    BIT3e,
    BIT3h,
    BIT3l,
    BIT3m,
    BIT3a,
    BIT4b,
    BIT4c,
    BIT4d,
    BIT4e,
    BIT4h,
    BIT4l,
    BIT4m,
    BIT4a,
    BIT5b,
    BIT5c,
    BIT5d,
    BIT5e,
    BIT5h,
    BIT5l,
    BIT5m,
    BIT5a,
    BIT6b,
    BIT6c,
    BIT6d,
    BIT6e,
    BIT6h,
    BIT6l,
    BIT6m,
    BIT6a,
    BIT7b,
    BIT7c,
    BIT7d,
    BIT7e,
    BIT7h,
    BIT7l,
    BIT7m,
    BIT7a,
    RES0b,
    RES0c,
    RES0d,
    RES0e,
    RES0h,
    RES0l,
    RES0m,
    RES0a,
    RES1b,
    RES1c,
    RES1d,
    RES1e,
    RES1h,
    RES1l,
    RES1m,
    RES1a,
    RES2b,
    RES2c,
    RES2d,
    RES2e,
    RES2h,
    RES2l,
    RES2m,
    RES2a,
    RES3b,
    RES3c,
    RES3d,
    RES3e,
    RES3h,
    RES3l,
    RES3m,
    RES3a,
    RES4b,
    RES4c,
    RES4d,
    RES4e,
    RES4h,
    RES4l,
    RES4m,
    RES4a,
    RES5b,
    RES5c,
    RES5d,
    RES5e,
    RES5h,
    RES5l,
    RES5m,
    RES5a,
    RES6b,
    RES6c,
    RES6d,
    RES6e,
    RES6h,
    RES6l,
    RES6m,
    RES6a,
    RES7b,
    RES7c,
    RES7d,
    RES7e,
    RES7h,
    RES7l,
    RES7m,
    RES7a,
    SET0b,
    SET0c,
    SET0d,
    SET0e,
    SET0h,
    SET0l,
    SET0m,
    SET0a,
    SET1b,
    SET1c,
    SET1d,
    SET1e,
    SET1h,
    SET1l,
    SET1m,
    SET1a,
    SET2b,
    SET2c,
    SET2d,
    SET2e,
    SET2h,
    SET2l,
    SET2m,
    SET2a,
    SET3b,
    SET3c,
    SET3d,
    SET3e,
    SET3h,
    SET3l,
    SET3m,
    SET3a,
    SET4b,
    SET4c,
    SET4d,
    SET4e,
    SET4h,
    SET4l,
    SET4m,
    SET4a,
    SET5b,
    SET5c,
    SET5d,
    SET5e,
    SET5h,
    SET5l,
    SET5m,
    SET5a,
    SET6b,
    SET6c,
    SET6d,
    SET6e,
    SET6h,
    SET6l,
    SET6m,
    SET6a,
    SET7b,
    SET7c,
    SET7d,
    SET7e,
    SET7h,
    SET7l,
    SET7m,
    SET7a
};

static void _OPCODE_CB(GBContext *cpu) {
    (*_cb[read8(cpu, cpu->PC++)])(cpu); //can overflow here if the bin is malformed, just ignore for the moment.
}

static void (*opcodes[]) (GBContext *cpu) = {
    NOP,
    LDBCnn,
    LDBCmA,
    INCBC,
    INCr_b,
    DECr_b,
    LDrn_b,
    RLCA,
    LDmmSP,
    ADDHLBC,
    LDABCm,
    DECBC,
    INCr_c,
    DECr_c,
    LDrn_c,
    RRCA,
    STOP,
    LDDEnn,
    LDDEmA,
    INCDE,
    INCr_d,
    DECr_d,
    LDrn_d,
    RLA,
    JRn,
    ADDHLDE,
    LDADEm,
    DECDE,
    INCr_e,
    DECr_e,
    LDrn_e,
    RRA,
    JRNZn,
    LDHLnn,
    LDHLIA,
    INCHL,
    INCr_h,
    DECr_h,
    LDrn_h,
    DAA,
    JRZn,
    ADDHLHL,
    LDAHLI,
    DECHL,
    INCr_l,
    DECr_l,
    LDrn_l,
    CPL,
    JRNCn,
    LDSPnn,
    LDHLDA,
    INCSP,
    INCHLm,
    DECHLm,
    LDHLmn,
    SCF,
    JRCn,
    ADDHLSP,
    LDAHLD,
    DECSP,
    INCr_a,
    DECr_a,
    LDrn_a,
    CCF,
    LDrr_bb,
    LDrr_bc,
    LDrr_bd,
    LDrr_be,
    LDrr_bh,
    LDrr_bl,
    LDrHLm_b,
    LDrr_ba,
    LDrr_cb,
    LDrr_cc,
    LDrr_cd,
    LDrr_ce,
    LDrr_ch,
    LDrr_cl,
    LDrHLm_c,
    LDrr_ca,
    LDrr_db,
    LDrr_dc,
    LDrr_dd,
    LDrr_de,
    LDrr_dh,
    LDrr_dl,
    LDrHLm_d,
    LDrr_da,
    LDrr_eb,
    LDrr_ec,
    LDrr_ed,
    LDrr_ee,
    LDrr_eh,
    LDrr_el,
    LDrHLm_e,
    LDrr_ea,
    LDrr_hb,
    LDrr_hc,
    LDrr_hd,
    LDrr_he,
    LDrr_hh,
    LDrr_hl,
    LDrHLm_h,
    LDrr_ha,
    LDrr_lb,
    LDrr_lc,
    LDrr_ld,
    LDrr_le,
    LDrr_lh,
    LDrr_ll,
    LDrHLm_l,
    LDrr_la,
    LDHLmr_b,
    LDHLmr_c,
    LDHLmr_d,
    LDHLmr_e,
    LDHLmr_h,
    LDHLmr_l,
    HALT,
    LDHLmr_a,
    LDrr_ab,
    LDrr_ac,
    LDrr_ad,
    LDrr_ae,
    LDrr_ah,
    LDrr_al,
    LDrHLm_a,
    LDrr_aa,
    ADDr_b,
    ADDr_c,
    ADDr_d,
    ADDr_e,
    ADDr_h,
    ADDr_l,
    ADDHL,
    ADDr_a,
    ADCr_b,
    ADCr_c,
    ADCr_d,
    ADCr_e,
    ADCr_h,
    ADCr_l,
    ADCHL,
    ADCr_a,
    SUBr_b,
    SUBr_c,
    SUBr_d,
    SUBr_e,
    SUBr_h,
    SUBr_l,
    SUBHL,
    SUBr_a,
    SBCr_b,
    SBCr_c,
    SBCr_d,
    SBCr_e,
    SBCr_h,
    SBCr_l,
    SBCHL,
    SBCr_a,
    ANDr_b,
    ANDr_c,
    ANDr_d,
    ANDr_e,
    ANDr_h,
    ANDr_l,
    ANDHL,
    ANDr_a,
    XORr_b,
    XORr_c,
    XORr_d,
    XORr_e,
    XORr_h,
    XORr_l,
    XORHL,
    XORr_a,
    ORr_b,
    ORr_c,
    ORr_d,
    ORr_e,
    ORr_h,
    ORr_l,
    ORHL,
    ORr_a,
    CPr_b,
    CPr_c,
    CPr_d,
    CPr_e,
    CPr_h,
    CPr_l,
    CPHL,
    CPr_a,
    RETNZ,
    POPBC,
    JPNZnn,
    JPnn,
    CALLNZnn,
    PUSHBC,
    ADDn,
    RST00,
    RETZ,
    RET,
    JPZnn,
    _OPCODE_CB,
    CALLZnn,
    CALLnn,
    ADCn,
    RST08,
    RETNC,
    POPDE,
    JPNCnn,
    _UNDEFINED,
    CALLNCnn,
    PUSHDE,
    SUBn,
    RST10,
    RETC,
    RETI,
    JPCnn,
    _UNDEFINED,
    CALLCnn,
    _UNDEFINED,
    SBCn,
    RST18,
    LDIOnA,
    POPHL,
    LDIOCA,
    _UNDEFINED,
    _UNDEFINED,
    PUSHHL,
    ANDn,
    RST20,
    ADDSPn,
    JPHL,
    LDmmA,
    _UNDEFINED,
    _UNDEFINED,
    _UNDEFINED,
    ORn,
    RST28,
    LDAIOn,
    POPAF,
    LDAIOC,
    DI,
    _UNDEFINED,
    PUSHAF,
    XORn,
    RST30,
    LDHLSPn,
    LDSPHL,
    LDAmm,
    EI,
    _UNDEFINED,
    _UNDEFINED,
    CPn,
    RST38
};

void execute(GBContext *cpu, uint8_t opcode) {
    (*opcodes[opcode])(cpu);

    return;
}