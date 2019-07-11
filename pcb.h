#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "defin.h"

class Thread;

class PCB {
public:

    enum State { NEW, READY, BLOCKED, RUNNING, FINISHED };

    PCB (Thread *thread, StackSize stackSize, Time timeSlice);
    ~PCB();

    void reschedule();
    void waitToComplete();

    ID getID();
    static ID getRunningId();
    static Thread *getThreadById(ID id);

    Thread* getThread() { return myThread; }

    static PCB* runningPCB; //or in Timerh
    static PCB* idlePCB;
    static PCBList* pcbList;

protected:


private:

    ID myID;
    Time remaining, timeSlice;
    State state;

    unsigned* stack; ///or unsigned int
    StackSize stackSize_;

    Thread* myThread;

    //for semaphor
    int wakeSignal;

    //friend class Thread;

    Reg bp;
	Reg sp;
	Reg ss;

};

#endif
