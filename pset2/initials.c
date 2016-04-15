#include <cs50.h>
#include <stdio.h>
#include <string.h>

void printUppercaseCharacter(char character)
{
    if (character >= 'a' && character <= 'z')
    {
        printf("%c", character - 32);
    }
    else
    {
        printf("%c", character);
    }
}

int main(void)
{
    string s = GetString();
    
    char initial = s[0];
    printUppercaseCharacter(initial);
    
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] == ' ')
        {
            initial = s[i+1];
            printUppercaseCharacter(initial);
        }
    }
    printf("\n");
}