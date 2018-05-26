#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

struct fibonacci_heap {
	int size; // количество узлов
	struct vortex * root;
};

struct vortex {
	int id; // значение
	double key; //признак сравнения
	int degree; // количество детей
	char mark;
	struct vortex * parent;
	struct vortex * child;
	struct vortex * left;
	struct vortex * right;
};

struct fibonacci_heap * create_fibonacci_heap (void);
struct vortex * insert_heap (struct fibonacci_heap * heap, double key, int id);
int delete_min_heap (struct fibonacci_heap * heap);
int decrease_key (struct fibonacci_heap * heap, struct vortex * p, double new_key);
void free_heap (struct fibonacci_heap * heap);

#endif