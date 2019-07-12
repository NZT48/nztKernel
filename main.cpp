#include "timer.h"
#include "thread.h"
#include "mainTh.h"
#include <iostream.h>
#include "pcb.h"

int main(int argc, char* argv[]){

    Timer::mainPCB = new PCB();
    Timer::runningPCB = Timer::mainPCB;

    MainThread *mainThread = new MainThread(argc, argv);
    mainThread->start();

    Timer::inic();
    dispatch();
    Timer::restore();

    return 0;
}
