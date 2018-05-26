#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "fibonacci_heap.h"

#define MAX 100000
#define TRUE_WAY '*'
#define BARRIER 'o'
#define FREE_WAY ' '

struct list {
	struct node * node;
	double weight;
	struct list * next;
};

struct node {
	int idx;
	int used; // посещена или нет
	int open; // есть в куче или нет
	double label; //метка на оптимальность
	struct vortex * im_heap; //указатель на вершину в куче
	struct list * siblings; //соседи
	struct node * way; //нужный путь
};

void print_idx (char * table, int height, int width) {
	assert(table != NULL);
	assert(height > 0 && width > 0);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (table[i*width + j] != BARRIER) {
				printf("%2d ", i*width + j);
			}
			else {
				printf("   ");
			}
		}
		printf("\n");
	}
}

void print_map (char * table, int height, int width) {
	assert(table != NULL);
	assert(height > 0 && width > 0);

	for (int i = 0; i < width + 2; i++) {
		printf("%c ", BARRIER);
	}
	printf("\n");
	for (int i = 0; i < height; i++) {
		printf("%c ", BARRIER);
		for (int j = 0; j < width; j++) {
			printf("%c ", table[i*width + j]);
		}
		printf("%c ", BARRIER);
		printf("\n");
	}

	for (int k = 0; k < width + 2; k++) {
		printf("%c ", BARRIER);
	}
	printf("\n");
}

void free_list (struct list * p) {
	while (p != NULL) {
		struct list * q = p;
		p = p->next;
		free(q);
	}
}

void free_slats (struct node ** field, int height, int width) {
	assert(field != NULL);
	assert(height > 0 && width > 0);

	for (int i = 0; i < height*width; i++) {
		if (field[i] != NULL) {
			free_list(field[i]->siblings);
			free(field[i]);
		}
	}
	free(field);
}

void write_way (struct node ** field, char * table, int height, int width, int finish) {
	assert(field != NULL);
	assert(table != NULL);
	assert(height > 0 && width > 0);
	assert(finish >= 0 && finish < height*width);

	struct node * p = field[finish];
	table[finish] = TRUE_WAY;
	while (p->way != NULL) {
		table[p->way->idx] = TRUE_WAY;
		p = p->way;
	}

	print_map(table, height, width);
}

void create_link (struct node * p1, struct node * p2, int length) {
	assert(p1 != NULL);
	assert(length >= 0);

	if (p2 == NULL) {
		return;
	}
	p1->label = MAX;
	struct list * new1 = (struct list *)calloc(1, sizeof(struct list));
	new1->weight = length;
	new1->next = p1->siblings;
	new1->node = p2;
	p1->siblings = new1;
}

double heuristic_function (int now, int finish, int width) {
	int x1 = now % width;
	int y1 = now / width;
	int x2 = finish % width;
	int y2 = finish / width;
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

int search_way (struct node ** field, struct fibonacci_heap * heap, int start, int finish, int width) {
	assert(field != NULL);
	assert(heap != NULL);

	struct node * p = field[start];
	if (p == NULL) {
		return -1;
	}
	p->label = 0;
	p->im_heap = insert_heap(heap, p->label, start);
	p->open = 1;
	struct node * end = field[finish];
	if (end == NULL) {
		return -1;
	}

	while (heap->root != NULL) {
		int min_idx = delete_min_heap(heap);
		if (min_idx == finish) {
			return finish;
		}
		struct node * min = field[min_idx];
		min->im_heap = NULL;
		struct list * neighbores = min->siblings;
		while (neighbores != NULL) {
			if (neighbores->node->used == 1) {
				neighbores = neighbores->next;
				continue;
			}
			
			double length = min->label + neighbores->weight + heuristic_function(min_idx, finish, width);
			if (length < neighbores->node->label) {
				neighbores->node->label = length;
				neighbores->node->way = min;
				if (neighbores->node->open == 1) {
					decrease_key(heap, neighbores->node->im_heap, length);
				}
			}
			if (neighbores->node->open == 0) {
				neighbores->node->im_heap = insert_heap(heap, neighbores->node->label, neighbores->node->idx);
				neighbores->node->open = 1;
			}
			neighbores = neighbores->next;
		}
		min->used = 1;
	}

	return -1;
}

int work (struct node ** field, struct fibonacci_heap * heap, char * table, int height, int width) {
	assert(field != NULL);
	assert(heap != NULL);
	assert(table != NULL);
	assert(height > 0 && width > 0);

	print_map(table, height, width);
	print_idx(table, height, width);
	int start = 0;
	int finish = 0;
	printf("Please type a start and a finish\n");
	scanf("%d %d", &start, &finish);
	
	clock_t start_time = clock();
    printf("Reading...\n");

	if (search_way(field, heap, start, finish, width) != finish) {
		printf("This way not exist\n");
		return -1;
	}
	clock_t finish_time = clock();
    printf("Passed %f seconds\n", ((float)(finish_time - start_time)) / CLOCKS_PER_SEC);

	write_way(field, table, height, width, finish);
	return 0;
}

void siblings (struct node ** field, int i, int width, int height) {
	int x = i % width;
	int y = i / width;
	if (x != 0) {
		create_link(field[i], field[i - 1], 2);
	}

	if (x != (width - 1)) {
		create_link(field[i], field[i + 1], 2);
	}

	if (y != 0) {
		create_link(field[i], field[(y - 1)*width + x], 2);
		if (x != 0) {
			create_link(field[i], field[(y - 1)*width - 1 + x], 3);
		}
		if (x != (width - 1)) {
			create_link(field[i], field[(y - 1)*width + 1 + x], 3);
		}
	}
	if (y != (height - 1)) {
		create_link(field[i], field[(y + 1)*width + x], 2);
		if (x != 0) {
			create_link(field[i], field[(y + 1)*width - 1 + x], 3);
		}
		if (x != (width - 1)) {
			create_link(field[i], field[(y + 1)*width + 1 + x], 3);
		}
	}
}

void read (FILE * data, struct node ** field, char * table, int height, int width) {
	assert(data != NULL);
	assert(field != NULL);
	assert(table != NULL);
	assert(width > 0 && height > 0);
	
	int i = 0;
	while (1) {
		int num = 0;
		int rc = fscanf(data, "%d", &num);
		if (rc == -1) {
			break;
		}
		if (num == 0) {
			table[i] = FREE_WAY;
			field[i] = (struct node *)calloc(1, sizeof(struct node));
			field[i]->idx = i;
		}
		else {
			table[i] = BARRIER;
		}
		i++;
	}

	for (i = 0; i < height*width; i++) {
		if (field[i] != NULL) {
			siblings(field, i, width, height);
		}
	}
}

int main (int argc, char * argv[]) {
	FILE * data = NULL;
	if (argc == 2) {
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

    int height = 0;
    int width = 0;
    fscanf(data, "%d %d", &height, &width);
    struct node ** field = (struct node **)calloc(height*width, sizeof(struct node));
    struct fibonacci_heap * heap = create_fibonacci_heap();
    char table[height*width];

    read(data, field, table, height, width);
    work(field, heap, table, height, width);

    free_slats(field, height, width);
    free_heap(heap);
    fclose(data);
    return 0;
}