#include "bloom.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX 12

int hash0(int key, int size) {
	long long int hash = key;
	hash += hash << 6 + hash << 16 + key >> 1;
	return hash % size;
}

int hash1(int key, int size) {
	long long int hash = key;
	hash += hash << 5 + hash << 7 + key >> 1;
	return hash % size;
}

int hash2(int key, int size) {
	long long int hash = key;
	hash += hash >> 4 + hash << 17 + key >> 1;
	return hash % size;
}

int hash3(int key, int size) {
	long long int hash = key;
	hash += hash >> 13 + hash << 14 + key >> 1;
	return hash % size;
}

int hash4(int key, int size) {
	long long int hash = key;
	hash += hash >> 7 + hash << 18 + key >> 1;
	return hash % size;
}

int hash5(int key, int size) {
	long long int hash = key;
	hash += hash >> 11 + hash << 20 + key >> 1;
	return hash % size;
}

int hash6(int key, int size) {
	long long int hash = key;
	hash += hash >> 8 + hash << 19 + key >> 1;
	return hash % size;
}

int hash7(int key, int size) {
	long long int hash = key;
	hash += hash >> 21 + hash << 15 + key >> 1;
	return hash % size;
}

int hash8(int key, int size) {
	long long int hash = key;
	hash += hash >> 23 + hash << 13 + key >> 1;
	return hash % size;
}

int hash9(int key, int size) {
	long long int hash = key;
	hash += hash >> 30 + hash << 2 + key >> 1;
	return hash % size;
}

int hash10(int key, int size) {
	long long int hash = key;
	hash += hash >> 26 + hash << 22 + key >> 1;
	return hash % size;
}

int hash11(int key, int size) {
	long long int hash = key;
	hash += hash >> 31 + hash << 5 + key >> 1;
	return hash % size;
}

int (*hash[MAX])(int, int) = {hash0, hash1, hash2, hash3, hash4, hash5, hash6, hash7, hash8, hash9, hash10, hash11};

struct bloom * create_bloom(int size, int num_hash) {
	struct bloom * bloom_table = (struct bloom *)calloc(1, sizeof(struct bloom));
	bloom_table->size = size + 8 - (size % 8);
	if (num_hash > MAX) {
		num_hash = MAX;
	}
	bloom_table->num_hash = num_hash;
	bloom_table->array = (char *)calloc(bloom_table->size / 8, sizeof(char));
	bloom_table->size = size;
	return bloom_table;
}

int check_belong(struct bloom * table, int key) {
	for (int i = 0; i < table->num_hash; i++) {
		int idx = hash[i](key, table->size);
		if (idx % 8 != 0) {
			if ((table->array[idx/8] >> (8 - (idx % 8)) & 1) == 0) {
				return -1; //не принадлежит
			}
		}
		else {
			if ((table->array[idx/8] & 1) == 0) {
				return -1; //не принадлежит
			}
		}
	}
	return 0; //возможно принадлежит
}

int add_bloom(struct bloom * table, int num) {
	if (table->num_el > (table->size / table->num_hash)) {
		return -1;
	}
	for (int i = 0; i < table->num_hash; i++) {
		int idx = hash[i](num, table->size);
		if (idx % 8 != 0) {
			table->array[idx/8] = table->array[idx/8] | (1 << (8 - (idx % 8)));
		}
		else {
			table->array[idx/8] = table->array[idx/8] | 1;
		}
	}
	table->num_el++;
	return 0;
}

void free_bloom(struct bloom * table) {
	free(table->array);
	free(table);
}