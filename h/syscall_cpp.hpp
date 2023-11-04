#ifndef _syscall_cpp
#define _syscall_cpp

#include "syscall_c.h"

void* operator new (size_t);
void operator delete (void*);

class Thread{
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread(){if (myHandle!= nullptr)delete myHandle;}

    int start ();

    static void dispatch ();
    // static int sleep (time_t);



protected:
    Thread();
    virtual void run () {}
    static void runWr(void* t);

private:
    thread_t myHandle;

};

class Console {
public:
    static char getc();
    static void putc(char);
};
#endif