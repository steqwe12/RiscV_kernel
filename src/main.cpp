#include "../h/riscv.hpp"


extern void userMain();

int main()
{
    
    init();
    TCB* th;

    thread_create(&th, nullptr, nullptr);
    TCB::running=th;

    userMain();

    delete th;

    return 0;
}