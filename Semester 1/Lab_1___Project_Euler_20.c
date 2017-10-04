#include <stdio.h>
#include <stdlib.h>
#define NUM 100000
#define DECIMAL 10

// пример: 5!
//  a[0] = 1*2 = 2; a[0] = 2*3 = 6;
//  a[0] = 6*4 = 24; a[0] = 4; a[1] = 0 + 2 = 2
//  a[0] = 4*5 = 20; a[0] = 0; a[1] = 2*5 + 2 = 12; a[1] = 2; a[2] = 0 + 1 = 1;
//  5! = 120

//удаление незначащих нулей
void insignificant_zeros (int *array, int *j) {
	*j = NUM - 1; 				// j - последний номер элемент
	while (*(array + *j) == 0) { // если последний элемент равен 0, то уменьшаем количество элементов на 1
		(*j)--;					// это необходимо, чтобы удалить незначащие нули
	}
}

//вычисление факториала
void factorial_calculation (int *array, int n) {
	// цикл записывает массив n!			 
	for (int j = 2; j <= n; j++) {				
		for (int i = 0; i < NUM; i++) {			
			*(array + i) *= j; 					
		}
<<<<<<< HEAD
		for (int i = 0; i < NUM; i++) { 							
			if (*(array + i) / DECIMAL > 0) { // в одном элементе должна находится одна цифра
				*(array + i + 1) += *(array + i) / DECIMAL;  		 
				*(array + i) = *(array + i) % DECIMAL;
=======
		for (i = 0; i < NUM; i++) { 							
			if (array[i] / DECIMAL > 0) { 			// в одном элементе должна находится одна цифра
				array[i+1] += array[i] / DECIMAL;  		 
				array[i] = array[i] % DECIMAL;
>>>>>>> 30f22097949d4c25fa73a124e5764b3283e8127b
			}
		}
	}
}

//функция вывода
void output (int j, int *array) {
	int sum = 0;
	printf("\nFactorial: ");
	while (j >= 0) { 
			sum += *(array + j);   
			printf("%d", *(array + j)); // склеиваем элементы массива, начиная с конца
			j--;
		}
		
<<<<<<< HEAD
	printf("\n\nSum of factorial digits: %d\n", sum);
}
=======
	printf("\n\nSum of factorial digits: %d\n", sum
>>>>>>> 30f22097949d4c25fa73a124e5764b3283e8127b

int main () {

	int n = 0; // n - факториал n!
	int j = 0; // счетчик
	int *array = (int*)calloc(NUM, sizeof(int));
	*(array) = 1;

	scanf("%d", &n);	
	factorial_calculation(array, n);	
	insignificant_zeros(array, &j);										
	output(j, array);
	
	free(array);
	return 0;
} 
