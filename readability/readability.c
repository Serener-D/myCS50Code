#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int calculateIndex(float avgLetters, float avgSentences);
void printGrade(int grade);

int main(void)
{
    string userInput = get_string("Text: ");
    int inputLength = strlen(userInput);
    float lettersCount = 0;
    float wordsCount = 1;
    float sentencesCount = 0;
    for (int i = 0; i < inputLength; i++)
    {
        if (userInput[i] == 32)
        {
            wordsCount++;
        }
        else if (userInput[i] == 33 || userInput[i] == 46 || userInput[i] == 32 || userInput[i] == 63)
        {
            sentencesCount++;
        }
        else if ((userInput[i] > 64 && userInput[i] < 91) || (userInput[i] > 96 && userInput[i] < 123))
        {
            lettersCount++;
        }
    }
    float avgLetters = (lettersCount / wordsCount) * 100;
    float avgSentences = (sentencesCount / wordsCount) * 100;
    int grade = calculateIndex(avgLetters, avgSentences);
    printGrade(grade);
}

int calculateIndex(float avgLetters, float avgSentences)
{
    return round(0.0588 * avgLetters - 0.296 * avgSentences - 15.8);
}

void printGrade(int grade)
{
    if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}