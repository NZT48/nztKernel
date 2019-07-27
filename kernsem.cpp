#include "kernsem.h"
#include "pcb.h"
#include "timer.h"
#include "SCHEDULE.h"
//#include <iostream.h>

KernelSem::~KernelSem() {
    delete blocked;
}

int KernelSem::wait(Time maxTimeToWait) {
    LOCK;
    Timer::runningPCB->wakeSignal = 1;
    if(--value < 0) {
        Timer::runningPCB->wakeSignal = 0;
        Timer::runningPCB->state = PCB::BLOCKED;
        blocked->put(Timer::runningPCB);
        //if(maxTimeToWait > 0) waitList->add(maxTimeToWait,Timer::runningPCB, this);
        UNLOCK;
        dispatch();
    }
    UNLOCK;
    return Timer::runningPCB->wakeSignal;
}

int KernelSem::signal(int n) {
    LOCK:
    if(++value <= 0) {
        PCB* p = blocked->get();
        //p->state = PCB::READY;
        //Scheduler::put(p);
        p->reschedule();
        p->wakeSignal = 1;
        UNLOCK;
        return 0;
    } else UNLOCK;
    return 1;
}
