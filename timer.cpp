#include "timer.h"
#include "pcb.h"

#include <dos.h>
#include <stdlib.h>

int volatile Timer::req = 0;

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
    setvect(0x08, odlTimerInt);
    HARD_UNLOCK
}


void interrupt Timer::timerInt() {
    if(!req){
        oldTimerInt();
        tick();

        if(runingPCB->remaining > 0){
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
        if(runningPCB->state == PCB::RUNNING){//&& runningPCB != idlePCB or MainPCB){
            runningPCB->state = PCB:READY;
            Scheduler::put(runningPCB);
        }

        runningPCB = Scheduler::get();
        runningPCB->state = PCB::RUNNING;
        runingPCB->remaining = runningPCB->timeSlice;

        //restore
        tss = runningPCB->ss;
        tsp = runningPCB->sp;
        tbp = runningPCB->bp;

        asm {
            mov ss, tss;
            mov sp, tsp;
            mov bp, tbp;
        }

        //reset request indicator
        req = 0;
    }
}
