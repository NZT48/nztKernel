#ifndef _DEFIN_H_
#define _DEFIN_H_

//From threadh
typedef unsigned long StackSize;
typedef unsigned int Time; //time, x55ms
typedef int ID;


//PCBh
typedef unsigned int  Register;
//extern volatile PCB* running;

//Timerh
typedef int IVTNo;

//locks
#define HARD_LOCK asm pushf;\
    asm cli;

#define HARD_UNLOCK asm popf;

#endif
