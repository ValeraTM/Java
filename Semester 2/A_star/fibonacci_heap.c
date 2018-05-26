#include "fibonacci_heap.h"
#include <stdlib.h>

struct fibonacci_heap * create_fibonacci_heap(void) {
	struct fibonacci_heap * heap = (struct fibonacci_heap *)calloc(1, sizeof(struct fibonacci_heap));
	return heap;
}

struct vortex * insert_heap(struct fibonacci_heap * heap, double key, int id) {
	struct vortex * p = (struct vortex *)calloc(1, sizeof(struct vortex));
	p->key = key;
	p->id = id;
	if (heap->root == NULL) {
		heap->root = p;
		p->left = p;
		p->right = p;
	}
	else {
		struct vortex * q = heap->root->right;
		heap->root->right = p;
		p->left = heap->root;
		p->right = q;
		q->left = p;
		if (p->key < heap->root->key) {
			heap->root = p;
		}
	}
	heap->size++;
	return p;
}

void merge (struct vortex * q, struct vortex * p) {
	if (q->child == NULL) {
		q->child = p;
		p->left->right = p->right;
		p->right->left = p->left;
		p->left = p;
		p->right = p;
	}
	else {
		struct vortex * left = q->child->left;
		struct vortex * right = p->right;
		p->right = q->child;
		q->child->left = p;
		right->left = p->left;
		p->left->right = right;
		p->left = left;
		left->right = p;
	}
	p->parent = q;
	p->mark = 0;
	q->degree++;
}

void consolidate (struct fibonacci_heap * heap) {
	struct vortex ** arr = (struct vortex **)calloc(2*heap->size, sizeof(struct vortex *));
	struct vortex * p = heap->root;
	while (arr[p->degree] != p) {
		if (arr[p->degree] == NULL) {
			arr[p->degree] = p;
		}
		else {
			struct vortex * min;
			struct vortex * max;
			int degree = p->degree;
			if (p->key < arr[p->degree]->key) {
				min = p;
				max = arr[p->degree];
			}
			else {
				min = arr[p->degree];
				max = p;
			}
			merge(min, max);
			arr[degree] = NULL;
			p = min;
		}
		if (p->key < heap->root->key) {
			heap->root = p;
		}
		p = p->right;
	}
	free(arr);
}

int delete_min_heap (struct fibonacci_heap * heap) {
	struct vortex * p = heap->root;
	if (p->child != NULL) {
		//если p - один
		struct vortex * q = p->child;
		for (int i = 0; i < p->degree; i++) {
			q->parent = NULL;
			q = q->right;
		}

		if (p->right == p) {
			heap->root = p->child;
		}
		else {
			struct vortex * left = p->child->left;
			p->child->left = p->left;
			p->left->right = p->child;
			left->right = p->right;
			p->right->left = left;
			heap->root = p->child;
		}
	}
	else {
		if (p == p->right) {
			int id = p->id;
			free(p);
			heap->size--;
			heap->root = NULL;
			return id;
		}
		else {
			p->left->right = p->right;
			p->right->left = p->left;
			heap->root = p->right;
		}
	}

	int id = p->id;
	free(p);
	consolidate(heap);
	heap->size--;
	return id;
}

void free_heap (struct fibonacci_heap * heap) {
	while (heap->root != NULL) {
		delete_min_heap(heap);
	}
	free(heap);
}

void cut(struct fibonacci_heap * heap, struct vortex * p) {
	struct vortex * parent = p->parent;
	parent->degree--;

	p->left->right = p->right;
	p->right->left = p->left;
	if (parent->child == p) {
		if (parent->degree == 0) {
			parent->child = NULL;
		}
		else {
			parent->child = p->right;
		}
	}
	p->left = heap->root;
	heap->root->right->left = p;
	p->right = heap->root->right;
	heap->root->right = p;
	p->parent = NULL;
	p->mark = 0;
}

void cascading_cut(struct fibonacci_heap * heap, struct vortex * p) {
	struct vortex * parent = p->parent;
	if (parent != NULL) {
		if (p->mark == 0) {
			p->mark = 1;
		}
		else {
			cut(heap, p);
			cascading_cut(heap, parent);
		}
	}
}

int decrease_key (struct fibonacci_heap * heap, struct vortex * p, double new_key) {
	if (new_key >= p->key) {
		return -1;
	}
	p->key = new_key;
	struct vortex * parent = p->parent;
	if (parent != NULL) {
		if (p->key < parent->key) {
			cut(heap, p);
			cascading_cut(heap, parent);
		}
	}
	if (p->key < heap->root->key) {
		heap->root = p;
	}
	return 0;
}