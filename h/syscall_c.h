#ifndef project_base_syscall_c_hpp
#define project_base_syscall_c_hpp

//#pragma once

#include "../lib/hw.h"
#include "syscal.h"
#include "tcb.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void* mem_alloc(size_t size);

int mem_free(void*);

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);

int thread_create_cpp(thread_t* handle, void(*start_routine)(void*), void* arg);

int thread_cpp_start(TCB* t);

int thread_exit();

void thread_dispatch();

char getc();

void putc(char);

#ifdef __cplusplus
}
#endif

#endif