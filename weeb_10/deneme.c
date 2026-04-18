#include <stdio.h>

int deneme(int **A, int count);

int main(void){
  int row1[] = {1, 2, 3};
  int row2[] = {4, 5, 6};
  int *a[] = {
    row1,
    row2
  };

  deneme(a, 2);
  return 0;
}

int deneme(int **A, int count) {
  for(int i = 0; i < count; i++){
    for(int j = 0; j < 3; j++){
      printf("%d\t", A[i][j]);
    }
    printf("\n");
  }
}
