#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "queue.h"
#include "hashtable.h"

#define MAX 1000000

void create_link (struct node * p1, struct node * p2, int capacity) {
	struct list * new = (struct list *)calloc(1, sizeof(struct list));
	new->next = p1->siblings;
	new->residual_capacity = capacity;
	p1->siblings = new;
	new->from = p2;
}

void breadth_first_search (struct hashtable * main_table, struct node * source) {
	struct queue * head = create_queue();
	head->id = source->id;
	struct node * p;

	while (head != NULL) {
		p = search(main_table, head->id);
		p->used = 1;
		struct list * siblings = p->siblings;
		p->storage = NULL;
		while (siblings != NULL) {
			if (siblings->residual_capacity != 0) {
				if (siblings->from->used != 1) {
					siblings->from->used = 1;
					siblings->from->order = p->order + 1;
					add_queue(head, siblings->from->id);
				}
				if (siblings->from->order == (p->order + 1)) {
					printf("%d ---> %d:  ", p->id, siblings->from->id);
					siblings->storage_next = p->storage;
					p->storage = siblings;
					printf("Capacity == %d\n", siblings->residual_capacity);
				}
			}
			siblings = siblings->next;
		}
		head = remove_queue(head);
	}
}

int depth_first_search (struct node * p, int * min_residual_capacity, struct node * sink, int capacity) {
	struct list * neighbour = p->storage;
	printf("%d   ", p->id);
	if (p == sink) {
		return 0;
	}
	if (neighbour == NULL) {
		return -1;
	}

	if (*min_residual_capacity > neighbour->residual_capacity) {
		capacity = *min_residual_capacity;
		*min_residual_capacity = neighbour->residual_capacity;
	}

	while (depth_first_search(neighbour->from, min_residual_capacity, sink, capacity) != 0) {
		printf("%d   ", p->id);
		*min_residual_capacity = capacity;
		p->storage = neighbour->storage_next;
		neighbour->storage_next = NULL;
		neighbour = p->storage;
		if (neighbour == NULL) {
			return -1;
		}
		if (*min_residual_capacity > neighbour->residual_capacity) {
			capacity = *min_residual_capacity;
			*min_residual_capacity = neighbour->residual_capacity;
		}
	}

	neighbour->residual_capacity -= *min_residual_capacity;
	neighbour->opposite->residual_capacity += *min_residual_capacity;
	if (neighbour->residual_capacity == 0) {
		p->storage = neighbour->storage_next;
		neighbour->storage_next = NULL;
	}
	return 0;
}

void residual_network (struct node * p) {
	struct list * siblings = p->siblings;
	while (siblings != NULL) {
		if (siblings->opposite == NULL) {
			struct list * new = (struct list *)calloc(1, sizeof(struct list));
			new->from = p;
			new->next = siblings->from->siblings;
			siblings->from->siblings = new;
			new->opposite = siblings;
			siblings->opposite = new;
		}
		siblings = siblings->next;
	}
}

int work (struct hashtable * main_table) {
	int source_id = 0;
	int sink_id = 0;
	printf("Type the key source and sink\n");
	scanf("%d %d", &source_id, &sink_id);
	struct node * source = search(main_table, source_id);
	struct node * sink = search(main_table, sink_id);

	for (int i = 0; i < main_table->size; i++) {
		if (main_table->table[i] != NULL) {
			residual_network(main_table->table[i]);
		}
	}

	int max_flow = 0;
	while (1) {
		printf("\n\n");
		int now_flow = 0;
		breadth_first_search(main_table, source);
		for (int i = 0; i < main_table->size; i++) {
			if (main_table->table[i] != NULL) {
				main_table->table[i]->used = 0;
			}
		}
		int min_residual_capacity = MAX;
		
		while (source->storage != NULL) {
			depth_first_search(source, &min_residual_capacity, sink, 0);
			if (min_residual_capacity != MAX) {
				printf("\nFlow == %d\n", min_residual_capacity);
				now_flow += min_residual_capacity;
			}
			min_residual_capacity = MAX;
		}

		if (now_flow == 0) {
			return max_flow;
		}
		else {
			max_flow += now_flow;
		}
	}
}

int read (FILE * data, struct hashtable * main_table) {
	while (1) {
		int num1 = 0;
		int num2 = 0;
		int capacity = 0;
		int rc = 0;
		rc = fscanf(data, "%d %d %d", &num1, &num2, &capacity);
		if (rc == -1) {
			break;
		}
		if (num1 == num2) {
			continue;
		}
		if (rc != 3) {
			printf("Invalid input data\n");
			free_node(main_table);
			return 1;
		}
		create_link(insert(main_table, num1), insert(main_table, num2), capacity);
	}
	return 0;
}

int main (int argc, char * argv[]) {
	FILE * data;
	if (argc > 1) {
        data = fopen(argv[1], "r");
        if (data == NULL) {
        	printf("File open error\n");
           	return 1;
        }
    }
    else {
    	printf("Not arguments\n");
       	return 1;
    }

    struct hashtable * main_table = create_hashtable();
    if (read(data, main_table) == 1) {
    	free_node(main_table);
    	fclose(data);
    	return -1;
    }

    printf("MAXIMUM FLOW == %d\n", work(main_table));
    free_node(main_table);
    fclose(data);
    return 0;
}