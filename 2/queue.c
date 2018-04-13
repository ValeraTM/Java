#include "queue.h"
#include <stdlib.h>

struct queue * create_queue () {
	struct queue * head = (struct queue *)calloc(1, sizeof(struct queue));
	return head;
}

void add_queue (struct queue * head, int num) {
	struct queue * p = (struct queue *)calloc(1, sizeof(struct queue));
	struct queue * q = head;
	while (q->next != NULL) {
		q = q->next;
	}
	q->next = p;
	p->id = num;
}

struct queue * remove_queue (struct queue * head) {
	struct queue * p = head;
	head = head->next;
	free(p);
	return head;
}
