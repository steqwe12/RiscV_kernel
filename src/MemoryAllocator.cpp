#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"

FreePart *FreePart::headFreePart = nullptr;

void init(){
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    volatile void *p = (void *)HEAP_START_ADDR;
    FreePart::headFreePart=(FreePart*)p;
    FreePart::headFreePart->next = nullptr;
    FreePart::headFreePart->previous = nullptr;
    FreePart::headFreePart->numOfBlocks =((size_t)HEAP_END_ADDR-(size_t)HEAP_START_ADDR) /MEM_BLOCK_SIZE;

   // Riscv::mc_sie(Riscv::SSTATUS_SIE);
    //  Riscv::ms_sstatus(Riscv::SSTATUS_SIE); // bitno
}