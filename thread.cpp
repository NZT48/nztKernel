#include "thread.h"
#include "locks.h"

Thread::Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice){
    if(stackSize > maxStackSize) stackSize = maxStackSize;
    HARD_LOCK
    myPCB = new PCB(this, stackSize, timeSlice);
    HARD_UNLOCK
}

Thread::~Thread(){
    waitToComplete();
    HARD_LOCK
    delete myPCB;
    HARD_UNLOCK
}

void Thread::start(){
    myPCB->start();
}

void Thread::waitToComplete(){
    myPCB->join();
}

ID Thread::getID() {
    return myPCB->getID();
}


ID Thread::getRunningId(){
    return PCB::running->getID();
}
Thread *Thread::getThreadById(ID id){
    return PCB::getThreadById(id);
}
