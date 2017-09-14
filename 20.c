#include <stdio.h>

int main (void)
{
	int n, i, j; // n - факториал n!
	const unsigned int num = 100000; //количество элементов
	unsigned int a[num];
	unsigned int sum = 0;
	scanf("%d", &n);
	
	a[0] = 1;
	for (i = 1; i < num; i++)
		a[i] = 0; //обнуляем элементы массива, кроме первого
	for (j = 1; j <= n; j++) // 
		{			
			for (i = 0; i < num; i++)
				a[i] *= j;
			for (i = 0; i < num; i++)
				{
					if (a[i] / 10 > 0) // в одном элементе должна находится одна цифра
					{			
						a[i+1] += a[i]/10;
						a[i] = a[i] % 10;
					}
				}
		}

	j = num - 1; // j - последний номер элемент
	while (a[j] == 0) // если последний элемент равен 0, то уменьшаем количество элементов на 1
		j--;	// это необходимо, чтобы удалить незначащие нули

	printf("\nFactorial: ");
	while (j >= 0)
		{
			sum = sum + a[j];
			printf("%d", a[j]);
			j--;
		}
		
	printf("\n\nSum of factorial digits: %d\n", sum);

	return 0;
} 