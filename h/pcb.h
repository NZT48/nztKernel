#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "defin.h"
#include "pcblist.h"
#include "sigList.h"

class Thread;
class PCBList;
class SignalList;

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

    unsigned int* stack;
    StackSize stackSize;

    Thread* myThread;
    PCBList* blockedPCB;

    int wakeSignal;

    Reg bp;
	Reg sp;
	Reg ss;

public:
    void signal(SignalId signal);

    void registerHandler(SignalId signal, SignalHandler handler);
    void unregisterAllHandlers(SignalId id);
    void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

    void blockSignal(SignalId signal);
    static void blockSignalGlobally(SignalId signal);
    void unblockSignal(SignalId signal);
    static void unblockSignalGlobally(SignalId signal);

    void serve(SignalId signal);
    static void signal0();

    PCB* parentPCB;
    volatile unsigned sLock;

    SignalList *signalList;
    volatile unsigned blockedSignals[16];
    static volatile int globallyBlockedSignals[16];

    class HandlerList {

    public:

    	struct HandlerElem {
            SignalHandler handler;
    	    HandlerElem* next;

    	    HandlerElem(SignalHandler hand, HandlerElem* n = 0) : handler(hand), next(n) {}
    	};

        HandlerList(): head(0), tail(0) {}
        ~HandlerList();

        void put(SignalHandler hand);
        void removeAll();

        HandlerElem* head;
        HandlerElem* tail;

    };

    HandlerList handlers[16];

};

#endif
