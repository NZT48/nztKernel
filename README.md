# nztKernel
### Project for course Operating Systems 1, School of Electrical Engineering, University of Belgrade

An implementation of small preemptive operating system kernel on 8086 processor.

### Useful links:
-------------
* [Operating systems class website](http://os.etf.bg.ac.rs/)

### Structure of project:

```
.
├── h                       # headers (.h files)
│   ├── defin.h
│   ├── event.h
│   ├── idle.h
│   ├── ivtEntry.h
│   ├── kernev.h
│   ├── kernsem.h
│   ├── mainTh.h
│   ├── pcb.h
│   ├── pcblist.h
│   ├── schedule.h
│   ├── semaphor.h
│   ├── sigList.h
│   ├── thread.h
│   └── timer.h
├── lib                     # libraries
│   └── applicat.lib
├── src                     # sources (.cpp files)
│   ├── defin.cpp
│   ├── event.cpp
│   ├── idle.cpp
│   ├── ivtEntry.cpp
│   ├── kernev.cpp
│   ├── kernsem.cpp
│   ├── main.cpp
│   ├── mainTh.cpp
│   ├── pcb.cpp
│   ├── pcblist.cpp
│   ├── semaphor.cpp
│   ├── sigList.cpp
│   ├── thread.cpp
│   └── timer.cpp
├── test                    # public test
│   ├── bounded.cpp
│   ├── bounded.h
│   ├── intLock.cpp
│   ├── intLock.h
│   ├── keyevent.cpp
│   ├── keyevent.h
│   ├── user.cpp
│   └── user.h
├── Projektni zadatak 2019.pdf
└── README.md

```
