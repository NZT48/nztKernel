#include "idle.h"
#include "thread.h"
#include "pcb.h"

Idle::Idle() : Thread(4096, 1) {}

void Idle::run() {

    while(1) { ; }

}
