#ifndef project_base_scheduler_hpp
#define project_base_scheduler_hpp

#include "list.hpp"

class TCB;

class Scheduler
{

public:
    static TCB *get();

    static void put(TCB *ccb);

    static List<TCB> readyThreadQueue; // treba private da bude mozda
};

#endif