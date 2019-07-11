#include "timer.h"
#include "thread.h"
#include "mainTh.h"
#include <iostream.h>
#include "pcb.h"


PCB *mainPCB;

int main(int argc, char* argv[]){

    mainPCB = new PCB();
    PCB::runningPCB = mainPCB;
    MainThread *mainThread = new MainThread(argc, argv);
    mainThread->start();
    Timer::inic();
    dispatch();
    Timer::restore();
    return 0;
}
