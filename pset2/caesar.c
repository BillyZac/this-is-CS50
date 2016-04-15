#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Input validation
    if (argc != 2)
    {
        printf("One and only one argument.\n");
        return 1;
    }
    
    // Get the message to be encoded
    string plaintext = GetString();

    int key = atoi(argv[1]);
    
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        int encipheredChar = -1;
        
        if (isalpha(plaintext[i]))
        {
            int asciiValue = plaintext[i];
        
            if (isupper(asciiValue))
            {
                int alphaIndex = asciiValue - 65;
                int shiftedAlphaIndex = (alphaIndex + key) % 26;
                encipheredChar = shiftedAlphaIndex + 65;
            }
            
            if (islower(asciiValue))
            {
                int alphaIndex = asciiValue - 97;
                int shiftedAlphaIndex = (alphaIndex + key) % 26;
                encipheredChar = shiftedAlphaIndex + 97;
            }
        }
        else 
        {
            encipheredChar = plaintext[i];
        }
        printf("%c", encipheredChar);
    }
    printf("\n");
    
    return 0;
}

