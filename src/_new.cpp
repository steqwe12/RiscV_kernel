// #include "../lib/mem.h"
#include "../h/syscall_c.h"


void *operator new(size_t n)
{
    //return __mem_alloc(n);
    return mem_alloc(n);
}

void *operator new[](size_t n)
{
    // return __mem_alloc(n);
   return mem_alloc(n);
}

void operator delete(void *p) noexcept
{
   // __mem_free(p);
   mem_free(p);
}

void operator delete[](void *p) noexcept
{
   // __mem_free(p);
    mem_free(p);
}