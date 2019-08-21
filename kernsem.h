#ifndef _kernsem_h_
#define _kernsem_h_

#include "defin.h"
#include "semaphor.h"
#include "pcb.h"
#include "pcblist.h"
#include "waitlist.h"

class KernelSem;

struct SemListElem {
    KernelSem* sem;
    SemListElem* next;

    SemListElem(KernelSem* s = 0, SemListElem* n = 0);
};


class KernelSem {

public:
    KernelSem(int init);
    ~KernelSem();

    int wait(Time maxTimeToWait);
    int signal(int n);

    void timerUpdate();

    int val() const { return value; }
    void incVal() {
        value++;
    }

private:

    int value;
    int valueIncExp;
    PCBList* blocked;

    static SemListElem* FirstSem;

    friend class WaitList;
    friend class Timer;

};

#endif
