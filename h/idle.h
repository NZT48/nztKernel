#ifndef _IDLE_H_
#define _IDLE_H_

#include "thread.h"

/* Idle - Empty loop thread, used when all other threads are blocked or waiting */

class Idle : public Thread {

public:
    Idle();

    virtual void run();

};

#endif
