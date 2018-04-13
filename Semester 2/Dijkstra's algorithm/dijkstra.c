#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> 
#include <assert.h>
#include <string.h>
#include <time.h>
#include "hashtable.h" 
#include "fibonacci_heap.h"

#define MAX 1000000

void free_list (struct list * p) {
	while (p != NULL) {
		struct list * q = p;
		p = p->next;
		free(q);
	}
}

void write_way (struct node * p) {
	if (p == NULL) {
		return;
	}
	write_way(p->way);
	printf("%d ", p->id);
}

void print_way (struct hashtable * main_table) {
	for (int i = 0; i < main_table->size; i++) {
		if (main_table->table[i] != NULL) {
			printf("Length == %d\t", main_table->table[i]->label);
			write_way(main_table->table[i]);
			printf("\n");
		}
	}
}

void create_link (struct node * p1, struct node * p2, int length) {
	p1->label = MAX;
	p2->label = MAX;

	struct list * new1 = (struct list *)calloc(1, sizeof(struct list));
	struct list * new2 = (struct list *)calloc(1, sizeof(struct list));
	new1->weight = length;
	new2->weight = length;

	new1->next = p1->siblings;
	new2->next = p2->siblings;

	p1->siblings = new1;
	p2->siblings = new2;

	new1->node = p2;
	new2->node = p1;
}

struct node * search_way (struct hashtable * main_table, struct fibonacci_heap * heap, int start, int finish) {
	struct node * p = search(main_table, start);
	if (p == NULL) {
		return NULL;
	}
	p->label = 0;
	insert_heap(heap, p->label, start);
	p->open = 1;
	struct node * end = search(main_table, finish);
	while (heap->root != NULL) {
		int min_id = delete_min_heap(heap);
		if (min_id == finish) {
			return end;
		}
		struct node * min = search(main_table, min_id);
		struct list * neighbores = min->siblings;
		while (neighbores != NULL) {
			if (neighbores->node->used == 1) {
				neighbores = neighbores->next;
				continue;
			}
			int length = min->label + neighbores->weight;
			if (length < neighbores->node->label) {
				neighbores->node->label = length;
				neighbores->node->way = min;
			}
			if (neighbores->node->open == 0) {
				insert_heap(heap, neighbores->node->label, neighbores->node->id);
				neighbores->node->open = 1;
			}
			neighbores = neighbores->next;
		}
		min->used = 1;
	}
	return NULL;
}

int work (struct hashtable * main_table, struct fibonacci_heap * heap) {
	int start = 0;
	int finish = 0;
	printf("Please type a start and a finish\n");
	scanf("%d %d", &start, &finish);
	
	clock_t start_time = clock();
    printf("Reading...\n");
    struct node * p;
	if ((p = search_way(main_table, heap, start, finish)) == NULL) {
		printf("This way not exist\n");
		return -1;
	}
	clock_t finish_time = clock();
    printf("Passed %f seconds\n", ((float)(finish_time - start_time)) / CLOCKS_PER_SEC);

	printf("Length == %d\t", p->label);
	write_way(p);
	return 0;
}

int read (FILE * data, struct hashtable * main_table) {  
	while (1) {
		int num1 = 0;
		int num2 = 0;
		int length = 0;
		int rc = 0;
		rc = fscanf(data, "%d %d %d", &num1, &num2, &length);
		if (rc == -1) {
			break;
		}
		if (rc != 3) {
			printf("Invalid input data\n");
			free_node(main_table);
			return 1;
		}
		create_link(insert(main_table, num1), insert(main_table, num2), length);
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
    struct fibonacci_heap * heap = create_fibonacci_heap();

    if (read(data, main_table) == 1) {
    	fclose(data);
    	return -1;
    }
    work(main_table, heap);

    free_node(main_table);
    free_heap(heap);
    fclose(data);
    return 0;
}