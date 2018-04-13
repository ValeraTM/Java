#ifndef QUEUE_H
#define QUEUE_H

struct queue {
	int id;
	struct queue * next;
};

struct queue * create_queue ();
void add_queue (struct queue * head, int num);
struct queue * remove_queue (struct queue * head);

#endif