#ifndef _SEMAPHOR_H_
#define _SEMAPHOR_H_

typedef unsigned int Time;

class KernelSem;

class Semaphore {

public:
    Semaphore(int init = 1);
    virtual ~Semaphore();

    virtual int wait(Time maxTimeToWait); /* maxTimeToWait - after that time thread will be unblocked */
    virtual int signal(int n = 0); /* Signal(n) will unblock n threads or less if there is less threads blocked */

    int val() const;

private:
    KernelSem* myImpl;
};


#endif
