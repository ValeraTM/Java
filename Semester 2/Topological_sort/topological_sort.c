#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> 
#include <assert.h>
#include <string.h>
#include <time.h>

int null_column (int * arr, int n) {
	int i = 0;
	int j = 0;
	while (j < n) {
		if (*(arr + i*n + j) != 0) {
			j++;
			i = -1;
		}
		i++;
		if (i == n) {
			return j;
		}
	}
	return -1;
}

void clear_str (int * arr, int n, int num) {
	for (int i = 0; i < n; i++) {
		*(arr + num*n + i) = 0;
		*(arr + i*n + num) = -1;
	}
}

int read (FILE * data, int * arr, int n) {
	while (1) {
		int num1, num2;
		int rc = fscanf(data, "%d %d", &num1, &num2);
		if (rc == -1) {
			break;
		}
		if (rc != 2) {
			free(arr);
			printf("Invalid input data\n");
			return -1;
		}
		arr[num1*n + num2] = 1;
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

	int n;
	fscanf(data, "%d", &n);
	int * arr = (int *)calloc(n*n, sizeof(int));

	if (read(data, arr, n) == -1) {
		fclose(data);
		return -1;
	}

	int num = 0;
	int count = 0;
	while ((num = null_column(arr, n)) != -1) {
		clear_str(arr, n, num);
		printf("%d ", num);
		count++;
	}
	if (count != 9) {
		printf("\nCycle\n");
	}

	fclose(data);
	free(arr);
	return 0;
} 