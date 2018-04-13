#include "fibonacci_heap.h"
#include <stdlib.h>

struct fibonacci_heap * create_fibonacci_heap(void) {
	struct fibonacci_heap * heap = (struct fibonacci_heap *)calloc(1, sizeof(struct fibonacci_heap));
	return heap;
}

void free_vortex (struct vortex * p) {
	if (p == NULL) {
		return;
	}
	free_vortex(p->child);
	while (p != NULL) {
		struct vortex * q = p;
		if (p == p->right) {
			p = NULL;
			free(q);															
		}
		else {
			p = p->right;
			free(q);
		}
	}
}

void free_heap (struct fibonacci_heap * heap) {
	struct vortex * p = heap->root;
	free_vortex(p);
	free(heap);
}

void insert_heap(struct fibonacci_heap * heap, int key, int id) {
	struct vortex * p = (struct vortex *)calloc(1, sizeof(struct vortex));
	p->key = key;
	p->id = id;
	if (heap->size == 0) {
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
}

void merge (struct vortex * q, struct vortex * p) {
	if (q->child == NULL) {
		p->parent = q;
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
}

void consolidate (struct fibonacci_heap * heap) {
	struct vortex ** arr = (struct vortex **)calloc(heap->size, sizeof(struct vortex *));
	struct vortex * p = heap->root;
	while (arr[p->height] != p) {
		if (arr[p->height] == NULL) {
			arr[p->height] = p;
			p = p->right;
		}
		else {
			struct vortex * min;
			struct vortex * max;
			if (p->key < arr[p->height]->key) {
				min = p;
				max = arr[p->height];
			}
			else {
				min = arr[p->height];
				max = p;
			}
			merge(min, max);
			arr[p->height] = NULL;
			min->height++;
			p = min;
		}
		if (p->key < heap->root->key) {
			heap->root = p;
		}
	}
	free(arr);
}

int delete_min_heap (struct fibonacci_heap * heap) {
	struct vortex * p = heap->root;
	if (p->child != NULL) {
		//если p - один
		if (p->right == p) {
			heap->root = p->child;
		}
		else {
			struct vortex * left = p->child->left;
			p->child->left = p->left;
			p->left->right = p->child;
			left->right = p->right;
			p->right->left = left;
			heap->root = p->right;
		}
		p->child->parent = NULL;
	}
	else {
		if (p->right == p) {
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
	heap->size--;
	consolidate(heap);
	return id;
}