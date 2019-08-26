#ifndef _THREAD_H_
#define _THREAD_H_

typedef int ID;
typedef unsigned long StackSize;
typedef unsigned int Time; //time, x55ms
const StackSize defaultStackSize = 4096;
const StackSize maxStackSize = 1UL << 16;

const Time defaultTimeSlice = 2;
const Time maximumTimeSlice = 65534;
const Time infiniteTimeSlice = 65535;

class PCB;

class Thread {

public:
    void start();
    void waitToComplete();
    virtual ~Thread();

    ID getID();
    static ID getRunningId();
    static Thread *getThreadById(ID id);

protected:
    friend class PCB;
    Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
    virtual void run() {}

private:
    PCB* myPCB;

};

void dispatch();

#endif
