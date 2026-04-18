#include <stdio.h>

int main(void) {
  float grade_1;
  float grade_2;

  printf("1. Notu giriniz : ");
  scanf("%f", &grade_1);

  printf("2. Notu giriniz : ");
  scanf("%f", &grade_2);

  float avarege = 0.4 * grade_1 + 0.6 * grade_2;

  if (grade_2 < 12) {
    printf("Failed");
    return 0;
  }

  if (avarege > 70) {
    printf("Passed\n");
  } else {
    printf("Failed");
    return 0;
  }

  if (avarege >= 70 && avarege < 80) {
    printf("BB");
  } else if (avarege >= 80 && avarege < 90) {
    printf("BA");
  } else if (avarege >= 90 && avarege <= 100) {
    printf("AA");
  }
}
