#ifndef _kernsem_h_
#define _kernsem_h_

#include "defin.h"
#include "semaphor.h"
#include "pcb.h"
#include "pcblist.h"

class KernelSem {

public:
    KernelSem(int init) : value(init) { blocked = new PCBList(); }
    ~KernelSem();

    int wait(Time maxTimeToWait);
    int signal(int n);

    int val() const { return value; }
    void incVal() {
        value++;
    }

private:
    int value;
    PCBList* blocked;
    //static WaitList* waitList;

};

#endif
