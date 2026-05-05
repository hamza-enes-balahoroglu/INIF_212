#include <stdio.h>


int *find_adress(int *A, int n, int value);

int main(void){
	int n;
	int number;
	printf("Array size : ");
	scanf("%d", &n);

		
	int arr[n];
	
	for(int i = 0; i < n; i++){
		printf("\ninput %d : ", i + 1);
		scanf("%d", &arr[i]);
	}
	
		
	printf("Find the : ");
	scanf("%d", &number);
	
	int *ptr = find_adress(arr, n, number);
	
	if(ptr != 0){
		printf("%d value is dedected on %p", *ptr, ptr );	
	}
	else{
		printf("Not Found.");
	}
	
}

int *find_adress(int *A, int n, int value){
	for(int i = 0; i < n; i++){
		if (*(A + i) == value){
			return (A + i);
		}
	}
	return 0;
}
