#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Make sure there's only one command line argument.
    if (argc != 2)
    {
        printf("Only one argument allowed!");
        return 1;
    }
    
    string key = argv[1];
    
    // Make sure the key is composed entirely of alphabetic characters.
    for (int i = 0; i < strlen(key); i++)
    {
        if (!isalpha(key[i]))
        {
            printf("The key cannot contain non-alphabetic characters!");
            return 1;
        }
    }
    
    string plaintext = GetString();
    
    int j = 0;
    
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        int encipheredChar = -1;
        int shift = 0;
        
        if (isalpha(plaintext[i]))
        {
            // Shift from the alphabet character range to a zero-indexed range, i.e. A=0, B=2, etc.
            // Handle both upper and lower cases.
            if (isupper(plaintext[i]))
            {
                shift = 65;
            }
            if (islower(plaintext[i]))
            {
                shift = 97;
            }
            int alphaIndex = plaintext[i] - shift;
            
            // Wrap around if we pass the end of the key.
            int keyIndex = j % strlen(key);
            
            // Get the cipher shift amount for the jth character in the key.
            int cipherShift = 0;
            if (isupper(key[keyIndex]))
            {
                cipherShift = key[keyIndex] - 65;       
            }
            if (islower(key[keyIndex]))
            {
                cipherShift = key[keyIndex] - 97;       
            }
            
            // Encipher the character. First shift by cipherAmount, then shift back into the alphabet character range.
            int shiftedAlphaIndex = (alphaIndex + cipherShift) % 26;
            encipheredChar = shiftedAlphaIndex + shift;
            
            // Move to the next character in the key.
            j++;
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