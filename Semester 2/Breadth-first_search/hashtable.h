#ifndef HASHTABLE_H
#define HASHTABLE_H

struct hashtable {
	struct node ** table;
	unsigned int size;
	int k;
};

struct list {
	struct node * from;
	struct list * next; 
};

struct node {
	int id;
	int label;
	struct list * siblings;
};

void free_node (struct hashtable * main_table);
struct node * search (struct hashtable * main_table, int key);
int same (struct hashtable * main_table, int key, int num);
struct node * insert (struct hashtable * main_table, int num);
struct hashtable * create_hashtable (void);

#endif