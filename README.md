# Operating-system-kernel

OS kernel is implemented for **RISC V** architecture and is ran in qemu emulator. No external libraries are used, meaning that it's not allowed to use any host's operating system system calls (threads, memory allocation, semaphores, interrupts etc. ) and everything is written from scratch.

---

The kernel provides three interfaces to the user program that are organized in layers.

![Layers](https://github.com/steqwe12/os-projekat/assets/124030239/6623f620-6813-4c9d-a531-8fa0818e7699)

Code for all the layers is being run in a unique address space. Kernel code and code with hardware access runs in privileged mode(system mode), while everything under the dark line runs in user mode.

ABI(application binary interface) is an binary interface for system calls, which are called by software interrupts. This layer provides sending arguments to system calls thru processor registers, changing mode from system to user and going into kernel code.

C API(application programming interface) is a classic, procedural programming interface for system calls implemented as C functions. These functions act as a wrapper for ABI interface.

C++ API is an object oriented API which serves as a wrapper for functions from C API.

| Number | Function                                                                                                                 | Description                                                                                                                                                                                                                                                                    |
| :----- | :----------------------------------------------------------------------------------------------------------------------- | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 0x01   | `void* mem_alloc(size_t size);`                                                                                          | Alocates size bytes of memory, rounded and aligned on block of size MEM_BLOCK_SIZE. Returns a pointer to the memory block if successfull, or null if not.                                                                                                                      |
| 0x02   | `int mem_free(void*);`                                                                                                   | Free's memory previously alocated with mem_alloc. Return's 0 in case of success or else, negative value (error code). Argument must be a returned value of mem_alloc. If that's not the case, action is undefined: kernel might return an error or do something unpredictable. |
| 0x11   | `class _thread; typedef _thread* thread_t; int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);` | Starts a thread of function start_routine, calling it with argument arg. In case of success, \*handle will contain the handle for the thread and return value will be 0, or else a negative value. "Handle" is used to indentify threads.                                      |
| 0x12   | `int thread_exit(); `                                                                                                    | Turns off the active thread. In case of an error, return a negative value.                                                                                                                                                                                                     |
| 0x13   | `void thread_dispach();`                                                                                                 | Potentially takes the processor from active thread and gives it to some other thread.                                                                                                                                                                                          |


> In short, the kernel provides:

- Synchronous context change
- Two modes, system and user mode
- Interrupt handling
- Fully implemented threads
- Memory allocation and deallocation
