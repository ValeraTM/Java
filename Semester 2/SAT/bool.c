#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> 
#include <assert.h>
#include <string.h>
#include <time.h>

int check_formula (int m, int n, int * formula, int count, int * values) {
	int new_form[m*n];
	memcpy(new_form, formula, m*n*sizeof(int));

	for (int i = 0; i < m; i++) {
		if (formula[i*n + count] == values[count]*(count + 1)) {
			memset(new_form + i*n + count + 1, 0, (n - count - 1)*sizeof(int));
			continue;
		}
		if (formula[i*n + count] == -1*values[count]*(count + 1)) {
			new_form[i*n + count] = 0;
		}
	}
	
	int OK = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (new_form[i*n + j] == 0) {
				OK++;
			}
		}
		if (OK == n) {
			return -1;
		}
		OK = 0;
	}

	memcpy(formula, new_form, m*n*sizeof(int));
	return 0;
}

int unit_propagation (int m, int n, int count, int * new_form, int * values) {
	int k = -1;
	int OK = 0;
	for (int i = 0; i < m; i++) {
		for (int j = count; j < n; j++) {
			if (new_form[i*n + j] != 0) {
				OK++;
				k = j;
			}
		}
		if (OK == 1) {
			values[k] = new_form[i*n + k]/(k + 1);
			if (check_formula(m, n, new_form, k, values) == -1) {
				values[k] = 0;
				return -1;
			}
			OK = 0;
		}
	}
	return 0;
}

void pure_literal_elimination (int m, int n, int count, int * new_form, int * values) {
	int j = 0;
	for (int i = count; i < n; i++) {
		while (j < m) {
			if (new_form[j*n + i] == 0) {
				j++;
			}
			else {
				break;
			}
		}
		int k = j++;
		while (j < m) {
			if (new_form[k*n + i] == new_form[j*n + i] || new_form[j*n + i] == 0) {
				j++;
			}
			else {
				break;
			}
		}
		if (j == m) {
			values[i] = new_form[k*n + i]/(i + 1);
			check_formula(m, n, new_form, i, values);
		}
	}
}

int satisfy (int m, int n, int * formula, int * values, int count) {
	if (count == n) {
		return 0;
	}

	if (unit_propagation(m, n, count, formula, values) == -1) {
		return -1;
	}
	pure_literal_elimination(m, n, count, formula, values);

	if (values[count] == 0) {
		values[count] = 1;
	}
	
	if (check_formula(m, n, formula, count, values) == -1) {
		values[count] = -1*values[count];
		if (check_formula(m, n, formula, count, values) == -1) {
			return -1;
		}
	}

	if (satisfy(m, n, formula, values, count + 1) == -1) {
		values[count] = -1*values[count];
		if (check_formula(m, n, formula, count, values) == -1) {
			values[count] = 0;
			return -1;
		}
		if (satisfy(m, n, formula, values, count + 1) == -1) {
			return -1;
		}
	}
	return 0;
}

void read (FILE * data, int * formula, int n) {
	int i = 0;
	int j = 0;
	while (1) {
		int num = 0;
		int rc = 0;
		rc = fscanf(data, "%d", &num);
		if (rc == -1) {
			break;
		}
		if (num == 0) {
			j++;
			i = 0;
		}
		else {
			i = abs(num);
			i--;
			formula[j*n + i] = num;
		}
		
	}
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

	int n = 0;
	int m = 0;
	fscanf(data, "p cnf %d %d", &n, &m);
	int * formula = (int *)calloc(m*n, sizeof(int));
	int * values = (int *)calloc(n, sizeof(int));

	read(data, formula, n);

	if (satisfy(m, n, formula, values, 0) == -1) {
		printf("This formula is not satisfy\n");
	}
	else {
		printf("This formula is satisfy\n");
		for (int i = 0; i < n; i++) {
			printf("x%d = %2d\n", i + 1, values[i]);
		}
	}

	free(formula);
	free(values);
	fclose(data);
	return 0;	
}