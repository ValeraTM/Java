#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DISTANCE 1 //расстояние до соседа
#define ON_CELL 2 //включенная клетка
#define OFF_CELL 0 //выключенная клетка
#define DEATH_CELL 1 //мертвая клетка

/* 
	Входной файл вида 
	#R Oabc 
	#Size height width
	2obd$5d3b5o!

	a, b, c, ... - количество соседей, необходимый для включения
	height - высота поля, width - ширина поля
	o - включенная клетка, b - выключенная клетка, d - мертвая клетка
	$ - переход на новую строку
	Числа вида 5d означают, что d повторяем 5 раз, т.е. 5d == ddddd
*/


//Тороидальное поле
void calculate_coordinates_of_neighbores (int *up_height, int *down_height, int *up_width, int *down_width, int i, int j, int height, int width) {
	*up_height = i - DISTANCE; //предыдущая строка
	*down_height = i + DISTANCE; //следующая строка
	*up_width = j - DISTANCE; //предыдущий столбец
	*down_width = j + DISTANCE; //следующий столбец

	if (*up_height < 0) {			
		*up_height = height - 1;
	}
	
	if (*down_height == height) {
		*down_height = 0;
	}
	
	if (*up_width < 0) {
		*up_width = width - 1;
	}
	
	if (*down_width == width) {
		*down_width = 0;
	}
}

//Количество включенных соседей
void calculate_of_ON_neighbores (int *neighbores, char *life, int up_height, int down_height, int up_width, int down_width, int i, int j, int height, int width) {
	*neighbores = 0;
	if (*(life + up_height*width + up_width) == ON_CELL)
		*neighbores += 1;
			
	if (*(life + up_height*width + j) == ON_CELL)
		*neighbores += 1;
			
	if (*(life + up_height*width + down_width) == ON_CELL)
		*neighbores += 1;
			
	if (*(life + i*width + up_width) == ON_CELL)
		*neighbores += 1;
			
	if (*(life + i*width + down_width) == ON_CELL)
		*neighbores += 1;
			
	if (*(life + down_height*width + up_width) == ON_CELL)
		*neighbores += 1;
			
	if (*(life + down_height*width + j) == ON_CELL)
		*neighbores += 1;
			
	if (*(life + down_height*width + down_width) == ON_CELL)
		*neighbores += 1;
}

//вычисление следующего поколения
void calculate_next_generation(char *life, char *new_life, int neighbores, int *rule) {
	int i = 0;
	int j = 0; 	
	if (*life == ON_CELL) {
		//если клетка включена, то она умирает
		*new_life = DEATH_CELL;
	}		

	if (*life == DEATH_CELL) {
		*new_life = OFF_CELL;
	}		

	if (*life == OFF_CELL) {
		//если 2 включенных соседа, то клетка включается
		for (i = 0; i < 9; i++) {
			if (neighbores == *(rule + i)) {
				*new_life = ON_CELL;
			}
			else {
				j++;
			}
		}
		if (j == 9) {
			*new_life = OFF_CELL;
		}
	}	
}

//следующее поколение становится нынешним
void copy (char *life, char *new_life, int height, int width) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			*(life + i*width + j) = *(new_life + i*width + j); 
		}
	}
}

//вывод поколения
void show_generation (char *life, int height, int width) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (*(life + i*width + j) == ON_CELL) {
				printf("*");
			}
			if (*(life + i*width + j) == OFF_CELL) {
				printf(" ");
			}
			if (*(life + i*width + j) == DEATH_CELL) {
				printf(" ");
			}
		}
		printf("\n");
	} 
}

//проверяет, остались ли включенные клетки 
void check (char *life, int *death, int height, int width) {
	*death = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (*(life + i*width + j) == ON_CELL) {
				(*death)++;
			}
		}
	}
}

//чтение основных характеристик
void read_rule (int *height, int *width, int *rule, FILE *data) {
	char *file = (char*)calloc(10000, sizeof(char));
	int i = 0;
	int j = 0;
	int result = 0;
	int count = 0;
	int k = 0;

	for (i = 0; i < 10000; i++) {
		*(file + i) = fgetc(data);
	}

	i = 0;
	while (*(file + i) != '!') {
		if ((*(file + i) == '#') && (*(file + i + 1) == 'R')) {
			while (*(file + i) != '\n') {
				result = 0;
				while (!isdigit(*(file + i))) {
					i++;
				}
				if (isdigit(*(file + i))) {
					result = *(file + i) - '0';
				}
				*(rule + j) = result;
				i++;
				j++;
			}	
			k = j - 1;
			for (j; j < 9; j++) {
				*(rule + j) = *(rule + k);
			}
			k = 0;
		}
		
		if ((*(file + i) == '#') && (*(file + i + 1) == 'S')) {
			while (*(file + i) != '\n') {
				result = 0;
				while (!isdigit(*(file + i))) {
					i++;
				}
				while (isdigit(*(file + i))) {
					result *= 10;
					result += *(file + i) - '0';
					i++;
				}
				count++;
				if (count == 1) {
					*height = result;
				}
				else {
					*width = result;
				}		
			}
		}
		i++;
	}
	free(file);
}

//считывает информацию с файла
void read (int height, int width, char *life, FILE *data, char *new_life) {
	int k = 0; 
	int amt = 0; //количество $
	int i = 0;
	int result = 1; // количество одинаковых значений клетки
	int j = 0; 
	int p = 0; // счетчик заполнения основного массива
	int count = 0; // индекс начала паттерна
	
	for (i = 0; i < height*width; i++) {
		*(new_life + i) = fgetc(data);
	}

	i = 0;
	while (*(new_life + i) != '!') {
		if (*(new_life + i) == '#') {
			while (*(new_life + i) != '\n') {
				i++;
			}
			count = i + 1;		
		}
		i++;
	}
	
	i = count;
	while (*(new_life + i) != '!') {
		if (isdigit(*(new_life + i))) {
			result = 0;
			while (isdigit(*(new_life + i))) {
				result *= 10;
				result += *(new_life + i) - '0';
				i++;
			}			
		}

		if (*(new_life + i) == '$') {
			amt = 0;
			for (j = 0; j <= i; j++) {
				if (*(new_life + j) == '$') {
					amt++;
				}
			}	
			i++;
			k = p;
		}

		if (*(new_life + i) == 'o') {
			for (j = 0; j < result; j++) {
				*(life + amt*width + p - k) = ON_CELL;
				p++;				
			}	
			i++;
			result = 1;
		}		
			
		if (*(new_life + i) == 'b') {
			for (j = 0; j < result; j++) {
				*(life + amt*width + p - k) = OFF_CELL;
				p++;
			}	
			i++;
			result = 1;
		}

		if (*(new_life + i) == 'd') {
			for (j = 0; j < result; j++) {
				*(life + amt*width + p - k) = DEATH_CELL;
				p++;
			}	
			i++;
			result = 1;
		}
	}
}

//проверяет, существует ли поле
void check_field (int *height, int *width) {
	if ((*height == 0) || (*width == 0)) {
		*height = 20;
		*width = 40;
	}
}


int main (int argc, char* argv[]) {
	int rule[9] = {0}; //количество соседей, необходимых для включения
	int height = 0; //строки
	int width = 0; //столбцы
	int up_height = 0; //предыдущая строка
	int down_height = 0; //следующая строка
	int up_width = 0; //предыдущий столбец
	int down_width = 0; //следующий столбец
	int neighbores = 0; //количество живых соседей
	int death = 1;
	int i = 0; //счетчик
	int j = 0; //счетчик
	int count = 0;

	FILE *data;
	if (argc > 1) {
        data = fopen(argv[1], "r");
    }
   	else{
        printf("Not arguments");
        exit(EXIT_FAILURE);
    }
    
    read_rule(&height, &width, rule, data);
	check_field(&height, &width);

	char *life = (char*)calloc(height*width, sizeof(char));
	char *new_life = (char*)calloc(height*width, sizeof(char));
	
	read(height, width, life, data, new_life);
	printf("Please press <ENTER>");	
	
	while((getchar()) && (death != 0)) {
		show_generation(life, height, width);

		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				calculate_coordinates_of_neighbores(&up_height, &down_height, &up_width, &down_width, i, j, height, width);
				calculate_of_ON_neighbores(&neighbores, life, up_height, down_height, up_width, down_width, i, j, height, width);
				calculate_next_generation(life + i*width + j, new_life + i*width + j, neighbores, rule);
			}
		}

		copy(life, new_life, height, width);
		check(life, &death, height, width);
		count++;
	}

	free(life);
	free(new_life);
	fclose(data);
	printf("Generated %d form", count);

	return 0;
}