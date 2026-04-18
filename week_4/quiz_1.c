/*
 * Tarih : 10.03.2026
 * Soru  : Number % divisor islemini mod alma oparatoru(%) olmadan yapiniz.
 */

#include <math.h>
#include <stdio.h>


int main(void){
	int divisor, number;
	
	printf("Number  : ");
	scanf("%d", &number);
	
	printf("Divisor : ");
	scanf("%d", &divisor);
	
	int mod = number;
	
	while (!(mod < divisor)){
		mod -= divisor;
	}
	
	printf("Kalan   : %d", mod);
}
