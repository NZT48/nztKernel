#ifndef _DEFIN_H_
#define _DEFIN_H_

typedef unsigned int  Reg;

extern unsigned volatile lockFlag;

#define HARD_LOCK asm pushf;\
    asm cli;

#define HARD_UNLOCK asm popf;

/* Soft lock */
#define LOCK      ++lockFlag;
#define UNLOCK    --lockFlag;

#endif
