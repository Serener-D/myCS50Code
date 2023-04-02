#include <cs50.h>
#include <stdio.h>

void printPyramid(int height);

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height <= 0 || height > 8);

    printPyramid(height);
}

void printPyramid(int height)
{
    int spacesCount = height - 1;
    int hashSymbolCount = 1;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < spacesCount; j++)
        {
            printf(" ");
        }
        spacesCount--;
        for (int j = 0; j < hashSymbolCount; j++)
        {
            printf("#");
        }
        printf("  ");
        for (int j = 0; j < hashSymbolCount; j++)
        {
            printf("#");
        }
        hashSymbolCount++;
        printf("\n");
    }
}