#include "../h/syscal.h"



void* syscall_mem_alloc(size_t size){

    asm volatile ("mv a1, a0");
    asm volatile ("li a0, 0x01");
    asm volatile ("ecall");

    void* volatile temp;
    asm volatile ("mv %0, a0" : "=r" (temp));

    return temp;

}

int syscall_mem_free(void* p){
    asm volatile ("mv a1, a0");
    asm volatile ("li a0, 0x02");
    asm volatile ("ecall");

    int volatile temp;
    asm volatile ("mv %0, a0" : "=r" (temp));

    return temp;
}

class TCB;
typedef TCB* thread_t;

int syscall_thread_create(thread_t* handle, void(*start_routine)(void*), void* arg, void* stack_space){

    asm volatile ("mv a4, a3");
    asm volatile ("mv a3, a2");
    asm volatile ("mv a2, a1");
    asm volatile ("mv a1, a0");

    asm volatile ("li a0, 0x11");
    asm volatile ("ecall");

    int volatile temp;
    asm volatile ("mv %0, a0" : "=r" (temp));

    return temp;
}

int syscall_thread_create_cpp(thread_t* handle, void(*start_routine)(void*), void* arg, void* stack_space){

    asm volatile ("mv a4, a3");
    asm volatile ("mv a3, a2");
    asm volatile ("mv a2, a1");
    asm volatile ("mv a1, a0");

    asm volatile ("li a0, 0x14");
    asm volatile ("ecall");

    int volatile temp;
    asm volatile ("mv %0, a0" : "=r" (temp));

    return temp;
}

int syscall_thread_cpp_start(TCB* t){

    asm volatile ("mv a1, a0");

    asm volatile ("li a0, 0x15");
    asm volatile ("ecall");

    int volatile temp;
    asm volatile ("mv %0, a0" : "=r" (temp));

    return temp;
}

int syscall_thread_exit(){

    asm volatile ("li a0, 0x12");
    asm volatile ("ecall");

    int volatile temp;
    asm volatile ("mv %0, a0" : "=r" (temp));

    return temp;
}

void syscall_thread_dispatch(){

    asm volatile ("li a0, 0x13");
    asm volatile ("ecall");

}

char syscall_getc(){

    asm volatile ("li a0, 0x41");
    asm volatile ("ecall");

    char volatile temp;
    asm volatile ("mv %0, a0" : "=r" (temp));

    return temp;
}

void syscall_putc(char c){

    asm volatile ("mv a1, a0");

    asm volatile ("li a0, 0x42");
    asm volatile ("ecall");

}