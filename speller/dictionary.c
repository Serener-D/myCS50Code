// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

void addNode(char word[]);

// TODO: Choose number of buckets in hash table
const unsigned int N = 1024;

// Hash table
node *table[N];

// number of words in dictionary
int WORDS_IN_DICTIONARY_COUNTER;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);
    int result = -1;
    node *next = table[index];
    while (next != NULL && result != 0)
    {
        result = strcasecmp(word, next->word);
        next = next->next;
    }
    return result == 0;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int sum = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        sum += tolower(word[i]);
    }
    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    char word[LENGTH + 1];
    int index = 0;
    char c;
    while (fread(&c, sizeof(char), 1, file))
    {
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            word[index] = c;
            index++;
        }
        else
        {
            word[index] = '\0';
            addNode(word);
            index = 0;
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return WORDS_IN_DICTIONARY_COUNTER;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *temp = table[i];
        while (table[i] != NULL && table[i]->next != NULL)
        {
            free(temp);
            table[i] = table[i]->next;
            temp = table[i];
        }
        free(temp);
    }
    return true;
}

void addNode(char word[])
{
    node *newNode = malloc(sizeof(node));
    strcpy(newNode->word, word);
    int index = hash(newNode->word);
    node *head = table[index];
    newNode->next = table[index];
    table[index] = newNode;
    WORDS_IN_DICTIONARY_COUNTER++;
}
