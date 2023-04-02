#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

string encode(string key, string plaintext);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    string key = argv[1];
    for (int i = 0; i < 26; i++)
    {
        key[i] = toupper(key[i]);
        if (key[i] < 65 || key[i] > 90)
        {
            printf("Key must contain only characters.\n");
            return 1;
        }
    }
    for (int i = 0; i < 26; i++)
    {
        for (int j = i + 1; j < 26; j++)
        {
            if (key[i] == key[j])
            {
                printf("Key should not contain duplicate characters.\n");
                return 1;
            }
        }
    }

    string plaintext = get_string("plaintext: ");
    string ciphertext = encode(key, plaintext);
    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

string encode(string key, string plaintext)
{
    char alphabet[26];
    int alphabetLetter = 97;
    for (int i = 0; i < 26; i++)
    {
        alphabet[i] = alphabetLetter;
        alphabetLetter++;
    }
    for (int i = 0; i < strlen(plaintext); i++)
    {
        for (int j = 0; j < 26; j++)
        {
            if (tolower(plaintext[i]) == alphabet[j])
            {
                if (tolower(plaintext[i]) == plaintext[i])
                {
                    plaintext[i] = tolower(key[j]);
                }
                else
                {
                    plaintext[i] = key[j];
                }
                break;
            }
        }
    }
    return plaintext;
}