#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> 
#include <assert.h>
#include <string.h>

#define EXP 12
#define DOUBLE 64
#define EXP_BIAS 1023

long long bin (int * arr, int end, int begin) {
	long long num = 0;
	int j = 1;

	for (int i = begin; i < end; i++) {
		if (arr[i] == 1) {
			num += pow(2, end - begin - j);
		}
		j++;
	}

	return num;
}

double exponent (int * bit) {
	int exponent = bin(bit, EXP, 1);
	printf("Exponent: %d or ", exponent);
	for (int i = 1; i < 12; i++) {
		printf("%d", bit[i]);
	}
	printf("\n");
	printf("Exponent Bias: %d\n", exponent - EXP_BIAS);
	return exponent - EXP_BIAS;
}

double mantissa (int * bit) {
	printf("Fraction: 1.");
	for (int i = 12; i < DOUBLE; i++) {
		printf("%d", bit[i]);
	}
	printf("\n");
	double mantissa = 1.0;
	int k = -1;
	for (int i = 12; i < DOUBLE; i++) {
		if (bit[i] == 1) {
			mantissa += pow(2.0, k);
		}
		k--;
	}
	printf("Fraction: %lf\n", mantissa);
	return mantissa;

}

double sign (unsigned char * p) {
	char sign = *p >> 7;
	if (sign == 0) {
		printf("Sign: %d or 1\n", sign);
		return 1;
	}
	else {
		printf("Sign: %d or -1\n", sign);
		return -1;
	}
}

int main () {
	double num;
	printf("Please type the key\n");

	while (1) {
		fscanf(stdin, "%lf", &num);
		unsigned char * p = (unsigned char *)&num;
		int bit[DOUBLE];
		int k = 0;
		for (int i = 7; i >= 0; i--) {
			for (int j = 7; j >= 0; j--) {
				bit[k] = ((*(p + i)) >> j) % 2;
				k++;
			}
		}
		double value = sign(p + 7)*pow(2, exponent(bit))*mantissa(bit);
		printf("Original: %lf\n\n", value);
	}
	
	return 0;
}