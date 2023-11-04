#include "../h/syscall_c.h"



void* mem_alloc(size_t size){
    size = size + 16;
    uint64 volatile numOfBlocks = size/MEM_BLOCK_SIZE;
    numOfBlocks += size%MEM_BLOCK_SIZE?1:0;
    return syscall_mem_alloc(numOfBlocks);

}

int mem_free(void* p){
    return syscall_mem_free(p);
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){

    void* volatile stack_space = nullptr;

    if(TCB::getTcb_id_cnt()!=0)
        stack_space = mem_alloc(sizeof(uint64)*DEFAULT_STACK_SIZE);
       // stack_space = __mem_alloc(sizeof(uint64)*DEFAULT_STACK_SIZE);
    if (stack_space== nullptr && TCB::getTcb_id_cnt()>0) return -3;

    int volatile a = syscall_thread_create(handle, start_routine, arg, stack_space);
    if (*handle == nullptr) return -4;

    return a;
}

int thread_create_cpp(thread_t* handle, void(*start_routine)(void*), void* arg){

    void* volatile stack_space = nullptr;

    if(TCB::getTcb_id_cnt()!=0)
        stack_space = mem_alloc(sizeof(uint64)*DEFAULT_STACK_SIZE);
        // stack_space = __mem_alloc(sizeof(uint64)*DEFAULT_STACK_SIZE);
    if (stack_space== nullptr && TCB::getTcb_id_cnt()>0) return -3;

    int volatile a = syscall_thread_create_cpp(handle, start_routine, arg, stack_space);
    if (*handle == nullptr) return -4;

    return a;
}

int thread_cpp_start(TCB* t){
    syscall_thread_cpp_start(t);
    return 0;
}

int thread_exit(){
    return syscall_thread_exit();
}

void thread_dispatch(){
    syscall_thread_dispatch();
}

char getc(){
   return syscall_getc();
}

void putc(char c){
    syscall_putc(c);
}
