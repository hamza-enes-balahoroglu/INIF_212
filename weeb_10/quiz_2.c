#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SIZE 100
int product(int (*A)[MAX_SIZE], int (*B)[MAX_SIZE], int (*result)[MAX_SIZE], int m, int n);

int main(void){
	int m_1 , n_1;
	int m_2 , n_2;
	
	printf("Array 1 m (row): ");
	scanf("%d", &m_1);

	printf("Array 1 n : ");
	scanf("%d", &n_1);

	printf("Array 2 m (row): ");
	scanf("%d", &m_2);
	
	printf("Array 2 n : ");
	scanf("%d", &n_2);
		
	int arr_1[m_1][n_1];
	int arr_2[m_2][n_2];
	
	printf("\nArray 1 INPUTS");
	for(int i = 0; i < m_1; i++){
		for(int j = 0; j < n_1; j++){
			printf("\ninput %d : ", i + 1);
			scanf("%d", &arr_1[i][j]);
		}

	}
	
	printf("\nArray 2 INPUTS");
	for(int i = 0; i < m_2; i++){
		for(int j = 0; j < n_2; j++){
			printf("\ninput %d : ", i + 1);
			scanf("%d", &arr_2[i][j]);			
		}	
	}
	
	if (n_1 != m_1){
		printf("Carpim yapilamaz. \n");
	}
	int n, m;
	
	if (n_1 > n_2){
		n = n_2;
	}
	else{
		n = n_1;
	}
	
	if (m_1 > m_2){
		m = m_2;
	}
	else{
		m = m_1;
	}
	
	int result[m][n];
	
	product(arr_1, arr_2, result, m, n);
	
	for (int i = 0; i < m; i++){
		for (int j = 0; j < n; j++){
			printf("\t %d", result[i][j]);
		}
		printf("\n");
	}
	
}

int product(int (*A)[MAX_SIZE], int (*B)[MAX_SIZE], int (*result)[MAX_SIZE], int m, int n){
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			int r = (*(*(A + i) + j) ) * (*(*(B + i) +j) );
			*(*(result + i) + j) = r;
		}
	}
}

