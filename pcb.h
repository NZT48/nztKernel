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
    static PCB *getThreadById(ID id);

protected:


private:

    ID myID;
    Time remaining, timeSlice;
    State state;

    unsigned* stack; ///moze i unsigned int
    StackSize stackSize_;

    Thread* myThread;

    //friend class Thread;

    unsigned bp;
	unsigned sp;
	unsigned ss;

};

#endif
