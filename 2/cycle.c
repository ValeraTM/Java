#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> 
#include <assert.h>
#include <string.h>
#include <time.h>
#include "hashtable.h"

#define FACTOR 10

struct list {
	int id;
	struct list * next;
};

void swap (struct node ** x, struct node ** y) {
	struct node * new = *x;
	*x = *y;
	*y = new;
}

void free_list (struct list * head) {
	while (head != NULL) {
		struct list * p = head;
		head = head->next;
		free(p);
	}
}

void print_list (struct list * head) {
	while (head != NULL) {
		printf("%d ", head->id);
		head = head->next;
	}
}

void create_link (struct node * p1, struct node * p2) {
	p1->num_siblings++;
	p2->num_siblings++;
	int count1 = p1->count;
	int count2 = p2->count;
	if (p1 == p2) {
		if (count1 == 0) {
			count1 = FACTOR;
			p1->siblings = (struct node **)calloc(count1, sizeof(struct node));
		}
		if (count1 < p1->num_siblings) {
			count1 += FACTOR;
			realloc(p1->siblings, count1*sizeof(struct node));
		}
		p1->count = count1;
		p1->siblings[p1->num_siblings - 1] = p2;
		p2->siblings[p2->num_siblings - 2] = p1;
	}
	else {
		if (count1 == 0) {
			count1 = FACTOR;
			p1->siblings = (struct node **)calloc(count1, sizeof(struct node));
		}
		if (count2 == 0) {
			count2 = FACTOR;
			p2->siblings = (struct node **)calloc(count2, sizeof(struct node));
		}

		if (count1 < p1->num_siblings) {
			count1 += FACTOR;
			realloc(p1->siblings, count1*sizeof(struct node));
		}
		if (count2 < p2->num_siblings) {
			count2 += FACTOR;
			realloc(p2->siblings, count2*sizeof(struct node));
		}
		p1->count = count1;
		p2->count = count2;
		p1->siblings[p1->num_siblings - 1] = p2;
		p2->siblings[p2->num_siblings - 1] = p1;
	}
}

//считывание данных из файла
int read (FILE * data, struct hashtable * main_table) {  
	while (1) {
		int num1 = 0;
		int num2 = 0;
		int rc = 0;
		rc = fscanf(data, "%d %d", &num1, &num2);
		if (rc == -1) {
			break;
		}
		if (rc != 2) {
			printf("Invalid input data\n");
			free_node(main_table);
			return 1;
		}
		create_link(insert(main_table, num1), insert(main_table, num2));
	}
	return 0;
}

int check_EULER_cycle (struct hashtable * main_table) {
	for (int i = 0; i < main_table->size; i++) {
		if (main_table->table[i] == NULL) {
			continue;
		}
		if (main_table->table[i]->num_siblings % 2 == 1) {
			return -1;
		}
	}
	return 1;
}

int search_idx (struct node * p) {
	int num = p->siblings[0]->num_siblings;
	for (int i = 0; i < num; i++) {
		if (p->siblings[0]->siblings[i] == p) {
			return i;
		}
	}
}

void cycle (struct node * vortex, struct list * head, int id) {
	struct list * p = (struct list *)calloc(1, sizeof(struct list));
	if (head->next == NULL) {
		head->next = p;
	}
	else {
		struct list * new = head->next;
		head->next = p;
		p->next = new;
	}

	p->id = vortex->siblings[0]->id;

	struct node * q = vortex->siblings[0];
	q->num_siblings--;
	vortex->num_siblings--;
	if (q->num_siblings != 0) {
		swap(&q->siblings[q->num_siblings], &q->siblings[search_idx(vortex)]);
	}
	if (vortex->num_siblings != 0) {
		swap(&vortex->siblings[0], &vortex->siblings[vortex->num_siblings]);
	}

	if (p->id == id) {
		return;
	}
	
	cycle(q, p, id);
}

void work (struct hashtable * main_table) {
	if (check_EULER_cycle(main_table) == -1) {
		printf("Cycle Euler not exist\n");
		return;
	}
	struct list * head = (struct list *)calloc(1, sizeof(struct list));
	int i = 0;

	while (main_table->table[i] == NULL) {
		i++;
	}
	if (i < main_table->size) {
		head->id = main_table->table[i]->id;
		cycle(main_table->table[i], head, main_table->table[i]->id);

		struct list * p = head;
		while (p != NULL) {
			struct node * q = search(main_table, p->id);
			while (q->num_siblings != 0) {
				cycle(q, p, q->id);
			}
			p = p->next;
		}
	}
	else {
		return;
	}

	print_list(head);
	free_list(head);
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
    clock_t start = clock();
   	printf("Reading...\n");
    if (read(data, main_table) == 1) {
    	fclose(data);
    	return -1;
    }
    clock_t finish = clock();
	printf("Passed %f seconds\n", ((float)(finish - start)) / CLOCKS_PER_SEC);
	work(main_table);
	free_node(main_table);
    fclose(data);
}