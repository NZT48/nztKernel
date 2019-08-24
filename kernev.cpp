#include "kernev.h"

#include "ivtEntry.h"
#include "pcb.h"
#include "timer.h"

KernelEv::KernelEv(IVTNo id1, Event* myEv) {
    id = id1;
    myEvent = myEv;
    value = 1;
    myPCB = Timer::runningPCB;
    IVTEntry::ivtEntries[id1]->setEvent(this);
}

KernelEv::~KernelEv() {
	IVTEntry::ivtEntries[id]->restore();
}

void KernelEv::wait() {
    LOCK;
    if(myPCB == Timer::runningPCB && value == 1) {
        myPCB->state = PCB::BLOCKED;
        value = 0;
        UNLOCK;
        dispatch();
    } else UNLOCK;
}

void KernelEv::signal() {
    LOCK;
    if( value == 0 ) {
        value = 1;
        myPCB->reschedule();
    }
    UNLOCK;
}
