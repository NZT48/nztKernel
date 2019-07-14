#include <dos.h>

#include "pcb.h"
#include "pcblist.h"
#include "SCHEDULE.H"
#include "timer.h"

PCBList* PCB::pcbList = new PCBList();
ID PCB::lastId = 0;

PCB::PCB(){
    timeSlice = remaining = 20;
}

PCB::PCB (Thread *thread, StackSize stackSize, Time timeSlice){

    myID = ++lastId;
    myThread = thread;
    blockedPCB = new PCBList();

    this->stackSize = stackSize / sizeof(unsigned);
    stack = new unsigned int[stackSize];

    stack[stackSize - 1] = 0x200; //PSWI = 1
    stack[stackSize - 2] = FP_SEG(PCB::threadWrapper);
    stack[stackSize - 3] = FP_OFF(PCB::threadWrapper);
    stack[stackSize - 12] = 0; //check it!

    ss = FP_SEG(stack + stackSize - 12);
    sp = FP_OFF(stack + stackSize - 12);
    bp = FP_OFF(stack + stackSize - 12);

    //Missing PCB list

    this->timeSlice = timeSlice;
    remaining = timeSlice;
    state = NEW;
    wakeSignal = 0; //can also be 1
    pcbList->put(this);

}

PCB::~PCB(){
    blockedPCB->release();
    if(stack) delete[] stack;
}

void PCB::reschedule(){
    state = READY;
    HARD_LOCK
    Scheduler::put(this);
    HARD_UNLOCK
}

void PCB::waitToComplete(){

    if(this == Timer::runningPCB || this->state == FINISHED
        || this == Timer::idlePCB || this->state == NEW) return;

    HARD_LOCK
    //cout << endl << Timer::runningPCB->getID() << " is waitin for " << getID() << endl;
    Timer::runningPCB->state = BLOCKED;
    blockedPCB->put(Timer::runningPCB);
    HARD_UNLOCK
    dispatch();
}

void PCB::threadWrapper() {
    Timer::runningPCB->myThread->run();

    Timer::runningPCB->blockedPCB->release();
    Timer::runningPCB->state = FINISHED;
    dispatch();
}


ID PCB::getID(){
    return myID;
}

ID PCB::getRunningId(){
    return Timer::runningPCB->getID();
}

Thread *PCB::getThreadById(ID id){
    PCBList::Node* ret = pcbList->front;
    while( ret != 0 && ret->pcb->getID() != id){
        ret = ret->next;
    }

    if(ret)
        return ret->pcb->myThread;
    else
        return 0;
}
