#ifndef HASHTABLE_H
#define HASHTABLE_H

struct hashtable {
	struct node ** table;
	unsigned int size;
	int k;
};

struct list {
	struct node * node;
	int weight;
	struct list * next;
};

struct node {
	char used : 1; // посещена или нет
	char open : 1; // открыта или нет
	int label; //метка на оптимальность
	int id; // ключ
	struct list * siblings; //соседи
	struct node * way; //нужный путь
};

void free_node (struct hashtable * main_table);
struct node * search (struct hashtable * main_table, int key);
int same (struct hashtable * main_table, int key, int num);
struct node * insert (struct hashtable * main_table, int num);
struct hashtable * create_hashtable (void);

#endif