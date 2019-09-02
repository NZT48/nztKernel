#include <dos.h>

#include "pcb.h"
#include "schedule.h"
#include "timer.h"


PCBList* PCB::pcbList = new PCBList();
ID PCB::lastId = 0;

volatile int PCB::globallyBlockedSignals[16] = {0};


PCB::PCB (Thread *thread, StackSize stackSize, Time timeSlice){

    myID = ++lastId;
    myThread = thread;
    blockedPCB = new PCBList();

    this->stackSize = stackSize / sizeof(unsigned);
    stack = new unsigned int[stackSize];

    stack[stackSize - 1] = 0x200; /* PSWI = 1 */
    stack[stackSize - 2] = FP_SEG(PCB::threadWrapper); /* Code segment */
    stack[stackSize - 3] = FP_OFF(PCB::threadWrapper); /* Instruction pointer */

    ss = FP_SEG(stack + stackSize - 12);
    sp = FP_OFF(stack + stackSize - 12);
    bp = FP_OFF(stack + stackSize - 12);

    if(myID == 2) Timer::idlePCB = this;

    this->timeSlice = timeSlice;
    remaining = timeSlice;
    state = NEW;
    wakeSignal = 1;
    pcbList->put(this);

    /* Implementation for signal */

    sLock = 0;
    parentPCB = Timer::runningPCB;
    signalList = new SignalList();

    if(myID > 1){
    	for(int i = 0; i < 16; i++){
    		PCB::HandlerList::HandlerElem* tmp = parentPCB->handlers[i].head;
    		while(tmp){
    			handlers[i].put(tmp->handler);
    			tmp = tmp->next;
   			}
   			if(parentPCB->myID == 1)
   				blockedSignals[i] = 0;
   			else
   				blockedSignals[i] = parentPCB->blockedSignals[i];
    	}
    } else {
    	handlers[0].put(signal0);
    }
}

PCB::~PCB(){
    blockedPCB->release();
    delete blockedPCB;
    delete signalList;
    if(stack) delete[] stack;
}

void PCB::reschedule(){
	if(this != Timer::idlePCB){
		state = READY;
		Scheduler::put(this);
	}
}

void PCB::waitToComplete(){

    if(this == Timer::runningPCB || this->state == FINISHED
        || this == Timer::idlePCB || this->state == NEW) return;

    HARD_LOCK
    Timer::runningPCB->state = BLOCKED;
    blockedPCB->put(Timer::runningPCB);
    HARD_UNLOCK
    dispatch();
}

void PCB::threadWrapper() {

    Timer::runningPCB->myThread->run();

    Timer::runningPCB->blockedPCB->release();
    Timer::runningPCB->parentPCB->signal(1);
    Timer::runningPCB->serve(2);
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

void PCB::signal(SignalId signal){
    if(signal > 15) return;
    signalList->put(signal);
}


void PCB::registerHandler(SignalId signal, SignalHandler handler){
	if(signal > 15) return;
    LOCK;
        handlers[signal].put(handler);
    UNLOCK;
}

void PCB::unregisterAllHandlers(SignalId id){
	if(id > 15) return;
    LOCK;
        handlers[id].removeAll();
    UNLOCK;
}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	if(id > 15) return;
    HandlerList::HandlerElem *tmp = handlers[id].head, *first = 0, *second = 0;
	while(tmp){
		if(tmp->handler == hand1)
			first = tmp;
		else if(tmp->handler == hand2)
			second = tmp;
		if(first && second){
			SignalHandler tmp2 = first->handler;
			first->handler = second->handler;
			second->handler = tmp2;
			break;
		}
		tmp = tmp->next;
	}
}


void PCB::blockSignal(SignalId signal){
	if(signal > 15) return;
    LOCK;
        blockedSignals[signal] = 1;
    UNLOCK;
}

void PCB::blockSignalGlobally(SignalId signal){
	if(signal > 15) return;
    LOCK;
        PCB::globallyBlockedSignals[signal] = 1;
    UNLOCK;
}

void PCB::unblockSignal(SignalId signal){
	if(signal > 15) return;
    LOCK;
        blockedSignals[signal] = 0;
    UNLOCK;
}

void PCB::unblockSignalGlobally(SignalId signal){
	if(signal > 15) return;
    LOCK;
        PCB::globallyBlockedSignals[signal] = 0;
    UNLOCK;
}

void PCB::serve(SignalId signal){
	if(signal > 15) return;
	PCB::HandlerList::HandlerElem* tmp = handlers[signal].head;
	while(tmp){
		tmp->handler();
		tmp = tmp->next;
	}
}

/* Kill signal */
void PCB::signal0(){
	HARD_LOCK;
	Timer::runningPCB->state = PCB::FINISHED;
	Timer::runningPCB->blockedPCB->release();
	Timer::runningPCB->parentPCB->signal(1);
	HARD_UNLOCK;
	dispatch();

}


PCB::HandlerList::~HandlerList(){
    LOCK;
    HandlerList::HandlerElem* tmp;
	while(head){
		tmp = head;
		head = head->next;
		delete tmp;
	}
    UNLOCK;
}

void PCB::HandlerList::put(SignalHandler hand){
    HandlerElem* tmp = new HandlerElem(hand);
    if(head)
        tail->next = tmp;
    else
        head = tmp;
    tail = tmp;
}

void PCB::HandlerList::removeAll(){
    HandlerElem *cur = head, *tmp;
	while(cur){
		tmp = cur;
		cur = cur->next;
		HARD_LOCK;
			delete tmp;
		HARD_UNLOCK;
	}
	head = tail = 0;
}
