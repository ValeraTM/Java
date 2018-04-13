#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

struct fibonacci_heap {
	int size; // количество узлов
	struct vortex * root;
};

struct vortex {
	int id; // значение
	int key;
	int height; // высота дерева
	struct vortex * parent;
	struct vortex * child;
	struct vortex * left;
	struct vortex * right;
};

struct fibonacci_heap * create_fibonacci_heap (void);
void insert_heap (struct fibonacci_heap * heap, int key, int id);
int delete_min_heap (struct fibonacci_heap * heap);
void free_heap (struct fibonacci_heap * heap);

#endif