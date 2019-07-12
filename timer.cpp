#include "timer.h"
#include "pcb.h"
#include "scheduler.h"

#include <dos.h>
#include <stdlib.h>

int volatile Timer::req = 0;
InterruptRoutine Timer::oldTimerInt = 0;

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

        if(PCB::runningPCB->remaining > 0){
            PCB::runningPCB->remaining--;
            /*if(runningPCB->remaining == 0 && runningPCB->timeSlice != 0)
                runningPCB->remaining = 1;*/
        }
    }

    if(req || (PCB::runningPCB->timeSlice != 0 && PCB::runningPCB->remaining == 0)){

        //save
        asm {
                mov tss, ss;
                mov tsp, sp;
                mov tbp, bp;
        }

        PCB::runningPCB->ss = tss;
        PCB::runningPCB->sp = tsp;
        PCB::runningPCB->bp = tbp;

        //change
        if(PCB::runningPCB->state == PCB::RUNNING){ //&& runningPCB != idlePCB or MainPCB)
        	PCB::runningPCB->state = PCB::READY;
            Scheduler::put(PCB::runningPCB);
        }

        PCB::runningPCB = Scheduler::get();
        PCB::runningPCB->state = PCB::RUNNING;
        PCB::runningPCB->remaining = PCB::runningPCB->timeSlice;

        //restore
        tss = PCB::runningPCB->ss;
        tsp = PCB::runningPCB->sp;
        tbp = PCB::runningPCB->bp;

        asm {
            mov ss, tss;
            mov sp, tsp;
            mov bp, tbp;
        }

        req = 0;
    }
}
