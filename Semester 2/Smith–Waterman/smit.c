#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#define Gap_penalty 20
#define Extend_penalty 1

#define MAX_length_str 100

int EBLOSUM62[100][100] = {['A']['A'] = 4, ['A']['T'] = 0, ['A']['G'] = 0, ['A']['C'] = 0, ['T']['A'] = 0, ['T']['T'] = 5,
['T']['G'] = -2, ['T']['C'] = -1, ['G']['A'] = 0, ['G']['T'] = -2, ['G']['G'] = 6, ['G']['C'] = -3,
['C']['A'] = 0, ['C']['T'] = -1, ['C']['G'] = -3, ['C']['C'] = 9};

struct elem {
	int value;
	int gap_open_up;
	int gap_open_left;
	struct elem * from;
};

void free_matrix(struct elem ** matrix, int size) {
	assert(size > 0);
	assert(matrix != NULL);
	for (int i = 0; i < size; i++) {
		assert(matrix[i] != NULL);
		free(matrix[i]);
	}
	free(matrix);
}

void print_matrix(char * str1, char * str2, struct elem ** matrix, int m, int n) {
	assert(str1 != NULL);
	assert(str2 != NULL);
	assert(matrix != NULL);
	assert(m > 0 && n > 0);
	printf("Matrix: EBLOSUM62\n");
	printf("Gap_penalty: %.1f\n", Gap_penalty/2.0);
	printf("Extend_penalty: %.1f\n\n", Extend_penalty/2.0);

	printf("       ");
	for (int i = 0; i < n - 1; i++) {
		printf(" %c  ", str2[i]);
	}
	printf("\n");
	for (int i = 0; i < m; i++) {
		if (i != 0) {
			printf("%c ", str1[i-1]);
		}
		else {
			printf("  ");
		}
		for (int j = 0; j < n; j++) {
			printf("%3d ", matrix[i*n + j]->value);
		}
		printf("\n");
	}
	printf("\n");
}

void reverse_str(char * str, int k) {
	assert(str != NULL);
	assert(k > 0);

	int i = 0;
	while (i < k) {
		char c = str[i];
		str[i] = str[k];
		str[k] = c;
		i++;
		k--;
	}

	int j = 0;
	while (str[j]) {
		printf("%c ", str[j]);
		j++;
	}
	printf("\n");
}

void print_result(char * str1, char * str2, char * merge, int length) {
	assert(str1 != NULL);
	assert(str2 != NULL);
	assert(merge != NULL);
	assert(length > 0);

	printf("Length: %d\n", length);
	int i = 0;
	int count = 0;
	int replace = 0;
	while (merge[i]) {
		if (merge[i] == '|') {
			count++;
		}
		if (merge[i] == '.') {
			replace++;
		}
		i++;
	}
	printf("Identity: %d/%d (%.1f%%)\n", count, length, (count/(1.0*length))*100);
	printf("Similarity: %d/%d (%.1f%%)\n", count, length, (count/(1.0*length))*100);
	printf("Gaps: %d/%d (%.1f%%)\n\n", length - count - replace, length, ((length - count - replace)/(1.0*length))*100);
	reverse_str(str1, length - 1);
	reverse_str(merge, length - 1);
	reverse_str(str2, length - 1);
}

int trace(char * str1, char * str2, struct elem ** matrix, int idx, int m, int n) {
	assert(str1 != NULL);
	assert(str2 != NULL);
	assert(matrix != NULL);
	assert(m > 0 && n > 0);
	assert(idx >= 0 && idx < m*n);

	int score = matrix[idx]->value;
	int size;
	if (m > n) size = m;
	else size = n;

	char * new_str1 = (char *)calloc(size, sizeof(char));
	char * new_str2 = (char *)calloc(size, sizeof(char));
	char * merge = (char *)calloc(size, sizeof(char));
	int k = 0;

	int i = idx / n;
	int j = idx % n;
	struct elem * q = matrix[i*n + j];
	while (q->from != NULL && q->value != 0) {
		int id = 0;
		int jd = 0;

		if (q->gap_open_left == 1) {
			new_str1[k] = '-';
			new_str2[k] = str2[j-1];
			jd++;
		}
		if (q->gap_open_up == 1) {
			new_str1[k] = str1[i-1];
			new_str2[k] = '-';
			id++;
		}
		if (q->gap_open_up != 1 && q->gap_open_left != 1) {
			new_str1[k] = str1[i-1];
			new_str2[k] = str2[j-1];
			id++;
			jd++;
		}
		k++;
		i -= id;
		j -= jd;
		q = q->from;
	}

	new_str1[k] = '\0';
	new_str2[k] = '\0';
	
	for (int p = 0; p < k; p++) {
		if (new_str1[p] == '-' || new_str2[p] == '-') {
			merge[p] = ' ';
		}
		else {
			if (new_str1[p] == new_str2[p]) {
				merge[p] = '|';
			}
			else {
				merge[p] = '.';
			}
		}
	}
	merge[k] = '\0';

	print_result(new_str1, new_str2, merge, k);

	free(merge);
	free(new_str1);
	free(new_str2);
	return score;
}

int maximum(int max1, int max2, int max3) {
	int max = 0;
	if (max1 > max2) {
		max = max1;
	}
	else {
		max = max2;
	}
	if (max < max3) {
		max = max3;
	}
	if (max < 0) {
		return 0;
	}
	else {
		return max;
	}
}

int scoring(struct elem ** matrix, char * str1, char * str2, int m, int n) {
	assert(matrix != NULL);
	assert(str1 != NULL);
	assert(str2 != NULL);
	assert(m > 0 && n > 0);

	int idx = 0;
	int max = 0;
	for (int i = 1; i < m; i++) {
		for (int j = 1; j < n; j++) {
			int max1 = matrix[(i-1)*n + j - 1]->value + 2*EBLOSUM62[str1[i-1]][str2[j-1]];
			int max2 = 0;
			if (matrix[i*n + j - 1]->gap_open_left == 0) {
				max2 = matrix[i*n + j - 1]->value - Gap_penalty;
			} 
			else {
				max2 = matrix[i*n + j - 1]->value - Extend_penalty;
			}

			int max3 = 0;
			if (matrix[(i-1)*n + j]->gap_open_up == 0) {
				max3 = matrix[(i-1)*n + j]->value - Gap_penalty;
			} 
			else {
				max3 = matrix[(i-1)*n + j]->value - Extend_penalty;
			}

			matrix[i*n + j]->value = maximum(max1, max2, max3);

			if (matrix[i*n + j]->value == max2) {
				matrix[i*n + j]->gap_open_left = 1;
				matrix[i*n + j]->from = matrix[i*n + j - 1];
			}
			else {
				if (matrix[i*n + j]->value == max3) {
					matrix[i*n + j]->gap_open_up = 1;
					matrix[i*n + j]->from = matrix[(i-1)*n + j];
				}
				else {
					if (matrix[i*n + j]->value == max1) {
						matrix[i*n + j]->from = matrix[(i-1)*n + j - 1];
					}	
				}
			}

			if (max <= matrix[i*n + j]->value) {
				max = matrix[i*n + j]->value;
				idx = i*n + j;
			}
		}
	}

	print_matrix(str1, str2, matrix, m, n);
	return idx;
}

int main(int argc, char * argv[]) {
	FILE * data1 = NULL;
	FILE * data2 = NULL;
	if (argc == 3) {
        data1 = fopen(argv[1], "r");
        data2 = fopen(argv[2], "r");
        if (data1 == NULL) {
        	printf("File open error\n");
           	return 1;
        }
        if (data2 == NULL) {
        	printf("File open error\n");
        	fclose(data1);
           	return 1;
        }
    }
    else {
    	printf("Not arguments\n");
       	return 1;
    }

    char str1[MAX_length_str];
    char str2[MAX_length_str];
    fscanf(data1, "%100s", str1);
    fscanf(data2, "%100s", str2);
    int m = strlen(str1) + 1;
    int n = strlen(str2) + 1;
    struct elem ** matrix = (struct elem **)calloc(m*n, sizeof(struct elem));
    for (int i = 0; i < m*n; i++) {
    	matrix[i] = (struct elem *)calloc(1, sizeof(struct elem));
    }

    int idx = scoring(matrix, str1, str2, m, n);
	int score = trace(str1, str2, matrix, idx, m, n);
	printf("\nScore: %.1f\n", score/2.0);
  	free_matrix(matrix, m*n);
    fclose(data1);
    fclose(data2);
    return 0;
}