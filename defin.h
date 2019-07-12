#ifndef _DEFIN_H_
#define _DEFIN_H_

//PCBh
typedef unsigned int  Reg;

//Timerh
typedef int IVTNo;

//locks
#define HARD_LOCK asm pushf;\
    asm cli;

#define HARD_UNLOCK asm popf;

#endif
