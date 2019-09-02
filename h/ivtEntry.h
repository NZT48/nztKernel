#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_

#include "event.h"
#include "kernev.h"

typedef void interrupt (*pInterrupt)(...);

class KernelEv;

class IVTEntry {

public:
    IVTEntry(IVTNo ivtNo_, pInterrupt newRoutine1 );
    ~IVTEntry();

    void setEvent(KernelEv *event_);
    void signal();
    void callOldRoutine();
    void restore(); /* Restore the old routine at the end */

    static IVTEntry* ivtEntries[256];

private:

    IVTNo ivtNo;
    KernelEv *event;

    pInterrupt oldRoutine;
    pInterrupt newRoutine;
};

/* Macro used for creating IVTEntry object and setting up new interrupt in IV Table */

#define PREPAREENTRY(num,callOldRout)                                    \
		void interrupt newRout##num(...);                                \
		IVTEntry ivtEntry##num(num, newRout##num);                       \
		void interrupt newRout##num(...) {                               \
            if(IVTEntry::ivtEntries[num])                                \
				ivtEntry##num.signal();                					 \
			if (callOldRout == 1 && IVTEntry::ivtEntries[num])           \
				ivtEntry##num.callOldRoutine();            				 \
			dispatch();													 \
		}

#endif
