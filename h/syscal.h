#ifndef project_base_syscal_hpp
#define project_base_syscal_hpp

#include "../lib/hw.h"

void* syscall_mem_alloc(size_t size);

int syscall_mem_free(void* p);

class TCB;
typedef TCB* thread_t;

int syscall_thread_create(thread_t* handle, void(*start_routine)(void*), void* arg, void* stack_space);

int syscall_thread_create_cpp(thread_t* handle, void(*start_routine)(void*), void* arg, void* stack_space);

int syscall_thread_cpp_start(TCB*);

int syscall_thread_exit();

void syscall_thread_dispatch();

char syscall_getc();

void syscall_putc(char);

#endif