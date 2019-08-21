#include "kernsem.h"
#include "pcb.h"
#include "timer.h"
#include "SCHEDULE.h"
#include "waitlist.h"

SemListElem* KernelSem::FirstSem = 0;

SemListElem::SemListElem(KernelSem* s, SemListElem* n) : sem(s), next(n) {}

KernelSem::KernelSem(int init){

    value = init;

    blocked = new PCBList();

    KernelSem::FirstSem = new SemListElem(this, KernelSem::FirstSem);

}

KernelSem::~KernelSem() {
    delete blocked;

    if( this == KernelSem::FirstSem->sem) {
        SemListElem* old = KernelSem::FirstSem;
        KernelSem::FirstSem = KernelSem:FirstSem->next;
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
		delete cur;
    }
}

int KernelSem::wait(Time maxTimeToWait) {
    LOCK;
    Timer::runningPCB->wakeSignal = 1;

    if(--value < 0) {
        Timer::runningPCB->wakeSignal = 0;
        Timer::runningPCB->state = PCB::BLOCKED;
        blocked->PriorPut(Timer::runningPCB, maxTimeToWait);
        UNLOCK;
        dispatch();
    } else UNLOCK;

    return Timer::runningPCB->wakeSignal;
}

int KernelSem::signal(int n) {
    LOCK;

    if(++value <= 0) {
        PCB* p = blocked->get();
        p->reschedule();
        p->wakeSignal = 1;
        UNLOCK;
        return 0;
        }
    } else UNLOCK;
    return 1;
}

void KernelSem::timerUpdate() {

    for(SemListElem* cur = KernelSem::FirstSem; cur != 0; cur = cur->next) {

    	PCBList* q = cur->sem->blocked;

        for(PCBList::Node* e = q->front; e != 0 && e->timeLeft != infiniteTimeSlice; ) {

            if(--e->timeLeft == 0) {

                cur->sem->value++;
                PCB* pcb = q->get();
                pcb->reschedule();
                e = q->front;
            } else {
                e = e->next;
            }
        }

    }
}
