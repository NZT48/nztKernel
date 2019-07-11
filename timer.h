#ifndef _TIMER_H_
#define _TIMER_H_

class Timer {
public:
    static void interrupt timerInt(...);
    static void inic();
    static void restore();

    static int volatile req;

private:
    static void interrupt (*oldTimerInt)(...);
    //static const int IVTE;
};

void tick();

#endif
