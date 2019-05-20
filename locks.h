#ifndef _LOCKS_H_
#define _LOCKS_H_

#define HARD_LOCK asm pushf;\
    asm cli;

#define HARD_UNLOCK asm popf;

#endif