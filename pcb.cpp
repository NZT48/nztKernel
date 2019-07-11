#include <dos.h>

#include "pcb.h"
#include "SCHEDULE.H"

PCB* PCB::runningPCB = 0;
PCB* PCB::idlePCB = 0;

PCBList* PCB::pcbList = new PCBList();


PCB::PCB (Thread *thread, StackSize stackSize, Time timeSlice){

}

PCB::~PCB(){

}

void PCB::reschedule(){
    state = READY;
    HARD_LOCK
    Scheduler::put(this);
    HARD_UNLOCK
}

void PCB::waitToComplete(){

    if(this == PCB::runningPCB || this->state == FINISHED ||
        this == PCB::idlePCB) return;

    HARD_LOCK
    PCB::runningPCB->state = BLOCKED;
    pcbList->put(PCB::runningPCB);
    HARD_UNLOCK
    dispatch();
}


ID PCB::getID(){
    return myID;
}

static ID PCB::getRunningId(){
    return runningPCB->getID();
}

static Thread *PCB::getThreadById(ID id){
    PCBList::Node* ret = pcbList->head();
    while( ret != 0 && ret->pcb->getID() != id){
        ret = ret->next;
    }

    if(ret)
        return ret->pcb->getThread();
    else
        return 0;
}
