#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> 
#include <assert.h>
#include <string.h>
#include <time.h>

#define DIGIT 8

int check_neighbores (int x, int y, int height, int width) {
	int num = 0;
	if (x - 2 >= 0 && y - 1 >= 0) {
		num++;
	}
	if (x - 2 >= 0 && y + 1 < height) {
		num++;
	}
	if (x - 1 >= 0 && y - 2 >= 0) {
		num++;
	}
	if (x - 1 >= 0 && y + 2 < height) {
		num++;
	}
	if (x + 1 < width && y - 2 >= 0) {
		num++;
	}
	if (x + 1 < width && y + 2 < height) {
		num++;
	}
	if (x + 2 < width && y - 1 >= 0) {
		num++;
	}
	if (x + 2 < width && y + 1 < height) {
		num++;
	}
	return num;
}

void check_siblings (int x, int y, int height, int width, int * neighbores, int * coordinates, int * i, int * chess_desk) {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		coordinates[*i] = y*width + x;
		if (chess_desk[coordinates[*i]] != 0) {
			neighbores[*i] = 9;
		}
		else {
			neighbores[*i] = check_neighbores(x, y, height, width);
		}
		(*i)++;
	}
}

int move_horse (int height, int width, int idx, int num, int * chess_desk) {
	int neighbores[DIGIT];
	int coordinates[DIGIT];

	int x = idx % width;
	int y = idx / width;
	int i = 0;

	check_siblings(x - 2, y - 1, height, width, neighbores, coordinates, &i, chess_desk);
	check_siblings(x - 2, y + 1, height, width, neighbores, coordinates, &i, chess_desk);
	check_siblings(x - 1, y - 2, height, width, neighbores, coordinates, &i, chess_desk);
	check_siblings(x - 1, y + 2, height, width, neighbores, coordinates, &i, chess_desk);
	check_siblings(x + 1, y - 2, height, width, neighbores, coordinates, &i, chess_desk);
	check_siblings(x + 1, y + 2, height, width, neighbores, coordinates, &i, chess_desk);
	check_siblings(x + 2, y - 1, height, width, neighbores, coordinates, &i, chess_desk);
	check_siblings(x + 2, y + 1, height, width, neighbores, coordinates, &i, chess_desk);

	if (i == 0) {
		return -1;
	}

	int count = 0;
	int min = neighbores[0];
	for (int j = 1; j < i; j++) {
		if (min > neighbores[j]) {
			min = neighbores[j];
			count = j;
		}
	}
	if (min == DIGIT + 1) {
		return -1;
	}

	chess_desk[coordinates[count]] = ++num;
	if (num == height*width) {
		return 0;
	}

	while (move_horse(height, width, coordinates[count], num, chess_desk) == -1) {
		num--;
		chess_desk[coordinates[count]] = 0;
		neighbores[count] = DIGIT + 1;
		min = 9;
		for (int j = 0; j < i; j++) {
			if (min > neighbores[j]) {
				min = neighbores[j];
				count = j;
			}
		}
		if (min == 9) {
			return -1;
		}
		chess_desk[coordinates[count]] = ++num;
	}
	return 0;
}

int main () {
	int height = 0;
	int width = 0;
	printf("Please tepe the size of field\n");
	scanf("%d %d", &height, &width);
	int * chess_desk = (int *)calloc(height*width, sizeof(int));

	printf("Please type the coordinates of begining\n");
	int x, y;
	scanf("%d %d", &y, &x);
	chess_desk[(y - 1)*width + x - 1] = 1;

	if (move_horse(height, width, (y - 1)*width + x - 1, 1, chess_desk) == -1) {
		printf("Sorry, this way not exist\n");
	}
	else {
		for (int k = 0; k < height; k++) {
			for (int j = 0; j < width; j++) {
				printf("%2d ", chess_desk[k*width + j]);
			}
			printf("\n");
		}
	}

	free(chess_desk);
	return 0;
}