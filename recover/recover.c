#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("You should pass excatly one argument\n");
        return 1;
    }
    FILE *rawFile = fopen(argv[1], "r");
    if (rawFile == NULL)
    {
        printf("Forensic image cannot be opened\n");
        return 1;
    }

    int newFilesCounter = -1;
    char newFileName[8];

    // reading Raw File to a buffer
    FILE *newFile;
    BYTE buffer[BLOCK_SIZE];
    while (fread(buffer, 1, BLOCK_SIZE, rawFile) == BLOCK_SIZE)
    {
        if (buffer[0] == 255 && buffer[1] == 216 && buffer[2] == 255 && buffer[3] >= 224)
        {
            if (newFilesCounter != -1)
            {
                fclose(newFile);
            }
            newFilesCounter++;
            sprintf(&newFileName[0], "0%i%i.jpg", newFilesCounter / 10, newFilesCounter % 10);
            newFile = fopen(&newFileName[0], "a");
        }
        if (newFilesCounter != -1)
        {
            fwrite(&buffer[0], 1, BLOCK_SIZE, newFile);
        }
    }
    fclose(newFile);
    fclose(rawFile);
}
