#include "timer.h"
#include "pcb.h"
#include "schedule.h"

#include <dos.h>
#include <stdlib.h>

int volatile Timer::req = 0;
InterruptRoutine Timer::oldTimerInt = 0;

PCB* Timer::idlePCB = 0;
PCB* Timer::mainPCB = 0;
PCB* Timer::runningPCB = 0;

static Reg volatile tsp, tss, tbp;


void Timer::inic() {
    HARD_LOCK
    oldTimerInt = getvect(0x08);
    setvect(0x08,timerInt);
    //setvect(60, oldTimerInt);
    HARD_UNLOCK
}

void Timer::restore() {
    HARD_LOCK
    setvect(0x08, oldTimerInt);
    HARD_UNLOCK
}


void interrupt Timer::timerInt(...) {
    if(!req){
        oldTimerInt();
        tick();

        if(runningPCB->remaining > 0){
            runningPCB->remaining--;
            /*if(runningPCB->remaining == 0 && runningPCB->timeSlice != 0)
                runningPCB->remaining = 1;*/
        }
    }

    if(req || (runningPCB->timeSlice != 0 && runningPCB->remaining == 0)){

        //save
        asm {
                mov tss, ss;
                mov tsp, sp;
                mov tbp, bp;
        }

        runningPCB->ss = tss;
        runningPCB->sp = tsp;
        runningPCB->bp = tbp;

        //change
        if(runningPCB->state == PCB::RUNNING){ //&& runningPCB != idlePCB or MainPCB)
        	runningPCB->state = PCB::READY;
            Scheduler::put(runningPCB);
        }

        runningPCB = Scheduler::get();
        runningPCB->state = PCB::RUNNING;
        runningPCB->remaining = runningPCB->timeSlice;

        //restore
        tss = runningPCB->ss;
        tsp = runningPCB->sp;
        tbp = runningPCB->bp;

        asm {
            mov ss, tss;
            mov sp, tsp;
            mov bp, tbp;
        }

        req = 0;
    }
}
