#ifndef _idle_h_
#define _idle_h_

#include "pcb.h"
#include "thread.h"

class Idle : public Thread {

public:
    Idle();

    virtual void run();

};

#endif _idle_h_
