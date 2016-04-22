/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include "helpers.h"
#include <stdio.h>

/**
 * BINARY SEARCH
 * Returns true if value is in array of n values, else false.
 */
 
bool search(int value, int values[], int n)
{
   int first = 0;
   int last = n - 1;
   int middle = (first + last) / 2;
 
   while (first <= last) 
   {
        if (values[middle] < value)
            first = middle + 1;    
         
        else if (values[middle] == value) {
            return true;
    }
    else
    {
        last = middle - 1;
    }
    middle = (first + last) / 2;
   }
   return false;  
}

/**
 * BUBBLE SORT
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int swapHappened;
    do
    {
        swapHappened = 0;
        for (int i = 0; i < n-1; i++)
        {
            if (values[i] > values[i + 1])
            {
                int temp = values[i + 1];
                values[i + 1] = values[i];
                values[i] = temp;
                swapHappened = 1;
            }
        }
    }
    while (swapHappened);
    
    return;
}