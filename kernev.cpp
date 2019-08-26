#include "ivtEntry.h"
#include "timer.h"

KernelEv::KernelEv(IVTNo id_, Event* myEv) {
    id = id_;
    myEvent = myEv;
    value = 1;
    myPCB = Timer::runningPCB;
    IVTEntry::ivtEntries[id]->setEvent(this);
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
