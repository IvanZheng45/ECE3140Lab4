/*
 * This test program creates multiple tasks that
 * attempt to acquire the same lock and toggle the red and green LEDs. The expected
 * behavior is that the tasks will acquire the lock one at a time, ensuring that the
 * LED toggling is not interleaved. This also checks the implementation of the blocking lock,
 * making sure that multiple blocked processes are properly put to sleep and unblocked when the lock becomes available.
 */

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
    blue_on();
    while(1);
}
