#include <stdio.h>

int shower_duration, bottles;

int main(void)
{
  while (shower_duration < 1)
  {
    printf("How long do you shower? ");
    scanf("%d", &shower_duration);
  }

  bottles = shower_duration * 192 / 16;

  printf("You shower with %d bottles of water.\n", bottles);
}
