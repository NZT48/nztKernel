#ifndef _SIGLIST_H_
#define _SIGLIST_H_

#include "defin.h"
#include "thread.h"
#include "pcb.h"

class SignalList {

public:

	struct SignalElem {
	    SignalId id;
	    SignalElem* next;

	    SignalElem(SignalId sid, SignalElem* n = 0) : id(sid), next(n) {}
	};

    SignalList(): head(0), tail(0) {}
    ~SignalList();

    void put(SignalId id);
    SignalId get(); /* Get the first in the list */
    void remove(SignalElem* e);

    SignalElem* head;
    SignalElem* tail;

};

#endif
