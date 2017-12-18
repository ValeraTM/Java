#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> 
#include <assert.h>
#include <string.h>
#include <time.h>

#define PART 100000
#define SIZE 15

struct table {
    struct list * merge;   
    int size;
    unsigned int number; //названия буферных файлов
    int gradation; //по возрастанию или по убыванию
} * head;

struct list {
    FILE * file;
    char name[SIZE];
};

void free_file () {
    for (int i = 0; i < head->size; i++) {
        fclose(head->merge[i].file);
        remove(head->merge[i].name);
    }
    free(head->merge);
    free(head);
}

//названия файлов
void str (struct list * p) {
    int i = 0;
    int num = head->number;
    memset(p->name, 0, SIZE);
    while (num > 0) {
        p->name[i] = '0' + num % 10;
        num = num / 10;
        i++;
    }
    head->number++;
}

int double_cmp (const void * x1, const void * y1) {
    double x = *(double *)x1;
    double y = *(double *)y1;
    if (x == y) {
        return 0;
    }
    else {
        if (x > y) {
            return 1 * head->gradation;
        }
        else {
            return -1 * head->gradation;
        }
    }
}

void write_double (FILE * file, const void * x1, int n) {
    double * x = (double *)x1;
    for (int i = 0; i < n; i++) {
        fprintf(file, "%lf ", *(x + i));
    }
}

int scan_double (FILE * file, const void * x1) {
    double * x = (double *)x1;
    
    if (fscanf(file, "%lf", x) == -1) {
        return -1;
    }
    else {
        return 0;
   }
}

void swap_byte (char * x, char * y) {
    char new = *x;
    *x = *y;
    *y = new;
}

void swap (void * x, void * y, int el_size) {
    for (int i = 0; i < el_size; i++) {
        swap_byte(x + i, y + i);
    }
}


int min (void * array, int size, int el_size, int (*scan)(FILE *, const void *), int (*cmp)(const void *, const void *)) {
    void * min = array;
    int idx = 0;

    for (int j = 1; j < size; j++) {
        if (cmp(array + j*el_size, min) == -1) {
            min = array + j*el_size;
            idx = j;
        }
    }
    return idx;
}

void merge (int el_size, int (*scan)(FILE *, const void *), void (*write)(FILE *, const void *, int), 
    int (*cmp)(const void *, const void *)) {

    int size = head->size;
    void * array = (void *)calloc(size, el_size);

    for (int i = 0; i < size; i++) {
        fseek(head->merge[i].file, 0, SEEK_SET); 
        scan(head->merge[i].file, array + i*el_size);
    }

    FILE * out = fopen("output.txt", "w");

    int idx = 0;
    while (size > 1) {
        idx = min(array, size, el_size, scan, cmp);
        write(out, array + idx*el_size, 1);

        if (scan(head->merge[idx].file, array + idx*el_size) == -1) {
            swap(array + idx*el_size, array + (size - 1)*el_size, el_size);
            swap(&head->merge[idx], &head->merge[size - 1], sizeof(struct list));
            size--;
        }
    }
    write(out, array, 1);

    free(array);
    fclose(out);
}

void sort (void * array, int size, size_t el_size, int (*cmp)(const void *, const void *)) {
    int i = 0;
    int j = size;

    do {
        while (cmp(array + i*el_size, array) == -1) {
            i++;
        }

        while (cmp(array + j*el_size, array) == 1) {
            j--;
        }

        if (i <= j) {
            swap(array + i*el_size, array + j*el_size, el_size);
            i++;
            j--;
        }
    } while (i <= j);

    if (j > 0) {
        sort(array, j, el_size, cmp);  
    }

    if (i < size) {
        sort(array + i*el_size, size - i, el_size, cmp);
    }
}

int read_double (FILE * data, int * count, void * array) {
    int rm = 0;
    double * arr = (double *)array;
    for (int i = 0; i < PART; i++) {
        if ((rm = fscanf(data, "%lf", &arr[i])) == -1) {
            break;
        }
        (*count)++;
    }
    return rm;
}

void read (FILE * data, size_t el_size, int (*part_read)(FILE *, int *, void *), 
    void (*write)(FILE *, const void *, int), int (*cmp)(const void *, const void *)) {

    head->merge = (struct list *)calloc(1, sizeof(struct list));
    void * array = (void *)calloc(PART, el_size);
    int rm = 0;
    int i = 0;
    int size = 2;
    while (rm != -1) {
        int count = 0;
        rm = part_read(data, &count, array);
        if (count == 0) {
            break;
        }
        
        sort(array, count - 1, el_size, cmp);

        str(&head->merge[i]);
        head->merge[i].file = fopen(head->merge[i].name, "w+");

        if (rm != -1) {
            head->merge = realloc(head->merge, size*sizeof(struct list));
        }
        size++;
        
        write(head->merge[i].file, array, count);
        memset(array, 0, el_size*count);
        i++;
    }
    head->size = i;
    free(array);
}

int main (int argc, char * argv[]) {
    head = (struct table *)calloc(1, sizeof(struct table));
    head->number = 1;
    FILE * data;
    if (argc == 3) {
        data = fopen(argv[1], "r");
        if (strcmp(argv[2], "-up") == 0) {
            head->gradation = 1;
        }
        if (strcmp(argv[2], "-down") == 0) {
            head->gradation = -1;
        }
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
    read(data, sizeof(double), read_double, write_double, double_cmp);
    clock_t finish = clock();
    printf("Passed %f seconds\n", ((float)(finish - start)) / CLOCKS_PER_SEC);

    start = clock();
    merge(sizeof(double), scan_double, write_double, double_cmp);
    finish = clock();
    printf("Passed %f seconds\n", ((float)(finish - start)) / CLOCKS_PER_SEC);

    free_file();
    fclose(data);
    return 0;
} 