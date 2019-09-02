#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "defin.h"
#include "pcbList.h"
#include "sigList.h"

class Thread;
class PCBList;
class SignalList;

class PCB {

public:
    enum State { NEW, READY, BLOCKED, RUNNING, FINISHED }; /* Thread states */

    PCB (Thread *thread, StackSize stackSize, Time timeSlice);
    ~PCB();

    void reschedule(); /* Change state and put back to scheduler */
    void waitToComplete(); /* Pauses calling thread until current is finished */

    static void threadWrapper(); /*  Method wrapping user thread run() */

    ID getID();
    static ID getRunningId();
    static Thread *getThreadById(ID id);

    static PCBList* pcbList; /* Global list of all PCBs */
    static ID lastId;

    ID myID;
    Time remaining, timeSlice;
    State state;

    unsigned int* stack;
    StackSize stackSize;

    Thread* myThread;
    PCBList* blockedPCB; /* List of PCBs waiting on this thread */

    int wakeSignal; /* Used for return value of wait */
    volatile unsigned sLock; /* Used for soft lock */


    Reg bp;
	Reg sp;
	Reg ss;

	/* Declarations for signals */

    void signal(SignalId signal); /* Send signal with id from 0 to 15 */

    void registerHandler(SignalId signal, SignalHandler handler);
    void unregisterAllHandlers(SignalId id);
    void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

    void blockSignal(SignalId signal);
    static void blockSignalGlobally(SignalId signal);
    void unblockSignal(SignalId signal);
    static void unblockSignalGlobally(SignalId signal);

    void serve(SignalId signal); /* Process signal */
    static void signal0(); /* Kill signal */

    PCB* parentPCB;

    SignalList *signalList; /* List of signals that PCB recieved */
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

    HandlerList handlers[16]; /* Array of lists with handlers for every signal from 0 to 15 */

};

#endif
