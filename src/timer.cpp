#include "timer.h"
#include "kernsem.h"
#include "pcb.h"

#include <dos.h>
#include <stdlib.h>
#include "schedule.h"

int volatile Timer::req = 0;
InterruptRoutine Timer::oldTimerInt = 0;

PCB* Timer::idlePCB = 0;
PCB* Timer::mainPCB = 0;
PCB* Timer::runningPCB = 0;

static Reg volatile tsp, tss, tbp;

/* Change IVTable and set my Timer interrupt */
void Timer::inic() {
    HARD_LOCK
    oldTimerInt = getvect(0x08);
    setvect(0x08,timerInt);
    HARD_UNLOCK
}

void Timer::restore() {
    HARD_LOCK
    setvect(0x08, oldTimerInt);
    HARD_UNLOCK
}

void interrupt Timer::timerInt(...) {

    if(!req){ /* Not requested interrupt */

        oldTimerInt(); /* Call old timer interrupt */
        tick(); /* Used in public test */
        KernelSem::FirstSem->sem->timerUpdate(); /* Update timeLeft of waiting PCBs */

        if(runningPCB->remaining > 0){
            runningPCB->remaining--; /* Decrement time given to thread for running */
            /* If remaining time for running thread is 0 and its forbidden to change context
             * by soft lock then set remaining time to 1
             */
            if(runningPCB->remaining == 0 && lockFlag && runningPCB->timeSlice != 0)
            				runningPCB->remaining = 1;
        }
    }


    if(req || (runningPCB->timeSlice != 0 && runningPCB->remaining == 0 && !lockFlag)){

    	/* save context */
        asm {
                mov tss, ss;
                mov tsp, sp;
                mov tbp, bp;
        }

        runningPCB->ss = tss;
        runningPCB->sp = tsp;
        runningPCB->bp = tbp;
        runningPCB->sLock = lockFlag;

        /* Put running thread to scheduler */
        if(runningPCB->state == PCB::RUNNING && runningPCB != Timer::mainPCB && (runningPCB != idlePCB)){
        	runningPCB->state = PCB::READY;
            Scheduler::put(runningPCB);
        }

        runningPCB = Scheduler::get();
        if(runningPCB == 0){ /* If all threads are blocked and there are no ready threads */
        	runningPCB = idlePCB;
        }
        runningPCB->state = PCB::RUNNING;
        runningPCB->remaining = runningPCB->timeSlice;

        /*restore context*/
        tss = runningPCB->ss;
        tsp = runningPCB->sp;
        tbp = runningPCB->bp;
		lockFlag = runningPCB->sLock;

        asm {
            mov ss, tss;
            mov sp, tsp;
            mov bp, tbp;
        }

        /* Reset request indicator */
        req = 0;

        /* Processing signals */

		LOCK; /* Allow change of context */
        asm sti; /* Disallow interrupts */
        int sig = runningPCB->signalList->get();
        /* Process every signal sent to running thread */
        for(; sig != -1; sig = runningPCB->signalList->get()){
            runningPCB->serve(sig);
        }
        asm cli;
        UNLOCK;
    }
}
