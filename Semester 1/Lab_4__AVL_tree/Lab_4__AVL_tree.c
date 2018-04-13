#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> 
#include <assert.h>
#include <string.h>
#include <time.h>

struct info {
	char name[100]; //ключ
	int data1;
	int data2;
};

struct tree {
	int balance;
	struct info value;
	struct tree * parent;
	struct tree * left;
	struct tree * right;
};

void print_tree (const struct tree *node) {
	if (node == NULL) {
		return;
	}
	printf("%d  ", node->value.data1);
	printf("%d  ", node->value.data2);
	printf("\n");
}

void free_tree (struct tree *node) {
	if (node == NULL) {
		return;
	}
    free_tree(node->left);
    free_tree(node->right);
    free(node);    
}

//левый поворот вокруг node
struct tree * rotate_left (struct tree* node) {
	assert(node != NULL);
	struct tree * p = node->right;
	node->right = p->left;
	if (p->left != NULL) {
		(p->left)->parent = node;
	}
	p->left = node;

	p->parent = node->parent;
	node->parent = p;
	if (p->balance == 0) {
		node->balance = 1;
		p->balance = -1;
	}
	else {
		node->balance = 0;
		p->balance = 0;
	}
	return p;
}

//правый поворот вокруг node
struct tree * rotate_right (struct tree* node) {
	assert(node != NULL);
	struct tree * p = node->left;
	node->left = p->right;
	if (p->right != NULL) {
		(p->right)->parent = node;
	}
	p->right = node;

	p->parent = node->parent;
	node->parent = p;

	if (p->balance == 0) {
		node->balance = -1;
		p->balance = 1;
	}
	else {
		node->balance = 0;
		p->balance = 0;
	}
	return p;
}

struct tree * big_rotate_left (struct tree * node) {
	struct tree * p = node->right;
	struct tree * q = p->left;
	node->right = q;
	p->left = q->right;
	if (q->right != NULL) {
		(q->right)->parent = p;
	}
	q->right = p;
	node->right = q->left;
	if (q->left != NULL) {
		(q->left)->parent = node;
	}
	q->left = node;

	q->parent = node->parent;
	node->parent = q;
	p->parent = q;
	
	switch (q->balance) {
		case 1:
			node->balance = 0;
			p->balance = -1;
			break;
		case -1:
			node->balance = 1;
			p->balance = 0;
			break;
		case 0:
			node->balance = 0;
			p->balance = 0;
	}
	q->balance = 0;
	return q;
}

struct tree * big_rotate_right (struct tree * node) {
	struct tree * p = node->left;
	struct tree * q = p->right;
	node->left = q;
	p->right = q->left;
	if (q->left != NULL) {
		(q->left)->parent = p;
	}

	q->left = p;
	node->left = q->right;
	if (q->right != NULL) {
		(q->right)->parent = node;
	}
	q->right = node;

	q->parent = node->parent;
	node->parent = q;
	p->parent = q;

	switch (q->balance) {
		case 1:
			node->balance = -1;
			p->balance = 0;
			break;
		case -1:
			node->balance = 0;
			p->balance = 1;
			break;
		case 0:
			node->balance = 0;
			p->balance = 0;
	}
	q->balance = 0;
	return q;
}

//балансировка узла node
struct tree * balance (struct tree * node) {
	assert(node != NULL);
	
	if (node->balance == -2) {
		if ((node->right)->balance > 0) {
			return big_rotate_left(node);
		}
		return rotate_left(node);
	}
	if (node->balance == 2) {
		if ((node->left)->balance < 0) {
			return big_rotate_right(node);
		}
		return rotate_right(node);
	}
	
	return node;
}

void fixbalance (struct tree * root, struct tree * node, const char * key) {
	while (node != root) {
		if (strcmp(node->value.name, key) == -1) {
			node->balance += -1;
		}
		else {
			node->balance += 1;
		}

		node = balance(node);

		if (strcmp((node->parent)->value.name, node->value.name) == -1) {
			(node->parent)->right = node;
		}
		else {
			(node->parent)->left = node;
		}

		if (node->balance == 0) {
			return;
		}

		node = node->parent;
	}

	if (strcmp(root->value.name, key) == -1) {
		root->balance -= 1;
	}
	else {
		root->balance += 1;
	}
	root = balance(root);
	return;
}

//если такой ключ уже существует, то пропускает
int same (struct tree * node, const char * key) {
	if (NULL == node) {
		return 0;
	}
	if (strcmp(node->value.name, key) == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

//вставка нового узла
void insert (struct tree * node, struct info p) {
	struct tree * root = node;
	while (NULL != node) {
		if (same(node, p.name) == 1) {
			return;
		}
		//больше
		if (strcmp(node->value.name, p.name) == -1) {
			if (NULL == node->right) {
				node->right = (struct tree *)calloc(1, sizeof(struct tree));
				node->right->value = p;
				node->right->parent = node;
				fixbalance(root, node, p.name);
				return;
			}		
			node = node->right;
		}
		//меньше
		if (strcmp(node->value.name, p.name) == 1) {
			if (NULL == node->left) {
				node->left = (struct tree *)calloc(1, sizeof(struct tree));
				node->left->value = p;
				node->left->parent = node;
				fixbalance(root, node, p.name);
				return;
			}
			node = node->left;
		}
	}
}

//считывание данных из файла
struct tree * read (struct tree * root, FILE * data) {
	int rc;
	if (NULL == root) {
		root = (struct tree *)calloc(1, sizeof(struct tree));
		root->parent = NULL;
		if ((rc = fscanf(data, "%s %d %d", &root->value.name, &root->value.data1, &root->value.data2)) != 3) {
			printf("ERROR! Invalid input data\n");
			free_tree(root);
			return NULL;
		}
	}

	while (1) {
		struct info p;
		rc = fscanf(data, "%s %d %d", &p.name, &p.data1, &p.data2);
		if (rc == -1) {
			break;
		}

		if (rc != 3) {
			printf("ERROR! Invalid input data\n");
			free_tree(root);
			return NULL;
		}
		
		insert(root, p);	
	}
	return root;
}

//поиск слова
struct tree * search (struct tree *node, const char * key) {
	if (node == NULL) {
		return NULL;
	}
	int rm = 0;
	if ((rm = strcmp(node->value.name, key)) == 0) {
    	return node;
    }
    else {
    	if (rm == 1) {
    		return search(node->left, key);
    	}
    	if (rm == -1) {
    		return search(node->right, key);
    	}
    }
} 

void work (struct  tree * root) {
	assert(root != NULL);
	char * key = (char *)calloc(100, sizeof(char));
    printf("Please type the key\n");

   	while (1) {
		scanf("%100s", key);
		if (strcmp(key, "exit") == 0) {
			break;
		}
		clock_t start = clock();
		struct tree * node = search(root, key);
		if (node == NULL) {
			printf("Sorry, NOT found\n");
		}
		else {
			print_tree(node);
		}
		clock_t finish = clock();
		printf("Passed %f seconds\n", ((float)(finish - start)) / CLOCKS_PER_SEC );
    }

    free(key);
}

//определяет высоту поддерева
int height (struct tree * node) {
	if (node == NULL) {
		return 0;
	}
	int left = 0;
	int right = 0;

	left += height(node->left);
	right += height(node->right);

	if (left > right) {
		return left + 1;
	}
	else {
		return right + 1;
	}
}

//проверка баланса
int check_balance (struct tree * node) {
	if (node == NULL) {
		return 0;
	}
	int count = 0;
	int balance = height(node->left) - height(node->right);
	if (balance < 2 && balance > -2) {
		count += check_balance(node->left);
		count += check_balance(node->right);
	}
	else {
		return 1;
	}
	return count;
}

int main (int argc, char * argv[]) {
	struct tree * root = NULL;
	FILE * data;

	if (argc == 2) {
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

    clock_t start = clock();
	printf("Reading...\n");
    if ((root = read(root, data)) == NULL) {
    	fclose(data);
    	return -1;
    }
    clock_t finish = clock();
    printf("Passed %f seconds\n", ((float)(finish - start)) / CLOCKS_PER_SEC );

   	if (check_balance(root) == 0) {
    	printf("This tree is balanced\n");
    }
    else {
    	printf("This tree is unbalanced\n");
    }
    work(root);

    free_tree(root);
    fclose(data);
	return 0;
} 