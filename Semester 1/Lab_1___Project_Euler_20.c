#include <stdio.h>
#define NUM 100000
#define DECIMAL 10

int main (void) {

	int n = 0; // n - факториал n!
	int i = 0; // счетчик
	int j = 0; // счетчик
	unsigned int array[NUM] = {0}; 
	unsigned int sum = 0;

	array[0] = 1;
	scanf("%d", &n);	
											// пример: 5!
	// цикл записывает массив n!			//  a[0] = 1*2 = 2; a[0] = 2*3 = 6; 
	for (j = 2; j <= n; j++) {				//  a[0] = 6*4 = 24; a[0] = 4; a[1] = 0 + 2 = 2;
		for (i = 0; i < NUM; i++) {			//  a[0] = 4*5 = 20; a[0] = 0; a[1] = 2*5 + 2 = 12; a[1] = 2; a[2] = 0 + 1 = 1;
			array[i] *= j; 					//  5! = 120
		}
		for (i = 0; i < NUM; i++) { 							
			if (array[i] / DECIMAL > 0) { 			// в одном элементе должна находится одна цифра
				array[i+1] += array[i] / DECIMAL;  		 
				array[i] = array[i] % DECIMAL;
			}
		}
	}

	j = NUM - 1; 				// j - последний номер элемент
	while (array[j] == 0) { 	// если последний элемент равен 0, то уменьшаем количество элементов на 1
		j--;					// это необходимо, чтобы удалить незначащие нули
	}

	printf("\nFactorial: ");
	while (j >= 0) { 
			sum = sum + array[j];   
			printf("%d", array[j]); // склеиваем элементы массива, начиная с конца
			j--;
		}
		
	printf("\n\nSum of factorial digits: %d\n", sum

	return 0;
} 
