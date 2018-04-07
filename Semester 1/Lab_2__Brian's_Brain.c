#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define DISTANCE 1 //расстояние до соседа
#define ON_CELL 2 //включенная клетка
#define OFF_CELL 0 //выключенная клетка
#define DEATH_CELL 1 //мертвая клетка
#define ON "1" //включенная клетка
#define OFF " " //выключенная клетка
#define DEATH " " //мертвая клетка

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
void calculate_coordinates_of_neighbores (int *up_height, int *down_height, int *up_width, int *down_width, int i, int j, const int height, const int width) {
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
void calculate_of_ON_neighbores (int *neighbores, const char *life, int up_height, int down_height, int up_width, int down_width, int i, int j, const int height, const int width) {
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
void calculate_next_generation(const char *life, char *new_life, int neighbores, const int *rule) {
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

//вывод поколения
void show_generation (const char *life, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (*(life + i*width + j) == ON_CELL) {
                printf(ON);
            }
            if (*(life + i*width + j) == OFF_CELL) {
                printf(OFF);
            }
            if (*(life + i*width + j) == DEATH_CELL) {
                printf(DEATH);
            }
        }
        printf("\n");
    } 
}

//проверяет, остались ли включенные клетки 
void check (const char *life, int *death, int height, int width) {
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
    int j = 0;
    int result = 0;
    int count = 0;
    int k = 0;
    char symbol = 0;

    if ((symbol = fgetc(data)) == '#' && (symbol = fgetc(data)) == 'R') {
        while (symbol != '\n') {
            result = 0;
            while (!isdigit(symbol)) {
                symbol = fgetc(data);
            }

            if (isdigit(symbol)) {
                result = symbol - '0';
            }

            *(rule + j) = result;
            symbol = fgetc(data);
            j++;
        }   

        k = j - 1;
        for (j; j < 9; j++) {
            *(rule + j) = *(rule + k);
        }
    }

    if ((symbol = fgetc(data)) == '#' && (symbol = fgetc(data)) == 'S') {
        while (symbol != '\n') {
            result = 0;
            while (!isdigit(symbol)) {
                symbol = fgetc(data);
            }
            while (isdigit(symbol)) {
                result *= 10;
                result += symbol - '0';
                symbol = fgetc(data);
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
}

//считывает паттерн с файла
void read (int height, int width, char *life, FILE *data) {
    int amt = 0; //количество $
    int result = 1; // количество одинаковых значений клетки
    int p = 0; // счетчик
    
    char symbol = 0;
    while ((symbol = fgetc(data)) != '!') {  
        if (isdigit(symbol)) {
            result = 0;
            while (isdigit(symbol)) {
                result *= 10;
                result += symbol - '0';
                symbol = fgetc(data);
            }           
        }

        if (symbol == '$') {
            amt++;
            p = 0;
        }

        if (symbol == 'o') {
            for (int j = 0; j < result; j++) {
                *(life + amt*width + p) = ON_CELL;
                p++;                
            }   
            result = 1;
        }       
            
        if (symbol == 'b') {
            for (int j = 0; j < result; j++) {
                *(life + amt*width + p) = OFF_CELL;
                p++;
            }   
            result = 1;
        }

        if (symbol == 'd') {
            for (int j = 0; j < result; j++) {
                *(life + amt*width + p) = DEATH_CELL;
                p++;
            }   
            result = 1;
        }
    }
}

void work (char * life, char * new_life, int height, int width, int *rule) {
    int up_height = 0; //предыдущая строка
    int down_height = 0; //следующая строка
    int up_width = 0; //предыдущий столбец
    int down_width = 0; //следующий столбец
    int neighbores = 0; //количество живых соседей
    int death = 1;
    int count = 0;
    printf("Please press <ENTER>"); 
    while((getchar()) && (death != 0)) {
        show_generation(life, height, width);

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                calculate_coordinates_of_neighbores(&up_height, &down_height, &up_width, &down_width, i, j, height, width);
                calculate_of_ON_neighbores(&neighbores, life, up_height, down_height, up_width, down_width, i, j, height, width);
                calculate_next_generation(life + i*width + j, new_life + i*width + j, neighbores, rule);
            }
        }

        memcpy(life, new_life, height*width);
        check(life, &death, height, width);
        count++;
    }
    printf("Generated %d form", count);
}

int main (int argc, char* argv[]) {
    int rule[9] = {0}; //количество соседей, необходимых для включения
    int height = 20; //строки
    int width = 40; //столбцы
    FILE *data;
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
    
    read_rule(&height, &width, rule, data);
    char *life = (char*)calloc(height*width, sizeof(char));
    char *new_life = (char*)calloc(height*width, sizeof(char));
    read(height, width, life, data);  
    work(life, new_life, height, width, rule);
    
    free(life);
    free(new_life);
    fclose(data);
    
    return 0;
}