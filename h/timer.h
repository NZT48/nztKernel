#ifndef _TIMER_H_
#define _TIMER_H_

#include "pcb.h"

typedef void interrupt (*InterruptRoutine)(...);


class Timer {

public:
    static void interrupt timerInt(...);
    static void inic(); /* Change interrupt routines */
    static void restore(); /* Restore old interrupt routine at the end */

	static PCB* mainPCB;
	static PCB* idlePCB;
	static PCB* runningPCB;

    static int volatile req; /* Request indicator - for explicit change of context */

private:
    static InterruptRoutine oldTimerInt;
};

void tick(); /* Used in test, doing nothing */

#endif
