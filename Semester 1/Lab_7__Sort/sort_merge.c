#include "sort_merge.h"
#include <string.h>

void sort (void * array, void * new, int left, int right, int el_size, int (*cmp)(const void *, const void *)) {
    if (left == right) {
        return;
    }
    int mid = (left + right)/2;
    sort(array, new, left, mid, el_size, cmp);
    sort(array, new, mid + 1, right, el_size, cmp);

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
            if (rm == 1) {
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