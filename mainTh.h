#ifndef _MAINTH_H_
#define _MAINTH_H_

class MainThread : public Thread {
public:
    int argc;
    char **argv;

private:
    MaintThread(int argc_, char **argv_);
    ~MainThread();
    void run();

};

#endif
