#include <stdio.h>

int main(void)
{
  int height = 0;
  int row, column;

  while ((height < 1) || (height > 32))
  {
    printf("How high? ");
    scanf("%d", &height);
  }

  for (row=1; row<=height; row++)
  {

    for (column=1; column<=height-row; column++)
    {
      printf(" ");
    }

    for (column=1; column<=row; column++)
    {
      printf("#");
    }

    printf("  ");

    for (column=1; column<=row; column++)
    {
      printf("#");
    }
    printf("\n");
  }

  return 0;
}
