#include "kernsem.h"
#include "timer.h"
#include "SCHEDULE.h"

KernelSem::SemListElem* KernelSem::FirstSem = 0;


KernelSem::KernelSem(int init){
	value = init;
    blocked = new PCBList();
    KernelSem::FirstSem = new KernelSem::SemListElem(this, KernelSem::FirstSem);
}

KernelSem::~KernelSem() {
	HARD_LOCK;
    delete blocked;
    HARD_UNLOCK;

    if( this == KernelSem::FirstSem->sem) {

        SemListElem* old = KernelSem::FirstSem;
        KernelSem::FirstSem = KernelSem::FirstSem->next;

        HARD_LOCK;
        delete old;
        HARD_UNLOCK;

    } else {

        SemListElem *prev = KernelSem::FirstSem, *cur = KernelSem::FirstSem->next;

        while (cur->sem != this) {
			prev = cur;
			cur = cur->next;
		}
		prev->next = cur->next;

		HARD_LOCK;
		delete cur;
		HARD_UNLOCK;
    }
}

int KernelSem::wait(Time maxTimeToWait) {
    LOCK;

    Timer::runningPCB->wakeSignal = 0;

    if(--value < 0) {
        Timer::runningPCB->state = PCB::BLOCKED;
        blocked->PriorPut(Timer::runningPCB, maxTimeToWait);
        UNLOCK;
        dispatch();

    } else UNLOCK;

    return Timer::runningPCB->wakeSignal;
}

int KernelSem::signal(int n) {
    LOCK;

    if (n < 0 ) {
    	UNLOCK;
    	return n;
    }

    int t = 0;
    if ( n == 0 ) { n = 1; }

    while( n > 0 && value <= 0 ){
		++value;
		PCB* p = blocked->get();
		p->reschedule();
		p->wakeSignal = 1;
		t++; n--;
	}
    UNLOCK;

    return t;
}

void KernelSem::timerUpdate() {

    for(SemListElem* cur = KernelSem::FirstSem; cur != 0; cur = cur->next) {

    	PCBList* q = cur->sem->blocked;

    	for(PCBList::Node* e = q->front; e != 0 && e->timeLeft != infiniteTimeSlice; ) {

    		if(--e->timeLeft == 0) {
    			cur->sem->value++;
    			PCB* pcb = q->get();
    			pcb->wakeSignal = 0;
    			pcb->reschedule();
    			e = q->front;
    		} else {
    			e = e->next;
    	    }
    	}
    }
}
