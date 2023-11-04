#include "../h/riscv.hpp"



TCB *TCB::running = nullptr;

int TCB::tcb_id_cnt=0;

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished()) { Scheduler::put(old); }

    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::dispatchThreadExit()
{
    running->setFinished(true); // ne znam da li treba
    running = Scheduler::get();

    uint64* volatile temp = (uint64*)(&running->context);
    asm volatile("mv a0, %0" : :"r" (temp));
    asm volatile("ld ra, 0 * 8(a0)");
    asm volatile("ld sp, 1 * 8(a0)");
    asm volatile("ret");
}

void TCB::threadWrapper()
{

    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    thread_dispatch();
}