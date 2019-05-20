#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"

class PCB{
public:
    
    enum State { NEW, READY, BLOCKED, PAUSED, TERMINATED };


    
    void start();
    void join();
    virtual ~PCB();

    ID getID();
    static ID getRunningId();
    static PCB *getThreadById(ID id);

protected:

    PCB (Thread *thread, StackSize stackSize, Time timeSlice);

private:
    ID myID;

    unsigned bp;
	unsigned sp;
	unsigned ss;

};

#endif