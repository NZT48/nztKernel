#ifndef _PCBLIST_H_
#define _PCBLIST_H_

#include "pcb.h"

class PCB;

class PCBList {
public:

    struct Node {
        PCB* pcb;
        Node* next;
        Node(PCB* pcb, Node* next = 0) : pcb(pcb), next(next) {}
    };

    PCBList();
    ~PCBList();

    PCB* head() const { return front->pcb; }
    PCB* tail() const { return back->pcb; }

    unsigned int empty() { return !front ? 0 : 1; }
    void release();

    void put(PCB* pcb);
    PCB* get();

    Node* front;
    Node* back;

};

#endif
