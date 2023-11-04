#ifndef project_base_MemoryAllocator_hpp
#define project_base_MemoryAllocator_hpp

#include "../lib/hw.h"

void init();

struct FreePart{
     FreePart* next;
     FreePart* previous;
     uint64 numOfBlocks;

    static FreePart* headFreePart;
};

class MemoryAllocator{
public:


    //pri inicijalizaciji programa headFreePart->next=nullptr prev=nullptr i numOfBlocks=...ceo heap

    static void* alct(size_t size){
        if (FreePart::headFreePart == nullptr) return nullptr;

        FreePart* cur = FreePart::headFreePart;
        while (cur != nullptr && size > cur->numOfBlocks) cur = cur->next;
        if (cur == nullptr) return nullptr; // ovde da se prosiri sa kompakcijom procesa na heapu, ako FreePartova ima vise od 1(nekim brojacem)


        if (size < cur->numOfBlocks){
            FreePart* temp = cur;

            temp = (FreePart*)(((uint64)temp) + (size*MEM_BLOCK_SIZE));

            temp->numOfBlocks = cur->numOfBlocks - size;
            temp->previous = cur->previous;
            temp->next = cur->next;

            if (cur->previous != nullptr) { (cur->previous)->next = temp; }
            if (cur->next != nullptr) { (cur->next)->previous = temp; }

            if (cur == FreePart::headFreePart) { FreePart::headFreePart = temp; }

        } else { // if (size == cur->numOfBlocks)
            if (cur->previous != nullptr) { (cur->previous)->next = cur->next; } // if (cur->previous->next==cur)
            if (cur->next != nullptr) { (cur->next)->previous = cur->previous; } // if (cur->next->previous==cur)

            if (cur == FreePart::headFreePart) { FreePart::headFreePart = cur->next; }
        }

        uint64* n =(uint64*)cur;
        *n = size;
        return (void*)(((uint64)cur)+16);
    }


    static int deAlct(void* addr){ // mozda treba da se inicijalizuje sve na 0/nullptr
        if (addr == nullptr) return -1;
        uint64* temp = ((uint64*)addr) - 2;
        if (temp < (uint64*)HEAP_START_ADDR || temp >= (uint64*)HEAP_END_ADDR ) return -2;

        uint64 num = *temp;  // num = number of blocks to deallocate, starting from address addr

        FreePart* cur = 0;
        if (!FreePart::headFreePart || (uint64)temp < (uint64)FreePart::headFreePart) cur = 0;
        else for (cur = FreePart::headFreePart; cur->next != 0 && (uint64)temp > (uint64)(cur->next); cur = cur->next);

        FreePart* newSeg = (FreePart*)temp;
        newSeg->numOfBlocks = num;
        newSeg->previous = cur;

        if (cur) newSeg->next = cur->next;
        else newSeg->next = FreePart::headFreePart;

        if (newSeg->next) newSeg->next->previous = newSeg;

        if (cur) cur->next = newSeg;
        else FreePart::headFreePart = newSeg;

        tryToJoin(newSeg);
        tryToJoin(cur);

        return 0;
    }

    static int tryToJoin(FreePart* cur){
        if (!cur) return -3;
        if (cur->next && (uint64)cur+(cur->numOfBlocks*MEM_BLOCK_SIZE) == (uint64)cur->next) {
            cur->numOfBlocks += cur->next->numOfBlocks;
            cur->next = cur->next->next;
            if (cur->next) cur->next->previous = cur;
            return 0;
        } else
            return -4;
    }
};

#endif