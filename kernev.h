#ifndef _kernev_h_
#define _kernev_h_

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
    PCB* myPCB;
    volatile int value;
    Event* myEvent;
};


#endif
