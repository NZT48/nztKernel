#include "dos.h"
#include "kernev.h"
#include "event.h"
#include "ivtEntry.h"

IVTEntry* IVTEntry::ivtEntries[256] = {0};


IVTEntry::IVTEntry(IVTNo ivtNo_, pInterrupt newRoutine1 ){
    ivtNo = ivtNo_;
    newRoutine = newRoutine1;
    oldRoutine = getvect(ivtNo);
    setvect(ivtNo, newRoutine);
    event = 0;
    ivtEntries[ivtNo] = this;
}

void IVTEntry::restore() {
    setvect(ivtNo, oldRoutine);
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
