'''
/*
 * The difference between a spinning lock and a blocking lock should be obvious from the code. 
 * The spinning lock continuously checks if the lock is available, while the blocking lock puts 
 * the process to sleep until the lock is available. In a spinning lock, the process will consume 
 * CPU cycles while waiting for the lock. In this test, the high priority worker will keep trying 
 * to acquire the lock, while the low priority holder will hold the lock for a long time. 
 * With a spinning lock, the high priority worker will consume CPU cycles while waiting, 
 * but with a blocking lock, the high priority worker will be put to sleep until the low priority holder releases the lock,
 * making sure that the process_blocked() is used properly
*/
''' 

#include "3140_concur.h"
#include "led.h"
#include "lock.h"

lock_t resource_lock;

void high_priority_worker(void) {
    for (int i = 0; i < 5; i++) {
        l_lock(&resource_lock);
        red_toggle_frdm(); 
        delay(100);
        red_toggle_frdm();
        l_unlock(&resource_lock);
        delay(50); 
    }
}

void low_priority_holder(void) {
    l_lock(&resource_lock);
    green_toggle_frdm();
    delay(2000);
    green_toggle_frdm();
    l_unlock(&resource_lock);
}

int main(void) {
    led_init();
    l_init(&resource_lock);

    process_create(low_priority_holder, 20); 
    process_create(high_priority_worker, 20); 

    process_start();
    while(1);
}