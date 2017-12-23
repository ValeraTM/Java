#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> 
#include <assert.h>
#include <string.h>
#include <time.h>
#include <float.h>

#define PART_SIZE 10000
#define SIZE 15

struct table {
    struct list * files;   
    int size;
    unsigned int number; //названия буферных файлов
};

struct list {
    FILE * file;
    char name[SIZE];
};

void free_file (struct table * head) {
    for (int i = 0; i < head->size; i++) {
        fclose(head->files[i].file);
        remove(head->files[i].name);
    }
    free(head->files);
    free(head);
}

//названия файлов
void file_names (struct table * head, int k) {
    int i = 0;
    int num = head->number;
    memset(head->files[k].name, 0, SIZE);
    while (num > 0) {
        head->files[k].name[i] = '0' + num % 10;
        num = num / 10;
        i++;
    }
    head->number++;
}

int double_cmp (const void * x1, const void * y1) {
    double x = *(double *)x1;
    double y = *(double *)y1;

    if (fabs(x - y) < DBL_EPSILON) {
        return 0;
    }
    else {
        if (x - y > 0) {
            return 1;
        }
        else {
            return - 1;
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

int min (void * array, int size, int el_size, int gradation, int (*cmp)(const void *, const void *)) {
    void * min = array;
    int idx = 0;

    for (int j = 1; j < size; j++) {
        if (cmp(array + j*el_size, min) == -1*gradation) {
            min = array + j*el_size;
            idx = j;
        }
    }
    return idx;
}

void merge (struct table * head, int el_size, int gradation, int (*scan)(FILE *, const void *), void (*write)(FILE *, const void *, int), 
    int (*cmp)(const void *, const void *)) {

    int size = head->size;
    void * array = calloc(size, el_size);

    for (int i = 0; i < size; i++) {
        fseek(head->files[i].file, 0, SEEK_SET); 
        scan(head->files[i].file, array + i*el_size);
    }

    FILE * out = fopen(".txt", "w");

    int idx = 0;
    while (size > 1) {
        idx = min(array, size, el_size, gradation, cmp);
        write(out, array + idx*el_size, 1);

        if (scan(head->files[idx].file, array + idx*el_size) == -1) {
            swap(array + idx*el_size, array + (size - 1)*el_size, el_size);
            swap(&head->files[idx], &head->files[size - 1], sizeof(struct list));
            size--;
        }
    }
    write(out, array, 1);

    while (scan(head->files[0].file, array) != -1) {
        write(out, array, 1);
    }

    free(array);
    fclose(out);
}

void sort (void * array, void * new, int left, int right, int el_size, int gradation, int (*cmp)(const void *, const void *)) {
    if (left == right) {
        return;
    }
    int mid = (left + right)/2;
    sort(array, new, left, mid, el_size, gradation, cmp);
    sort(array, new, mid + 1, right, el_size, gradation, cmp);

    int i = left;
    int j = mid + 1;
    int k = 0;
    while (i <= mid && j <= right) {
        int rm = 0;
        if ((rm = cmp(array + i*el_size, array + j*el_size)) == 0) {
            memcpy(new + k*el_size, array + i*el_size, el_size);
            k++;
            memcpy(new + k*el_size, array + j*el_size, el_size);
            i++;
            j++;
        }
        else {
            if (rm == (1*gradation)) {
                memcpy(new + k*el_size, array + j*el_size, el_size);
                j++;
            }
            else {
                memcpy(new + k*el_size, array + i*el_size, el_size);
                i++;
            }
        }
        k++;
    }

    while (i <= mid) {
        memcpy(new +k*el_size, array + i*el_size, el_size);
        i++;
        k++;
    }

    while (j <= right) {
        memcpy(new + k*el_size, array + j*el_size, el_size);
        j++;
        k++;
    }

    for (i = 0; i < right - left + 1; i++) {
        memcpy(array + left*el_size + i*el_size, new + i*el_size, el_size);
    }
}

int read_double (FILE * data, int * count, void * array) {
    int rm = 0;
    double * arr = (double *)array;
    for (int i = 0; i < PART_SIZE; i++) {
        if ((rm = fscanf(data, "%lf", &arr[i])) == -1) {
            break;
        }
        (*count)++;
    }
    return rm;
}

void read (struct table * head, FILE * data, size_t el_size, int gradation, int (*part_read)(FILE *, int *, void *), 
    void (*write)(FILE *, const void *, int), int (*cmp)(const void *, const void *)) {

    head->files = (struct list *)calloc(1, sizeof(struct list));
    void * array = calloc(PART_SIZE, el_size);
    void * new = calloc(PART_SIZE, el_size);
    int rm = 0;
    int i = 0;
    int size = 2;
    while (rm != -1) {
        int count = 0;
        rm = part_read(data, &count, array);
        if (count == 0) {
            break;
        }
        
        sort(array, new, 0, count - 1, el_size, gradation, cmp);

        file_names(head, i);
        head->files[i].file = fopen(head->files[i].name, "w+");

        if (rm != -1) {
            head->files = realloc(head->files, size*sizeof(struct list));
        }
        size++;
        
        write(head->files[i].file, array, count);
        memset(array, 0, el_size*count);
        i++;
    }
    head->size = i;
    free(array);
    free(new);
}

int main (int argc, char * argv[]) {
    struct table * head = (struct table *)calloc(1, sizeof(struct table));
    head->number = 1;
    int gradation = 0;
    FILE * data;
    if (argc == 3) {
        data = fopen(argv[1], "r");
        if (data == NULL) {
            printf("File open error\n");
            return 1;
        }
        if (strcmp(argv[2], "-up") == 0) {
            gradation = 1;
        }
        if (strcmp(argv[2], "-down") == 0) {
            gradation = -1;
        }
    }
    else {
        printf("Not enough arguments\n");
        return 1;
    }
    
    clock_t start = clock();
    printf("Reading...\n");
    read(head, data, sizeof(double), gradation, read_double, write_double, double_cmp);
    clock_t finish = clock();
    printf("Passed %f seconds\n", ((float)(finish - start)) / CLOCKS_PER_SEC);

    start = clock();
    merge(head, sizeof(double), gradation, scan_double, write_double, double_cmp);
    finish = clock();
    printf("Passed %f seconds\n", ((float)(finish - start)) / CLOCKS_PER_SEC);

    free_file(head);
    fclose(data);
    return 0;
} 