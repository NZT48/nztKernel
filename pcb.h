#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "defin.h"
#include "pcblist.h"

class Thread;
class PCBList;

class PCB {
public:

    enum State { NEW, READY, BLOCKED, RUNNING, FINISHED };

    PCB();
    PCB (Thread *thread, StackSize stackSize, Time timeSlice);
    ~PCB();

    void reschedule();
    void waitToComplete();

    static void threadWrapper();

    ID getID();
    static ID getRunningId();
    static Thread *getThreadById(ID id);

    static PCBList* pcbList;
    static ID lastId;

    ID myID;
    Time remaining, timeSlice;
    State state;

    unsigned* stack; //or unsigned int
    StackSize stackSize;

    Thread* myThread;
    PCBList blockedPCB;

    //for semaphor
    int wakeSignal;

    Reg bp;
	Reg sp;
	Reg ss;

};

#endif
