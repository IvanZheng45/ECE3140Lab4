/* test_2.c
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

lock_t w;
lock_t r;

unsigned int nr= 0;

void p1 (){
	int i;
	for (i=0; i < 10; i++) {
		delay(300);
		green_toggle_frdm();
	}
}

void reader (void) {
  /* enter */
	l_lock(&r);
	
	nr++;
	if (nr == 1) {
	  l_lock (&w);	
	}
	
	l_unlock (&r);
	
	/*start reading*/
	p1();
	/*end reading*/

  /* exit */
	l_lock(&r);

	nr--;
	if (nr == 0) {
	   l_unlock (&w);
	}

	l_unlock (&r);
}

void writer (void) {
  /* enter */	
	l_lock(&w);
	
	/*start writing*/
	delay(300);
	red_toggle_frdm();
	delay(300);
	red_toggle_frdm();
	/*end writing*/

  /* exit */			
	l_unlock(&w);
}



int main (void){		
	led_init();           /* Initialize the LEDs          */
	l_init (&w);
	l_init (&r);
 
	if (process_create (writer,20) < 0) {
	 	return -1;
	}
	if (process_create (reader,20) < 0) {
	 	return -1;
	}
	if (process_create (reader,20) < 0) {
	 	return -1;
	}
	if (process_create (reader,20) < 0) {
	 	return -1;
	}
	if (process_create (writer,20) < 0) {
	 	return -1;
	}
	
	process_start();
 
	blue_on();

	while(1);
}

