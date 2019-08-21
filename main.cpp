#include "timer.h"
#include "thread.h"
#include "mainTh.h"
#include <iostream.h>
#include "pcb.h"

int main(int argc, char* argv[]){

    Timer::mainPCB = new PCB(0, defaultStackSize, 20);
    Timer::runningPCB = Timer::mainPCB;
    Idle* idle = new Idle();

    MainThread *mainThread = new MainThread(argc, argv);

	idle->start();
    mainThread->start();

    Timer::inic();
    dispatch();
    Timer::restore();

    return 0;
}
