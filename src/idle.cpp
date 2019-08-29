#include "idle.h"

Idle::Idle() : Thread(4096, 1) {}

void Idle::run() {

    while(1) { ; }

}
