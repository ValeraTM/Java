#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> 
#include <assert.h>
#include <string.h>
#include <time.h>

long long primes[] = {53, 103, 211, 421, 821, 1621, 3221, 6521, 12821, 
		25667, 51437, 102437, 204857, 409657, 819251, 1638431, 3276821, 6553621, 13107247};

struct hashtable {
	struct hash **table;
	unsigned int size;
	int k;
};

struct hash {
	char name[100]; //ключ
	int data1;
	int data2;	
};

void free_hash (struct hashtable * main_table) {
	for (int i = 0; i < main_table->size; i++) {
		if (main_table->table[i] != NULL) {
			free(main_table->table[i]);
		}		
	}
	free(main_table->table);
    free(main_table);
}

void print_hash (struct hash *node) {
	printf("%d  ", node->data1);
	printf("%d  ", node->data2);
	printf("\n");
}

unsigned int first_hash (const char * key) {
	unsigned int hash = 5113;
	for (int i = 0; i < strlen(key); i++) {
	    hash = ((hash << 5) + hash) + key[i];
	}
	return hash;
}

unsigned int second_hash (const char * key) {
	unsigned int hash = 1111;
	for (int i = 0; i < strlen(key); i++) {
		hash = key[i] + (hash << 6) + (hash << 16);
	}
	return hash;
}

void resize (struct hashtable * main_table) {
	struct hash ** new_hashtable = (struct hash **)calloc(primes[main_table->k], sizeof(struct hash));
	for (int i = 0; i < main_table->size; i++) {
		if (main_table->table[i] != NULL) {
			unsigned int h1 = first_hash(main_table->table[i]->name);
			unsigned int h2 = second_hash(main_table->table[i]->name);
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

//поиск слова
struct hash * search (struct hashtable * main_table, const char * key) {
	unsigned int code1 = first_hash(key);
	unsigned int code2 = second_hash(key);
	unsigned int code = code1 % main_table->size;

	for (long long i = 1; i < main_table->size; i++) {
		if (main_table->table[code] == NULL) {
			return NULL;
		}

		if (strcmp(main_table->table[code]->name, key) != 0) {
			code = (code1 + i*code2) % main_table->size;
		}
		else {
			return main_table->table[code];
		}
	}
	return NULL;
} 

void work (struct hashtable * main_table) {
	char * key = (char *)calloc(100, sizeof(char));
    printf("Please type the key\n");

   	while (1) {
		scanf("%100s", key);
		if (strcmp(key, "exit") == 0) {
			break;
		}
		clock_t start = clock();
		struct hash * node = search(main_table, key);
		if (node == NULL) {
			printf("Sorry, NOT found\n");
		}
		else {
			print_hash(node);
		}
		clock_t finish = clock();
		printf("Passed %f seconds\n", ((float)(finish - start)) / CLOCKS_PER_SEC );
    }

    free(key);
}

//если такой ключ уже существует, то пропускает
int same (struct hashtable * main_table, struct hash * p, unsigned int key) {
	if (NULL == main_table->table[key]) {
		return 0;
	}
	if (strcmp(main_table->table[key]->name, p->name) == 0) {
		return 1;
	}
	else {
		return 0;
	}		
}

void insert (struct hashtable * main_table, struct hash * p) {
	unsigned int h1 = first_hash(p->name);
	unsigned int h2 = second_hash(p->name);
	unsigned int key = h1 % main_table->size;
	unsigned int i = 1;

	if (same(main_table, p, key) == 1) {
		free(p);
		return;
	}
	
	while (main_table->table[key] != NULL) {
		key = (h1 + i*h2) % main_table->size;
		i++;
	}
	main_table->table[key] = p;

	if (i > main_table->size/2) {
		resize(main_table);	
	}
}

//считывание данных из файла
int read (FILE * data, struct hashtable * main_table) {  
	while (1) {
		int rc = 0;
		struct hash * p = (struct hash *)calloc(1, sizeof(struct hash));
		rc = fscanf(data, "%s %d %d", &p->name, &p->data1, &p->data2);
		if (rc == -1) {
			free(p);
			break;
		}

		if (rc != 3) {
			printf("Invalid input data\n");
			free(p);
			free_hash(main_table);
			return 1;
		}

		insert(main_table, p);
	}
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

    struct hashtable * main_table = (struct hashtable *)calloc(1, sizeof(struct hashtable));
    main_table->size = 23;
    main_table->table = (struct hash **)calloc(main_table->size, sizeof(struct hash));
    clock_t start = clock();
   	printf("Reading...\n");
    if (read(data, main_table) == 1) {
    	fclose(data);
    	return -1;
    }
    clock_t finish = clock();
	printf("Passed %f seconds\n", ((float)(finish - start)) / CLOCKS_PER_SEC );

	work(main_table);
	free_hash(main_table);
    fclose(data);
	return 0;
} 