#ifndef _TIMER_H_
#define _TIMER_H_

typedef void interrupt (*InterruptRoutine)(...);


class Timer {
public:
    static void interrupt timerInt(...);
    static void inic();
    static void restore();

    static int volatile req;

    static InterruptRoutine oldTimerInt;
};

void tick();

#endif
