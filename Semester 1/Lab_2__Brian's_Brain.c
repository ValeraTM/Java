#include <stdio.h>
#include <stdlib.h>

#define DISTANCE 1 //расстояние до соседа
#define ON_CELL 2 //включенная клетка
#define OFF_CELL 0 //выключенная клетка
#define DEATH_CELL 1 //мертвая клетка
#define HEIGHT 10 //строки
#define WIDTH 20 //столбцы

//Тороидальное поле
void toroidal (int *up_height, int *down_height, int *up_width, int *down_width, int i, int j) {
	*up_height = i - DISTANCE; //предыдущая строка
	*down_height = i + DISTANCE; //следующая строка
	*up_width = j - DISTANCE; //предыдущий столбец
	*down_width = j + DISTANCE; //следующий столбец

	if (*up_height < 0) {			
		*up_height = HEIGHT - 1;
	}
	
	if (*down_height == HEIGHT) {
		*down_height = 0;
	}
	
	if (*up_width < 0) {
		*up_width = WIDTH - 1;
	}
	
	if (*down_width == WIDTH) {
		*down_width = 0;
	}
}

//Количество включенных соседей
void live_neighbores (int *neighbores, int *life, int up_height, int down_height, int up_width, int down_width, int i, int j) {
	*neighbores = 0;
	if (*(life + up_height*WIDTH + up_width) == ON_CELL)
		*neighbores += 1;
			
	if (*(life + up_height*WIDTH + j) == ON_CELL)
		*neighbores += 1;
			
	if (*(life + up_height*WIDTH + down_width) == ON_CELL)
		*neighbores += 1;
			
	if (*(life + i*WIDTH + up_width) == ON_CELL)
		*neighbores += 1;
			
	if (*(life + i*WIDTH + down_width) == ON_CELL)
		*neighbores += 1;
			
	if (*(life + down_height*WIDTH + up_width) == ON_CELL)
		*neighbores += 1;
			
	if (*(life + down_height*WIDTH + j) == ON_CELL)
		*neighbores += 1;
			
	if (*(life + down_height*WIDTH + down_width) == ON_CELL)
		*neighbores += 1;
}

//вычисление следующего поколения
void transition_rule(int *life, int *new_life, int neighbores) { 
	if (*life == OFF_CELL) {
		//если 2 включенных соседа, то клетка включается
		if (neighbores == 2) {
			*new_life = ON_CELL;
		}
		else {
			*new_life = OFF_CELL;
		}
	}	
	if (*life == ON_CELL) {
		//если клетка включена, то она умирает
		*new_life = DEATH_CELL;
	}								
}

//следующее поколение становится нынешним
void change (int *life, int *new_life) {
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			*(life + i*WIDTH + j) = *(new_life + i*WIDTH + j); 
		}
	}
}

//вывод поколения
void output_generation (int *life) {
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (*(life + i*WIDTH + j) == ON_CELL) {
				printf("*");
			}
			else {
				printf(" ");
			}
		}
		printf("\n");
	} 
}

//проверяет, остались ли включенные клетки 
void check (int *life, int *death) {
	*death = 0;
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (*(life + i*WIDTH + j) == ON_CELL) {
				(*death)++;
			}
		}
	}
}

int main () {
	int up_height = 0; //предыдущая строка
	int down_height = 0; //следующая строка
	int up_width = 0; //предыдущий столбец
	int down_width = 0; //следующий столбец
	int neighbores = 0; //количество живых соседей
	int death = 1;
	int i = 0; //счетчик
	int j = 0; //счетчик
	int *life = (int*)calloc(HEIGHT*WIDTH, sizeof(int));
	int *new_life = (int*)calloc(HEIGHT*WIDTH, sizeof(int));
	
	while((getchar()) && (death != 0)) {
		output_generation(life);
		for (i = 0; i < HEIGHT; i++) {
			for (j = 0; j < WIDTH; j++) {
				toroidal(&up_height, &down_height, &up_width, &down_width, i, j);
				live_neighbores(&neighbores, life, up_height, down_height, up_width, down_width, i, j);
				transition_rule(life + i*WIDTH + j, new_life + i*WIDTH + j, neighbores);
			}
		}
		change(life, new_life);
		check(life, &death);
	}

	free(life);
	free(new_life);

	return 0;
} 
