#include "thread.h"
#include "defin.h"
#include "timer.h"
#include "pcb.h"

Thread::Thread(StackSize stackSize, Time timeSlice){
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
    myPCB->reschedule();
}

void Thread::waitToComplete(){
    myPCB->waitToComplete();
}

ID Thread::getID() {
    return myPCB->getID();
}

ID Thread::getRunningId(){
    return Timer::runningPCB->getID();
}

Thread *Thread::getThreadById(ID id){
    return PCB::getThreadById(id);
}

void Thread::signal(SignalId signal){
    myPCB->signal(signal);
}

void Thread::registerHandler(SignalId signal, SignalHandler handler){
    myPCB->registerHandler(signal, handler);
}

void Thread::unregisterAllHandlers(SignalId id){
    myPCB->unregisterAllHandlers(id);
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
    myPCB->swap(id, hand1, hand2);
}


void Thread::blockSignal(SignalId signal){
    myPCB->blockSignal(signal);
}

void Thread::blockSignalGlobally(SignalId signal){
    PCB::blockSignalGlobally(signal);
}

void Thread::unblockSignal(SignalId signal){
    myPCB->unblockSignal(signal);
}

void Thread::unblockSignalGlobally(SignalId signal){
    PCB::unblockSignalGlobally(signal);
}

void dispatch(){
    HARD_LOCK
    Timer::req = 1;
    Timer::timerInt();
    HARD_UNLOCK
}
