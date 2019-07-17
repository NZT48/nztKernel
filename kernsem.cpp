#include "kernsem.h"
#include "pcb.h"
#include "timer.h"
#include <iostream>

KernelSem::~KernelSem() {
    //delete timeList;
}

int KernelSem::wait(Time maxTimeToWait) {
    HARD_LOCK
    Timer::runningPCB->wakeSignal = 1;
    if(--value < 0) {
        Timer::runningPCB->wakeSignal = 0;
        Timer::runningPCB->state = Timer::BLOCKED;
        HARD_UNLOCK
        dispatch();
    }
    HARD_UNLOCK
    return Timer::runningPCB->wakeSignal;
}

int KernelSem::signal(int n) {
    HARD_LOCK
    if(++value <= 0) {
        return 0;
    }
    HARD_UNLOCK
    return 1;
}
