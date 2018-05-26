#ifndef BLOOM_H
#define BLOOM_H

struct bloom {
	int size;
	int num_hash;
	int num_el;
	char * array;
};

struct bloom * create_bloom(int size, int num_hash);
int check_belong(struct bloom * table, int key);
int add_bloom(struct bloom * table, int num);
void free_bloom(struct bloom * table);

#endif