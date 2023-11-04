#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.h"


Thread::Thread(void (*body)(void *), void *arg) {
    thread_create_cpp(&myHandle, body, arg);
}

Thread::Thread() {
    thread_create_cpp(&myHandle, Thread::runWr, this);
}

void Thread::dispatch() {thread_dispatch();}

int Thread::start() {

   return thread_cpp_start(this->myHandle);
}

void Thread::runWr(void* t) {((Thread*)t)->run();}


char Console::getc() {return ::getc();}

void Console::putc(char c) { ::putc(c);}