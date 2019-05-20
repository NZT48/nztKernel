#ifndef _PCBLIST_H_
#define _PCBLIST_H_

class PCB;

class PCBList {
public:
    PCBList();
    ~PCBList();

    PCB* front() const { return front->pcb; }
    PCB* back() const { return back->pcb; }
    unsigned int empty() { !front ? 0 : 1; }

    void pushBack(PCB* pcb);
    PCB* popFront();

    void rescheduleAll();

private:
    struct Node {
        PCB* pcb;
        Node* next;
        Node(PCB* pcb, Node* next = NULL) : pcb(pcb), next(next) {}
    };

    Node* front;
    Node* back;

};

#endif