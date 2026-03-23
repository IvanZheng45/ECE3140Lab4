#include "lock.h"

extern volatile int total_blocked_processes;

void l_init(lock_t* l) {
    l->locked = 0;
}

void l_lock(lock_t* l) {
    while (1) {
        __disable_irq();
        if (l->locked == 0) {
            l->locked = 1;
            __enable_irq();
            return;
        }
        total_blocked_processes++;
        __enable_irq();
    }
}

void l_unlock(lock_t* l) {
    __disable_irq();
    l->locked = 0;
    total_blocked_processes--;
    __enable_irq();
}