#ifndef INPUT_H
#define	INPUT_H

#include "cpu.h"

#define IO_P1 *(joyp->io.P1)

typedef enum {
    IO_P1_ADDR = 0xFF00,
} JOYPRegs_addr;

typedef struct {
    uint8_t *P1;
} JOYPRegs;

typedef struct {
    JOYPRegs io;

    int up, down, left, right, select, start, a, b;
} JOYPContext;


void         joyp_ctx_update(GBContext *, JOYPContext *);
JOYPContext *joyp_init(GBContext *);
void         joyp_destroy(JOYPContext *);

#endif	/* INPUT_H */

