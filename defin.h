#ifndef _DEFIN_H_
#define _DEFIN_H_

//PCBh
typedef unsigned int Reg;
extern unsigned volatile lock;


//Timerh
typedef int IVTNo;

//locks
#define HARD_LOCK asm pushf; \
        asm cli;

#define HARD_UNLOCK asm popf;

#define LOCK      ++lock
#define UNLOCK    /*if (--lock == 0 && Timer::lockTimedOut) {  dispatch(); }*/ --lock;
#define LOCKED(s) LOCK; s UNLOCK

#endif
