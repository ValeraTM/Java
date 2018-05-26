#ifndef HASHTABLE_H
#define HASHTABLE_H

struct hashtable {
	struct node ** table;
	unsigned int size;
	int k;
};

struct list {
	struct node * from;
	int residual_capacity; //остаточная пропускная способность
	struct list * next;
	struct list * storage_next; //
	struct list * opposite; //противоположное ребро
};

struct node {
	int id;
	int order;
	int used; //посещен или нет
	struct list * siblings;
	struct list * storage;
};

void free_node (struct hashtable * main_table);
struct node * search (struct hashtable * main_table, int key);
int same (struct hashtable * main_table, int key, int num);
struct node * insert (struct hashtable * main_table, int num);
struct hashtable * create_hashtable (void);

#endif