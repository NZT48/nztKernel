#include "dos.h"
#include "ivtEntry.h"

IVTEntry* IVTEntry::ivtEntries[256] = {0};

IVTEntry::IVTEntry(IVTNo ivtNo_, pInterrupt newRoutine_){
    ivtNo = ivtNo_;
    event = 0;
    ivtEntries[ivtNo] = this;

    newRoutine = newRoutine_;
    HARD_LOCK;
    oldRoutine = getvect(ivtNo);
    setvect(ivtNo, newRoutine);
    HARD_UNLOCK;
}

void IVTEntry::restore() {
	HARD_LOCK;
    setvect(ivtNo, oldRoutine);
    HARD_UNLOCK;
    event = 0;
}

IVTEntry::~IVTEntry(){
	restore();
}


void IVTEntry::setEvent(KernelEv *event_) {
    event = event_;
}

void IVTEntry::signal() {
    event->signal();
}

void IVTEntry::callOldRoutine() {
    if (oldRoutine != 0) {
        oldRoutine();
    }
}
