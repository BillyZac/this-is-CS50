/**
 * Test halving a number with a while loop
 */
       
#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int number = 10;
    
    while (number > 0)
    {
        number = number / 2;
        printf("%d\n", number);
    }   
}