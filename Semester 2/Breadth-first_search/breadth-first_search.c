#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"
#include "hashtable.h"

void create_link (struct node * p1, struct node * p2) {
	struct list * new1 = (struct list *)calloc(1, sizeof(struct list));
	struct list * new2 = (struct list *)calloc(1, sizeof(struct list));

	new1->next = p1->siblings;
	new2->next = p2->siblings;

	p1->siblings = new1;
	p2->siblings = new2;

	new1->from = p2;
	new2->from = p1;
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
		struct list * siblings = p->siblings;
		while (siblings != NULL) {
			if (siblings->from->label != 1) {
				siblings->from->label = 1;
				add_queue(head, siblings->from->id);
			}
			siblings = siblings->next;
		}
		printf("%d ", head->id);
		head = remove_queue(head);
	}	
}

void work (struct hashtable * main_table) {
	for (int i = 0; i < main_table->size; i++) {
		if (main_table->table[i] != NULL) {
			if (main_table->table[i]->label != 1) {
				breadth_first_search(main_table, main_table->table[i]->id);
			}
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