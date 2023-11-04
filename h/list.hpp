#ifndef project_base_list_hpp
#define project_base_list_hpp

#include "MemoryAllocator.hpp"
#include "../lib/mem.h"

template<typename T>
class List
{
private:
    struct Elem
    {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}

        void *operator new(size_t n){
            n = n + 16;
            uint64 volatile numOfBlocks = n/MEM_BLOCK_SIZE;
            numOfBlocks += n%MEM_BLOCK_SIZE?1:0;
            return MemoryAllocator::alct(numOfBlocks);

           // return __mem_alloc(n);
       }

        void operator delete(void *p) {
            MemoryAllocator::deAlct(p);
           // __mem_free(p);
        }

        ~Elem(){}
    };

    Elem *head, *tail;

public:
    List() : head(0), tail(0) {}

    List(const List<T> &) = delete;

    List<T> &operator=(const List<T> &) = delete;

    void addLast(T *data)
    {
        Elem *elem = new Elem(data, 0);
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    T *removeFirst()
    {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        T *ret = elem->data;
        delete elem;
        return ret;
    }
};

#endif