/*
 * Tarih : 03.03.2026
 * Hafta : 3
 * Girilen sayinin 3 ve 7 ile bolunup bolunmedigini kontrol eden kodu yaziniz.
 */

#include <stdio.h>
#include <math.h>

int main(void){
	
	int num;
	printf("Sayi gir : ");
	scanf("%d", &num);
	
	if (num % 3 == 0 && num % 7 == 0){
		printf("Hem 3 hem 7 ile bolunur.");
	}
	else{
		printf("Hem 3 hem 7 ile bolunmez.");
	}
}
