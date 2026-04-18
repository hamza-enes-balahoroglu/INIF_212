#include <stdio.h>

int print_arr(int arr[10][10], int n, int m);
int get_arr(int arr[10][10], int n, int m);
int swap(int arr[10][10], int n, int m, int c_1, int c_2);

int main(void){
	
	int arr[10][10];
	int new_arr[10][10];
	int n,m;// m dikey n yatay
	
	int c_1, c_2;
	
	printf("n : ");
	scanf("%d", &n);
	
	printf("m : ");
	scanf("%d", &m);
	
	printf("c_1 : ");
	scanf("%d", &c_1);
	
	printf("c_2 : ");
	scanf("%d", &c_2);
	
	get_arr(arr, n, m);
	
	printf("**** ORIGINAL ****\n");
	print_arr(arr, n, m);
	
	
	swap(arr, n, m, c_1, c_2);
	printf("**** SWAPPED ****\n");
	print_arr(arr, n, m);
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

int print_arr(int arr[10][10], int n, int m){
	for(int i = 0; i < m; i++){
		printf("|\t");
		for(int j = 0; j < n; j++ ){
			printf("%d\t", arr[i][j]);
		}
		printf("|\n");
	}
}

int swap(int arr[10][10], int n, int m, int c_1, int c_2){
	for(int i = 0; i < m; i++){
		int temp = arr[i][c_1 - 1];
		
		arr[i][c_1 - 1] = arr[i][c_2 - 1];
		arr[i][c_2 - 1] = temp;
	}
}
