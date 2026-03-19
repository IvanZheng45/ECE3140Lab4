/* test_1.c
 *
 * Nils Napp
 * Cornell University
 * All right reserved
 *
 * Jan 2024
 * Ithaca NY
 *
 * This file is part of the ECE3140/CS3420 offering for Spring 2024. If you
 * are not part of this class you should not have access to this file. Do not
 * post, share, or otherwise distribute this file. We will consider it an AI
 * violation if you do. If you somehow get this code and you are NOT enrolled
 * the Spring 2024 version of ECE3140 please contact the course staff
 * immediately and describe how you found it.
 */

#include "3140_concur.h"
#include "led.h"
#include "lock.h"


lock_t l;

/*How often and at what frequency should the LEDs blink? */
void p1(void){
	int i= 0;
	while(i < 3){
		/*NCS*/
		delay(300);
		i++;
		/*CS*/
		l_lock(&l);
		red_toggle_frdm();
		delay(300);
		red_toggle_frdm();
		delay(300);
		green_toggle_frdm();
		delay(300);
		green_toggle_frdm();
		delay(300);
		l_unlock(&l);
	}
}

int main(void){
	led_init();

	l_init(&l);
	
	if (process_create (p1,20) < 0) {
	 	return -1;
	}
	if (process_create (p1,20) < 0) {
	 	return -1;
	}
	
	process_start();
	green_on_frdm();

	while(1);
	return 0;	
}
