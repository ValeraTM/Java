#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> 
#include <assert.h>
#include <string.h>
#include <time.h>

struct list {
	int id;
	struct list * next;
};

void print_way (int * table, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 1; j < n; j++) {
			for (int k = 1; k < n; k++) {
				printf("%2d ", table[i*n*n + n*j + k]);
			}
			printf("\n");
		}
		printf("\n\n");
	}
}

void print_list (struct list * head) {
	struct list * p = head;
	while (p != NULL) {
		printf("%d ", p->id);
		p = p->next;
	}
}

void free_list (struct list * head) {
	while (head != NULL) {
		struct list * p = head;
		head = head->next;
		free(p);
	}
}

struct list * left_way (struct list * p, int k) {
	struct list * q = (struct list *)calloc(1, sizeof(struct list));
	q->id = k;
	q->next = p;
	return q;
}

struct list * right_way (struct list * p, int k) {
	struct list * q = (struct list *)calloc(1, sizeof(struct list));
	q->id = k;
	p->next = q;
	return q;
}

struct list * search_right (int n, int j, int * table, int k, struct list * right);

struct list * search_left (int n, int i, int * table, int k, struct list * left) {
	int p = k--;
	while (k > 0) {
		if (table[k*n*n + i*n + p] == table[(k - 1)*n*n + i*n + p]) {
			k--;
		}
		else {
			break;
		}
	}
	if (k == 0) {
		return left;
	}
	left = left_way(left, k);
	struct list * right = left;
	search_right(n, p, table, k, right);
	return search_left(n, i, table, k, left);
}

struct list * search_right (int n, int j, int * table, int k, struct list * right) {
	int p = k--;
	while (k > 0) {
		if (table[k*n*n + p*n + j] == table[(k - 1)*n*n + p*n + j]) {
			k--;
		}
		else {
			break;
		}
	}
	if (k == 0) {
		return right;
	}
	right = right_way(right, k);
	struct list * left = right;
	search_left(n, p, table, k, left);
	return search_right(n, j, table, k, right);
}

void show_way (int * table, int n, int i, int j) {
	struct list * head = (struct list *)calloc(1, sizeof(struct list));
	struct list * left = head;
	struct list * right = head;

	int k = n - 1;
	while (k > 0) {
		if (table[k*n*n + i*n + j] == table[(k-1)*n*n + i*n + j]) {
			k--;
		}
		else {
			break;
		}
	}
	if (k == 0) {
		if (table[i*n + j] == 0) {
			printf("This way not exist\n");
			return;
		}
		else {
			head->id = i;
			right_way(head, j);
		}
	}
	else {
		head->id = k;
		left = search_left(n, i, table, k, left);
		right = search_right(n, j, table, k, right);
		head = left_way(left, i);
		right = right_way(right, j);
	}

	print_list(head);
	free_list(head);
}

void Floyd_Worcheall (int n, int k, int * table, int * prev_table) {
	for (int i = 1; i < n; i++) {
		for (int j = 1; j < n; j++) {
			if (prev_table[n*i + k] == 0 || prev_table[n*k + j] == 0) {
				table[n*i + j] = prev_table[n*i + j];
				continue;
			}
			int length = prev_table[n*i + k] + prev_table[n*k + j];
			if (length < prev_table[n*i + j]) {
				table[n*i + j] = length;
			}
			else {
				if (prev_table[n*i + j] == 0 && length != 0) {
					table[n*i + j] = length;
				}
				else {
					table[n*i + j] = prev_table[n*i + j];
				}
			}
		}
	}
}

void search_way (int * table, int n) {
	for (int k = 1; k < n; k++) {
		Floyd_Worcheall(n, k, table + k*n*n, table + (k-1)*n*n);
	}

	for (int i = 1; i < n; i++) {
		for (int j = 1; j < n; j++) {
			printf("%d -> %d:  ", i, j);
			show_way(table, n, i, j);
			printf(" Length = %d", table[(n-1)*n*n + i*n + j]);
			printf("\n");
		}
	}
}

int read (FILE * data) {
	int n = 0;
	fscanf(data, "%d", &n);
	int * table = (int *)calloc(n*n*n, sizeof(int));
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
			return 1;
		}
		table[num1*n + num2] = length;
	}

	search_way(table, n);
	free(table);
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

    if (read(data) == 1) {
    	return -1;
    }
    fclose(data);
    return 0;
}
