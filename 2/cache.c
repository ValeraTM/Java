#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> 
#include <assert.h>
#include <string.h>
#include <time.h>
#include "hashtable.h"

struct LRUcache {
	struct hashtable * main_table;
	struct node * head;
	struct node * tail;
	int max_size;
	int now_size;
};

int get (struct LRUcache * cache, int value) {
	struct node * p = search(cache->main_table, value);
	if (p == NULL) {
		return -1;
	}
	if (p->used == 0) {
		return -1;
	}
	if (p == cache->head) {
		return 0;
	}

	struct node * q = p->prev;
	q->next = p->next;
	p->prev = NULL;
	if (p == cache->tail) {
		cache->tail = q;
	}
	else {
		q->next->prev = q;
	}

	p->next = cache->head;
	cache->head->prev = p;
	cache->head = p;
	return 0;
}

int put (struct LRUcache * cache, int value) {
	if (cache->head != NULL) {
		if (cache->head->next == NULL) {
			cache->tail = cache->head;
		}
	}
	struct node * p = insert(cache->main_table, value);
	if (p->used == 1) {
		return -1;
	}
	else {
		p->used = 1;
	}
	if (cache->now_size == cache->max_size) {
		cache->tail->used = 0;
		cache->tail = cache->tail->prev;
		cache->tail->next->prev = NULL;
		cache->tail->next = NULL;
	}
	else {
		cache->now_size++;
	}
	p->next = cache->head;
	if (cache->head != NULL) {
		cache->head->prev = p;
	}
	cache->head = p;
	return 0;
}

int read (FILE * data, struct hashtable * main_table) {  
	while (1) {
		int num = 0;
		int rc = 0;
		rc = fscanf(data, "%d", &num);
		if (rc == -1) {
			break;
		}
		insert(main_table, num);
	}
	return 0;
}

void remove_cache (struct LRUcache * cache) {
	while (cache->head != NULL) {
		struct node * p = cache->head;
		cache->head = cache->head->next;
		p->next = NULL;
		p->prev = NULL;
		p->used = 0;
	}
	cache->tail = NULL;
}

void work (struct LRUcache * cache) {
	char * key = (char *)calloc(10, sizeof(char));
	while (1) {
		scanf("%s", key);
		if (strcmp(key, "exit") == 0) {
			break;
		}
		int value;
		if (strcmp(key, "put") == 0) {
			scanf("%d", &value);
			if (put(cache, value) == -1) {
				printf("This value exist\n");
			}
		}
		if (strcmp(key, "get") == 0) {
			scanf("%d", &value);
			if (get(cache, value) == -1) {
				printf("This value not exist\n");
			}
		}
		if (strcmp(key, "remove") == 0) {
			remove_cache(cache);
		}

		struct node * p = cache->head;
		while (p != NULL) {
			printf("%d ", p->id);
			p = p->next;
		}
		printf("\n");
	}
	free(key);
}

int main (int argc, char * argv[]) {
    struct LRUcache * cache = (struct LRUcache *)calloc(1, sizeof(struct LRUcache));
    cache->main_table = create_hashtable();
    cache->max_size = 5;

    work(cache);
    free_node(cache->main_table);
	return 0;
}
