#ifndef _TIMER_H_
#define _TIMER_H_

typedef void interrupt (*InterruptRoutine)(...);


class Timer {

public:
    static void interrupt timerInt(...);
    static void inic();
    static void restore();

    static PCB* mainPCB;
    static PCB* idlePCB;
    static PCB* runningPCB;

    static int volatile req;

private:
    static InterruptRoutine oldTimerInt;
    
};

void tick();

#endif
