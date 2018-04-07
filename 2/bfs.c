#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"
#include "hashtable.h"

#define FACTOR 10

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

void breadth_first_search (struct hashtable * main_table, int num) {
	struct queue * head = create_queue();
	head->id = num;
	struct node * p;

	while (head != NULL) {
		p = search(main_table, head->id);
		p->label = 1;
		for (int j = 0; j < p->num_siblings; j++) {
			if (p->siblings[j]->label == 1) {
				continue;
			}
			p->siblings[j]->label = 1;
			add_queue(head, p->siblings[j]->id);
		}
		printf("%d ", head->id);
		head = remove_queue(head);
	}	
}

void work (struct hashtable * main_table) {
	int i = 0;
	while (1) {
		while (i < main_table->size) {
			if (main_table->table[i] == NULL) {
				i++;
			}
			else {
				break;
			}
		}
		if (i < main_table->size) {
			if (main_table->table[i]->label == 1) {
				i++;
			}
			else {
				breadth_first_search(main_table, main_table->table[i]->id);
			}
		}
		else {
			return;
		}
	}
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