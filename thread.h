#ifndef _THREAD_H_
#define _THREAD_H_

const StackSize defaultStackSize = 4096;
const StackSize maxStackSize = 1UL << 16;
const Time defaultTimeSlice = 2;

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
