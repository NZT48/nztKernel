#include "kernsem.h"
#include "schedule.h"
#include "timer.h"

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
    /* Ignore and send back n */
    if (n < 0 ) {
    	UNLOCK;
    	return n;
    }

    int t = 0;
    if ( n == 0 ) { n = 1; } /* Standard signal call */

    while( n > 0 && value <= 0 ){ /* Unblock n threads */
		++value;
		PCB* p = blocked->get();
		p->reschedule();
		p->wakeSignal = 1; /* Woke up by signal, wait return value is 1 */
		t++; n--;
	}
    UNLOCK;

    return t;
}

void KernelSem::timerUpdate() {

	/* Go through every Semaphore that exists */
    for(SemListElem* cur = KernelSem::FirstSem; cur != 0; cur = cur->next) {
    	/*Get the list of blocked PCBs from current semaphore */
    	PCBList* q = cur->sem->blocked;
    	/* Go through every PCB in the blocked list */
    	for(PCBList::Node* e = q->front; e != 0 && e->timeLeft != infiniteTimeSlice; ) {
    		/* If maxTimeToWait reached 0 then unblock it */
    		if(--e->timeLeft == 0) {
    			cur->sem->value++;
    			PCB* pcb = q->get();
    			pcb->wakeSignal = 0; /* Woke up by timer, wait return value is 0*/
    			pcb->reschedule();
    			e = q->front;
    		} else {
    			e = e->next;
    	    }
    	}
    }
}
