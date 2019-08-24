#include "event.h"
#include "defin.h"
#include "kernev.h"

Event::Event(IVTNo ivtNo) {
    myImpl = new KernelEv(ivtNo, this);
}

Event::~Event() {
    delete (myImpl);
}

void Event::wait() {
    myImpl->wait();
}

void Event::signal() {
    myImpl->signal();
}
