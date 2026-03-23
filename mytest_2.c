/*
 * This task attempts to test the two lock's response to deadlocking
 * by allowing two tasks  to grab two different locks in 
 * opposite orders. Task Red  holds Lock A and waits for B, 
 * while Task Green holds Lock B and waits for A.
 * We are checking if the blocking lock implementation and the spining
 * implementation are both vulnerable to deadlocks and lets the proceses
 * "hang" indefinitely. If the blue LED is off, then the processes are not hanging 
 * and that means the processes exited early instead of hanging
 *  */

#include "3140_concur.h"
#include "led.h"
#include "lock.h"

lock_t lock_a;
lock_t lock_b;
void task_red(void) {
    l_lock(&lock_a);
    for(int i=0; i<3; i++) {
        red_toggle_frdm();
        delay(500);
    }
    l_lock(&lock_b);
    red_off_frdm();
    l_unlock(&lock_a);
    l_unlock(&lock_b);
}

void task_green(void) {
    l_lock(&lock_b);
    for(int i=0; i<3; i++) {
        green_toggle_frdm();
        delay(500);
    }
    l_lock(&lock_a);
    green_off_frdm();
    l_unlock(&lock_b);
    l_unlock(&lock_a);
}

int main(void) {
    led_init();
    l_init(&lock_a);
    l_init(&lock_b);

    blue_on();
    process_create(task_red, 20);
    process_create(task_green, 20);
    process_start();
    // should never be off
    blue_off();
    while(1);
}
