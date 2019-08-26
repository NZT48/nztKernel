#ifndef _PCBLIST_H_
#define _PCBLIST_H_

#include "pcb.h"
#include "thread.h"

class PCB;

class PCBList {

public:

	struct Node {
	    PCB* pcb;
	    Time timeLeft;
	    Node* next;

	    Node(PCB* p, Node* n = 0, Time t = infiniteTimeSlice) : pcb(p), next(n), timeLeft(t) {}
	};

    PCBList();
    ~PCBList();

    PCB* head() const { return front->pcb; }
    PCB* tail() const { return back->pcb; }

    unsigned int empty() { return !front ? 0 : 1; }
    void release();

    void put(PCB* pcb);
    void PriorPut(PCB* p, Time maxTimeToWait);
    PCB* get();

    Node* front;
    Node* back;

    friend void timerUpdate();

};

#endif
