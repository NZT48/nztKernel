#ifndef _DEFIN_H_
#define _DEFIN_H_

//From threadh



//PCBh
typedef unsigned int  Reg;
//extern volatile PCB* running;

//Timerh
typedef int IVTNo;

//locks
#define HARD_LOCK asm pushf;\
    asm cli;

#define HARD_UNLOCK asm popf;

#endif
