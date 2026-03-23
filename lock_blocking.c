#include "lock.h"

extern volatile int total_blocked_processes;

void l_init(lock_t *l) {
	l->locked = 0;
	l->blocked_queue.head = NULL;
}


void l_lock(lock_t *l) {
	__disable_irq();
	if (!l->locked) {
		l->locked = 1;
		__enable_irq();
		return;
	}
	current_process_p->blocked = 1;
	enqueue(current_process_p, &l->blocked_queue);
	total_blocked_processes++;

	__enable_irq();

	process_blocked();
}

void l_unlock(lock_t *l) {
	__disable_irq();
	l->locked = 0;

	while (!is_empty(&l->blocked_queue)) {
		process_t *removed = dequeue(&l->blocked_queue);
		removed->blocked = 0;
		enqueue(removed, &process_queue);
	}
 	total_blocked_processes--;
	__enable_irq();
}
