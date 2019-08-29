#ifndef _KERNEV_H_
#define _KERNEV_H_

#include "event.h"
#include "pcb.h"

class PCB;
class Event;

class KernelEv {

public:
    KernelEv(IVTNo id, Event* myEvent);
    ~KernelEv();

    void wait();
    void signal();

private:
    IVTNo id;
    volatile int value;

    PCB* myPCB;
    Event* myEvent;
};

#endif
