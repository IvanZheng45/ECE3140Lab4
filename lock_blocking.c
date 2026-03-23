#include "lock.h"

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

	__enable_irq();

	process_blocked();
}

void l_unlock(lock_t *l) {
	__disable_irq();

	if (is_empty(&l->blocked_queue)) {
		l->locked = 0;
	} else {

		process_t *removed = dequeue(&l->blocked_queue);
		removed->blocked = 0;
		enqueue(removed, &process_queue);
	}

	__enable_irq();
}
