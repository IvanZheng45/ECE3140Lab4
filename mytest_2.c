'''
/*
 * This test program creates multiple tasks that
 * attempt to acquire the same lock and toggle the red and green LEDs. The expected
 * behavior is that the tasks will acquire the lock one at a time, ensuring that the
 * LED toggling is not interleaved. This also checks the implementation of the blocking lock,
 * making sure that a blocked process is properly put to sleep and unblocked when the lock becomes available.
 */
'''

#include "3140_concur.h"
#include "led.h"
#include "lock.h"

lock_t relay_lock;

void task_red(void) {
    l_lock(&relay_lock);
    for(int i=0; i<3; i++) {
        red_toggle_frdm();
        delay(500);
    }
    red_off();
    l_unlock(&relay_lock);
}

void task_green(void) {
    l_lock(&relay_lock);
    for(int i=0; i<3; i++) {
        green_toggle_frdm();
        delay(500);
    }
    green_off();
    l_unlock(&relay_lock);
}

int main(void) {
    led_init();
    l_init(&relay_lock);

    process_create(task_red, 20);
    process_create(task_green, 20);
    process_create(task_red, 20); 

    process_start();
    while(1);
}