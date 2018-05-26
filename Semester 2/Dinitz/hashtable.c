#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>

long long primes[] = {53, 103, 211, 421, 821, 1621, 3221, 6521, 12821, 
		25667, 51437, 102437, 204857, 409657, 819251, 1638431, 3276821, 6553621, 13107247};

unsigned int first_hash (unsigned int key) {
	unsigned int hash = key;
	hash = (hash << 5) + (hash << 7);
	return hash;
}

unsigned int second_hash (unsigned int key) {
	unsigned int hash = key;
	hash = key + (hash << 6) + (hash << 16);
	return hash;
}

void free_node (struct hashtable * main_table) {
	for (int i = 0; i < main_table->size; i++) {
		if (main_table->table[i] != NULL) {
			struct list * p = main_table->table[i]->siblings;
			while (p != NULL) {
				struct list * q = p;
				p = p->next;
				free(q);
			}
			free(main_table->table[i]);
		}		
	}
	free(main_table->table);
    free(main_table);
}

struct node * search (struct hashtable * main_table, int key) {
	unsigned int code1 = first_hash(key);
	unsigned int code2 = second_hash(key);
	unsigned int code = code1 % main_table->size;

	for (int i = 1; i < main_table->size; i++) {
		if (main_table->table[code] == NULL) {
			return NULL;
		}
		if (main_table->table[code]->id != key) {
			code = (code1 + i*code2) % main_table->size;
		}
		else {
			return main_table->table[code];
		}
	}
	return NULL;
} 

void resize (struct hashtable * main_table) {
	struct node ** new_hashtable = (struct node **)calloc(primes[main_table->k], sizeof(struct node));
	for (int i = 0; i < main_table->size; i++) {
		if (main_table->table[i] != NULL) {
			unsigned int h1 = first_hash(main_table->table[i]->id);
			unsigned int h2 = second_hash(main_table->table[i]->id);
			unsigned int key = h1 % primes[main_table->k];
			int j = 1;
			while (new_hashtable[key] != NULL) {
				key = (h1 + j*h2) % primes[main_table->k];
				j++;
			}
			
			new_hashtable[key] = main_table->table[i];
		}
	}	
	free(main_table->table);
	main_table->table = new_hashtable;
	main_table->size = primes[main_table->k];
	main_table->k++;
}

int same (struct hashtable * main_table, int key, int num) {
	if (NULL == main_table->table[key]) {
		return 0;
	}
	if (main_table->table[key]->id == num) {
		return 1;
	}
	else {
		return -1;
	}		
}

struct node * insert (struct hashtable * main_table, int num) {
	unsigned int h1 = first_hash(num);
	unsigned int h2 = second_hash(num);
	unsigned int key = h1 % main_table->size;
	int i = 1;
	int check = 0;
	while ((check = same(main_table, key, num)) != 0) {
		if (check == 1) {
			return main_table->table[key];
		}
		key = (h1 + i*h2) % main_table->size;
		i++;
	}

	struct node * p = (struct node *)calloc(1, sizeof(struct node));
	p->id = num;
	main_table->table[key] = p;
	if (i > main_table->size/2) {
		resize(main_table);	
		return search(main_table, num);
	}
	return main_table->table[key];
}

struct hashtable * create_hashtable () {
	struct hashtable * main_table = (struct hashtable *)calloc(1, sizeof(struct hashtable));
    main_table->size = 23;
    main_table->table = (struct node **)calloc(main_table->size, sizeof(struct node));
    return main_table;
}