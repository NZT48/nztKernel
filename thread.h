#ifndef _THREAD_H_
#define _THREAD_H_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
const StackSize maxStackSize = 1UL << 16;
typedef unsigned int Time; //time, x55ms
const Time defaultTimeSlice = 2;
typedef int ID;

class PCB; //Kernel's implementaton of a user's thread

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
    Thread (StackSize stackSize = defaultStackSize, Time timeSlice= defaultTimeSlice);
    virtual void run() {}

private:
    PCB* myPCB;
};

void dispatch();

#endif

