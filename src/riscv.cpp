#include "../h/riscv.hpp"
#include "../test/printing.hpp"

using Body = void(*)(void*);

void Riscv::popSppSpie()  // ovo je sve u prek rutini, treba da se podesi i sstatus  da se "vraca" u korisnicki rezim
{
    Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
    asm volatile("csrw sepc, ra");
    asm volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    uint64 volatile arg1;
    uint64 volatile arg2;
    uint64 volatile arg3;
    uint64 volatile arg4;
    uint64 volatile arg5;
    asm volatile("mv %0, a0" : "=r" (arg1));
    asm volatile("mv %0, a1" : "=r" (arg2));
    asm volatile("mv %0, a2" : "=r" (arg3));
    asm volatile("mv %0, a3" : "=r" (arg4));
    asm volatile("mv %0, a4" : "=r" (arg5));

    uint64 scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();
        switch (arg1){
            case 0x01: {    // syscall_mem_alloc

                void* volatile temp;
                temp = MemoryAllocator::alct((size_t)arg2);           // mozda treba da se kastuje u uint64 kao ovo dole
                asm volatile("mv a0, %0" : :"r" (temp));  // treba da se upise rezultat u a0; a0(return value) is pointer to allocated part of memory(heap)(nullptr if fail)

                asm volatile("sd a0, 80(fp)");

            } break;

            case 0x02: {    // syscall_mem_free

                int volatile temp = MemoryAllocator::deAlct((void*)arg2);       // kako se ovo tacno prosledjuje za podatak SIGNED 32b(int)
                asm volatile("mv a0, %0" : :"r" (temp));

                asm volatile("sd a0, 80(fp)");

            } break;

            case 0x11: { // thread_create

                thread_t *handle = (thread_t *)arg2;

                Body body=(Body)arg3;

                void* volatile arg=(void*)arg4;

                uint64* stack_space=(uint64*)arg5;

                *handle = new TCB(body, stack_space, arg, 1);

               // uint64 volatile temp=0;
               // asm volatile("mv a0, %0" : :"r" (temp));
               asm volatile("mv a0, %0" : :"r" (0x0));
               asm volatile("sd a0, 80(fp)");

            } break;

            case 0x12: {  // thread_exit
                // vrv ako je neki main thread ili poslednji threead u scheduleru(trebalo bi da je to main thread) vraca neg integer
                if (TCB::running->getTcb_id_cnt()==0) { // ako je neka glavna nit ne sme da se ugasi
                    int volatile temp = -5;
                    asm volatile("mv a0, %0" : :"r" (temp));
                } else {
                   // delete TCB::running; mem dealct mora
                    TCB::dispatchThreadExit();
                }
               // specijalni TCB::dispatch();  koji postavlja running = Scheduler::get(); i stavlja kontekst sp, ra na tu nit

                asm volatile("sd a0, 80(fp)");

            } break;

            case 0x13: { // context switch, a0=0x13, dispatch();

                TCB::dispatch();

            } break;

            case 0x14: { // thread_create_cpp

                thread_t *handle = (thread_t *)arg2;

                Body body=(Body)arg3;

                void* volatile arg=(void*)arg4;

                uint64* stack_space=(uint64*)arg5;

                *handle = new TCB(body, stack_space, arg, 2);

                // uint64 volatile temp=0;
                // asm volatile("mv a0, %0" : :"r" (temp));
                asm volatile("mv a0, %0" : :"r" (0x0));
                asm volatile("sd a0, 80(fp)");

            } break;

            case 0x15: { // scheduler_put_cpp(.start)

                TCB* temp = (TCB*)arg2;
                Scheduler::put(temp);

                asm volatile("mv a0, %0" : :"r" (0x0));
                asm volatile("sd a0, 80(fp)");

            } break;

            case 0x41: {
                char volatile c = __getc();
                asm volatile("mv a0, %0" : :"r" (c));
                asm volatile("sd a0, 80(fp)");
            } break;

            case 0x42: {
                __putc((char)arg2);
            } break;

        }
        w_sepc(sepc);
        w_sstatus(sstatus);
       // mc_sip(SIP_SSIP);
    }
    else if (scause == 0x8000000000000001UL) // timer
    {
        mc_sip(SIP_SSIP);
    }
    else if (scause == 0x8000000000000009UL) // console
    {
        console_handler();
    }
    else
    {
        uint64 scause = r_scause();
        uint64 sepc = r_sepc();

        printString("scause: ");
        printInt(scause);

        printString("  sepc: ");
        printInt(sepc);

    }
}