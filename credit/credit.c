#include <cs50.h>
#include <stdio.h>

void checkValidity(long creditCardNumber);
int countDigitsSum(long number);
void reverseIfOddCreditNumber(int digitsSum, int firstTwoDigits[2]);
void printResult(int digitsSum, int digitsCount, int firstTwoDigits[2]);

int main(void)
{
    long creditCardNumber;
    do
    {
        creditCardNumber = get_long("Number: ");
    }
    while (creditCardNumber <= 0);
    checkValidity(creditCardNumber);
}

void checkValidity(long creditCardNumber)
{
    int firstTwoDigits[2];
    int digitsCount = 0;
    int digitsSum = 0;
    do
    {
        int digit = creditCardNumber % 10;
        if (digitsCount % 2 != 0)
        {
            firstTwoDigits[0] = digit;
            digitsSum += countDigitsSum(digit * 2);
        }
        else
        {
            firstTwoDigits[1] = digit;
            digitsSum += digit;
        }
        creditCardNumber = creditCardNumber / 10;
        digitsCount++;
    }
    while (creditCardNumber != 0);
    reverseIfOddCreditNumber(digitsCount, firstTwoDigits);
    printResult(digitsSum, digitsCount, firstTwoDigits);
}

int countDigitsSum(long number)
{
    int digitsSum = 0;
    do
    {
        int digit = number % 10;
        digitsSum += digit;
        number = number / 10;
    }
    while (number != 0);
    return digitsSum;
}

void reverseIfOddCreditNumber(int digitsCount, int firstTwoDigits[2])
{
    int bufferArray[2] = {firstTwoDigits[0], firstTwoDigits[1]};
    if (digitsCount % 2 != 0)
    {
        firstTwoDigits[0] = bufferArray[1];
        firstTwoDigits[1] = bufferArray[0];
    }
}

void printResult(int digitsSum, int digitsCount, int firstTwoDigits[2])
{
    string result;
    if (digitsSum % 10 != 0)
    {
        result = "INVALID\n";
    }
    else if (firstTwoDigits[0] == 3 && firstTwoDigits[1] == 7 && digitsCount == 15)
    {
        result = "AMEX\n";
    }
    else if (firstTwoDigits[0] == 5 && (firstTwoDigits[1] == 1 || firstTwoDigits[1] == 5) && digitsCount == 16)
    {
        result = "MASTERCARD\n";
    }
    else if (firstTwoDigits[0] == 4 && (digitsCount == 13 || digitsCount == 16))
    {
        result = "VISA\n";
    }
    else
    {
        result = "INVALID\n";
    }
    printf("%s", result);
}