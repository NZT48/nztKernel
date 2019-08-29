#ifndef _EVENT_H_
#define _EVENT_H_

typedef unsigned char IVTNo;

#include "ivtEntry.h"

class KernelEv;

class Event {

public:
    Event(IVTNo ivtNo);
    ~Event();

    void wait();

protected:
    friend class KernelEv;

    void signal();

private:
    KernelEv* myImpl;
};

#endif
