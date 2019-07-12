#include "mainTh.h"
#include "timer.h"
#include "SCHEDULE.H"

extern int userMain(int, char*[]);
extern PCB *mainPCB;

MainThread::MainThread(int argc_, char *argv_[]): Thread(4096,0) {
    argc = argc_;
    argv = argv_;
}

MainThread::~MainThread() {
    waitToComplete();
}

void MainThread::run(){
    userMain(argc,argv);
    Scheduler::put(Timer::mainPCB);
}
