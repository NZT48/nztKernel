#ifndef _kernsem_h_
#define _kernsem_h_

#include "defin.h"
#include "semaphore.h"
#include "pcb.h"

class KernelSem {

public:
    KernelSem(int init) : value(init) {};
    ~KernelSem();

    int wait(Time maxTimeToWait);
    int signal(int n);

    int val() const { return value; }
    void incVal() {
        value++;
    }

private:
    int value;

};

#endif
