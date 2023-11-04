#ifndef project_base_tcb_hpp
#define project_base_tcb_hpp

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "../h/syscall_c.h"


// Thread Control Block
class TCB
{
public:
    ~TCB() {
        if(this->tcb_id!=0)
        delete stack; }
   // ~TCB() { delete stack; }
   void *operator new(size_t n){
        n = n + 16;
        uint64 volatile numOfBlocks = n/MEM_BLOCK_SIZE;
        numOfBlocks += n%MEM_BLOCK_SIZE?1:0;
        return MemoryAllocator::alct(numOfBlocks);}

    //void operator delete(void *p){ // mora preko sistemskog poziva delete
      //  MemoryAllocator::deAlct(((TCB*)p)->stack);
    //    mem_free(p); }


    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    using Body = void(*)(void*);

    static TCB *running;

    static int getTcb_id_cnt() { return tcb_id_cnt; }

private:
    TCB(Body body, uint64* stack_space, void* arg, int i)
    {
        this->body = body;
        this->stack = stack_space;
        this->finished=false;
        this->tcb_id=tcb_id_cnt++;
        this->arg=arg;

        if(body!= nullptr) {context={(uint64) &threadWrapper,(uint64) &stack[DEFAULT_STACK_SIZE]};}
        else {context={0,0};}

        if (body != nullptr) { if(i==1) Scheduler::put(this); }
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64 *stack;
    Context context;

    bool finished;
    int tcb_id;
    void* arg;

    friend class Riscv;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static void dispatchThreadExit();

    static int tcb_id_cnt;



};


#endif