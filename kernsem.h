#ifndef _kernsem_h_
#define _kernsem_h_

#include "defin.h"
#include "semaphor.h"
#include "pcb.h"
#include "pcblist.h"

class KernelSem {

public:

	struct SemListElem {
	    KernelSem* sem;
	    SemListElem* next;

	    SemListElem(KernelSem* s = 0, SemListElem* n = 0) : sem(s), next(n) {}
	};

    KernelSem(int init = 1);
    ~KernelSem();

    int wait(Time maxTimeToWait);
    int signal(int n);

    void timerUpdate();

    int val() const { return value; }

private:
    int value;
    PCBList* blocked;

    static SemListElem* FirstSem;

    friend class Timer;
};

#endif
