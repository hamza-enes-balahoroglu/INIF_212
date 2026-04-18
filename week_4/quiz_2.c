/*
 * Tarih : 10.03.2026
 * Soru  : 
 */

#include <math.h>
#include <stdio.h>

int ust_al(int x, int y){
	int result = 1;
	for (int i = 0; i < y; i++){
		result *= x;
	}
	return result;
}

int main(void){
	
	int number;
	
	printf("Input : ");
	scanf("%d", &number);
	int temp = number;
	
	int basamak = 1;
	
	while (number % (int)pow(10.0, basamak) != number){
		basamak += 1;
	}
	printf("Basamak : %d", basamak);
	
	int sum = 0;
	while (number > 0){
		sum += ust_al((number % 10), basamak);
		number = (int) (number / 10);
	}
	
	printf("%d", sum);
	
	if (sum == temp){
		printf("\n %d is an armstong number.", temp);
	}
	else {
		printf("\n %d is not armstong number.", temp);
	}
}


