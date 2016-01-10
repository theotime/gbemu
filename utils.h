#ifndef UTILS_H
#define UTILS_H

#define GET_BIT(bits, bit_pos) ((bits & bit_pos) != 0)
#define SET_BIT(bits, bit_pos) (bits |= bit_pos)
#define RES_BIT(bits, bit_pos) (bits &= ~bit_pos)
#define VAL_BIT(bits, bit_pos, v) ((v) ? SET_BIT(bits, bit_pos) :  RES_BIT(bits, bit_pos))

#define BIT0 (1 << 0)
#define BIT1 (1 << 1)
#define BIT2 (1 << 2)
#define BIT3 (1 << 3)
#define BIT4 (1 << 4)
#define BIT5 (1 << 5)
#define BIT6 (1 << 6)
#define BIT7 (1 << 7)

#define BIT8 (1 << 8)
#define BIT9 (1 << 9)
#define BIT10 (1 << 10)
#define BIT11 (1 << 11)
#define BIT12 (1 << 12)
#define BIT13 (1 << 13)
#define BIT14 (1 << 14)
#define BIT15 (1 << 15)

#define BIT16 (1 << 16)


#endif // UTILS_H