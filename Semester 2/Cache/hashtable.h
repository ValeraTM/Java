#ifndef HASHTABLE_H
#define HASHTABLE_H

struct hashtable {
	struct node ** table;
	unsigned int size;
	int k;
};

struct node {
	int id;
	int used;
	struct node * next;
	struct node * prev;
};

void free_node (struct hashtable * main_table);
struct node * search (struct hashtable * main_table, int key);
int same (struct hashtable * main_table, int key, int num);
struct node * insert (struct hashtable * main_table, int num);
struct hashtable * create_hashtable (void);

#endif