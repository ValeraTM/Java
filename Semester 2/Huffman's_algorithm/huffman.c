#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> 
#include <assert.h>
#include <string.h>
#include <time.h>

struct node {
	int c; //-1, если не символ
	int freq;
	int used;
	int bit;
	unsigned char key[32];
	struct node * left;
	struct node * right;
	struct node * parent;
};

struct code {
	int bit;
	unsigned char key[32];
};

void free_tree (struct node * p) {
	if (p == NULL) {
		return;
	}
	free_tree(p->left);
	free_tree(p->right);
	free(p);
}

void order (struct node * p, struct code * coder) {
	if (p->parent != NULL) {
		p->bit = p->parent->bit + 1;
		int i;
		if (p->bit % 8 == 0) {
			i = p->bit / 8 - 1;
		}
		else {
			i = p->bit / 8;
		}
		memcpy(p->key, p->parent->key, (i + 1)*sizeof(char));
		if (p->parent->left == p) {
			p->key[i] += 1 << (7 - ((p->bit - 1) % 8));
		}
	}

	if (p->left == NULL && p->right == NULL) {
		coder[p->c].bit = p->bit;
		memcpy(coder[p->c].key, p->key, 32*sizeof(char));
		return;
	}

	order(p->left, coder);
	order(p->right, coder);
}

struct node * min (struct node * tree, int n) {
	int i = 0;
	while (tree[i].used == 1) {
		i++;
	}
	int min = tree[i].freq;
	struct node * p = &tree[i];

	for (i; i < n; i++) {
		if (tree[i].used == 1) {
			continue;
		}
		if (min > tree[i].freq) {
			min = tree[i].freq;
			p = &tree[i];
		}
	}
	p->used = 1;
	p->parent = &tree[n];
	return p;
}

struct node * build_tree (int * freq, struct code * coder, int n) {
	struct node * tree = (struct node *)calloc(2*n - 1, sizeof(struct node));
	int j = 0;
	for (int i = 0; i < n; i++) {
		while (j < 256) {
			if (freq[j] != 0) {
				break;
			}
			j++;
		}
		if (j == 256) {
			break;
		}
		tree[i].c = j;
		tree[i].freq = freq[j];
		j++;
	}

	int size = 2*n - 1;
	while (n < size) {
		struct node * min1 = min(tree, n);
		struct node * min2 = min(tree, n);
		tree[n].c = -1;
		tree[n].freq = min1->freq + min2->freq;
		tree[n].left = min1;
		tree[n].right = min2;
		n++;
	}

	if (tree[size - 1].left == NULL && tree[size - 1].right == NULL) {
		tree[size - 1].bit = 1;
	}
	order(&tree[size - 1], coder);
	return tree;
}

void tree_coding (FILE * out, struct node * p, unsigned char * new, char * count) {
	if (p == NULL) {
		return;
	}

	if (p->c != -1) {
		*new += 1 << (7 - (*count));
		(*count)++;
		if (*count == 8) {
			fprintf(out, "%c", *new);
			*new = 0;
			*count = 0;
			fprintf(out, "%c", p->c);
		}
		else {
			unsigned char symbol = (unsigned char)p->c;
			*new += symbol >> (*count);
			fprintf(out, "%c", *new);
			*new = 0;
			*new += symbol << (8 - (*count));
		}
	}
	else {
		(*count)++;
	}

	if (*count == 8) {
		fprintf(out, "%c", *new);
		*new = 0;
		*count = 0;
	}

	tree_coding(out, p->left, new, count);
	tree_coding(out, p->right, new, count);
}

int read (FILE * data, int * freq) {
	int n = 0;
	unsigned char c = fgetc(data);
	while (!feof(data)) {
		if (freq[c] == 0) {
			n++;
		}
		freq[c]++;
		c = fgetc(data);
	}
	return n;
}

int coder (FILE * data) {
	clock_t start = clock();
    printf("Loading...\n");

	int * freq = (int *)calloc(256, sizeof(int));
    int n = read(data, freq);
    if (n == 0) {
    	free(freq);
    	printf("File is empty\n");
    	return -1;
    }
    printf("File read...\n");
    clock_t finish = clock();
    printf("Passed %f seconds\n", ((float)(finish - start)) / CLOCKS_PER_SEC);

    struct code * coder = (struct code *)calloc(256, sizeof(struct code));
    struct node * tree = build_tree(freq, coder, n);
    finish = clock();
    printf("Passed %f seconds\n", ((float)(finish - start)) / CLOCKS_PER_SEC);
    printf("Tree built...\n");

	fseek(data, 0, SEEK_SET);
	FILE * out = fopen("output.txt", "wb");
	char count = 0;
	unsigned char c = 0;
	fprintf(out, "1");
	tree_coding(out, &tree[2*n - 2], &c, &count);
	if (count != 0) {
		fprintf(out, "%c", c);
	}
	finish = clock();
    printf("Passed %f seconds\n", ((float)(finish - start)) / CLOCKS_PER_SEC);
    printf("Tree is coded...\n");

	unsigned char new = 0;
	int j = 0;

	while (fscanf(data, "%c", &c) != -1) {
		int i = 0;
		int bit = coder[c].bit;
		while (bit >= 8) {
			new += (coder[c].key[i] >> j);
			fprintf(out, "%c", new);
			new = 0;
			if (j != 0) {
				new += coder[c].key[i] << (8 - j);
			}
			i++;
			bit = bit - 8;
		}

		if (bit != 0) {
			new += (coder[c].key[i] >> j);
			if (bit > (8 - j)) {
				fprintf(out, "%c", new);
				new = 0;
				new += coder[c].key[i] << (8 - j);
				j = bit - (8 - j);
			}
			else {
				if (bit == (8 - j)) {
					fprintf(out, "%c", new);
					new = 0;
					j = 0;
				}
				else {
					j += bit;
				}
			}
		}
	}

	if (j != 0) {
		fprintf(out, "%c", new);
	}
	fseek(out, 0, SEEK_SET);
	fprintf(out, "%d", j);

	free(tree);
	free(freq);
	free(coder);
	fclose(out);

	finish = clock();
    printf("Passed %f seconds\n", ((float)(finish - start)) / CLOCKS_PER_SEC);
	return 0;
}

struct node * tree_decoding (FILE * out, struct node * p, unsigned char * c, char * count) {
	p = (struct node *)calloc(1, sizeof(struct node));
	p->c = -1;
	if (*count == -1) {
		fscanf(out, "%c", c);
		*count = 7;
	}

	if ((((*c) >> (*count)) & 1) == 0) {
		(*count)--;
		p->left = tree_decoding(out, p->left, c, count);
		p->right = tree_decoding(out, p->right, c, count);
	}
	else {
		(*count)--;
		if (*count == -1) {
			fscanf(out, "%c", c);
			p->c = *c;
		}
		else {
			unsigned char symbol = 0;
			symbol += (*c) << (7 - *count);
			fscanf(out, "%c", c);
			symbol += (*c) >> (8 - (7 - *count));
			p->c = symbol;
		}
		return p;
	}

	return p;
}

int decoder (FILE * out) {
	clock_t start = clock();
    printf("Reading...\n");
	char useless_bit = 0;
	if (fscanf(out, "%c", &useless_bit) == -1) {
		printf("File is empty\n");
		return -1;
	}
	useless_bit = useless_bit -'0';
	if (useless_bit == 0) {
		useless_bit = 8;
	}

	struct node * root = NULL;
	unsigned char c = 0;
	char count = -1;
	root = tree_decoding(out, root, &c, &count);
	clock_t finish = clock();
    printf("Passed %f seconds\n", ((float)(finish - start)) / CLOCKS_PER_SEC);
    printf("Tree is decoded...\n");

	FILE * data = fopen("Initial.txt", "wb");
	struct node * p = root;
	unsigned char next;
	fscanf(out, "%c", &c);
	while (fscanf(out, "%c", &next) != -1) {
		count = 7;
		while (count != -1) {
			if (((c >> count) & 1) == 1) {
				if (p->left != NULL) {
					p = p->left;
				}
			}
			else {
				if (p->right != NULL) {
					p = p->right;
				}
			}
			count--;
			if (p->c != -1) {
				fprintf(data, "%c", p->c);
				p = root;
			}
		}
		c = next;
	}

	count = 7;
	for (int i = 0; i < useless_bit; i++) {
		if (((c >> count) & 1) == 1) {
			if (p->left != NULL) {
				p = p->left;
			}
		}
		else {
			if (p->right != NULL) {
				p = p->right;
			}
		}
		count--;
		if (p->c != -1) {
			fprintf(data, "%c", p->c);
			p = root;
		}
	}

	free_tree(root);
	fclose(data);
	clock_t finish = clock();
    printf("Passed %f seconds\n", ((float)(finish - start)) / CLOCKS_PER_SEC);
	return 0;
}

int main (int argc, char * argv[]) {
	FILE * data;
	if (argc > 1) {
        data = fopen(argv[1], "rb");
        if (data == NULL) {
        	printf("File open error\n");
           	return 1;
        }
    }
    else {
    	printf("Not arguments\n");
       	return 1;
    }

    char key[15];
    scanf("%s", key);
    if (strcmp(key, "coding") == 0) {
    	if (coder(data) == -1) {
    		fclose(data);
    		return -1;
    	}
    }

    if (strcmp(key, "decoding") == 0) {
    	if (decoder(data) == -1) {
    		fclose(data);
    		return -1;
    	}
    }
    fclose(data);
    return 0;
}