#ifndef _THREAD_H_
#define _THREAD_H_

typedef int ID;
typedef unsigned long StackSize;
typedef unsigned int Time; /* time, x55ms */
const StackSize defaultStackSize = 4096;
const StackSize maxStackSize = 1UL << 16;

const Time defaultTimeSlice = 2; /* default = 2 * 55ms */
const Time maximumTimeSlice = 65534;
const Time infiniteTimeSlice = 65535;

typedef void (*SignalHandler) ();

typedef unsigned SignalId;

class PCB; /* Kernel's implementation of a user's thread */

class Thread {

public:
    void start();
    void waitToComplete();
    virtual ~Thread();

    ID getID();
    static ID getRunningId();
    static Thread *getThreadById(ID id);

    void signal(SignalId signal);

    void registerHandler(SignalId signal, SignalHandler handler);
    void unregisterAllHandlers(SignalId id);
    void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

    void blockSignal(SignalId signal);
    static void blockSignalGlobally(SignalId signal);
    void unblockSignal(SignalId signal);
    static void unblockSignalGlobally(SignalId signal);

protected:
    friend class PCB;
    Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
    virtual void run() {}

private:
    PCB* myPCB;

};

void dispatch();

#endif
