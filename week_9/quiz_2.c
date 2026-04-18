#include <stdio.h>

int get_arr(int arr[10][10], int n, int m);

int main(void){
	int arr [10][10];
	int n;
	
	
	printf("n : ");
	scanf("%d", &n);
	
	get_arr(arr, n, n);
	
	for(int i = 0; i < n; i++){
		
		for(int j = 0; j < n; j++ ){
			if (arr[i][j] !=  arr[j][i]){
				printf("It is not symmetric");
				return 0;
			}
		}
	}
	
	printf("It is symmetric");
	
	return 0;
}

int get_arr(int arr[10][10], int n, int m){
	printf("INPUT ARRAY\n");
	for(int i = 0; i < m; i++){
		printf("---- ROW %d ----\n", i + 1);
		
		for(int j = 0; j < n; j++ ){
			printf("ITEM %d : ", j + 1);
			scanf("%d", &arr[i][j]);
		}
		printf("\n");
	}
}
