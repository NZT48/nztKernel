#ifndef _KERNSEM_H_
#define _KERNSEM_H_

#include "defin.h"
#include "semaphor.h"
#include "pcb.h"
#include "pcbList.h"

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

    void timerUpdate(); /* Decrements timeLeft on every blocked thread */

    int val() const { return value; }

private:
    int value;
    PCBList* blocked; /* List of threads waiting on semaphore */

    static SemListElem* FirstSem; /* List of all semaphores */

    friend class Timer;
};

#endif
