#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

#define delims " \t\r\n"

#define MAX_LEN 128

void readInput(int *numer1, int *denom1, char *op, int *numer2, int *denom2)
{
    char c = 0, input[MAX_LEN], *buffer;
    int index = 0;

    char left[MAX_LEN], right[MAX_LEN];

    while(1)
    {
        c = getc(stdin);

        if (c == '\n')
        {
            input[index] = '\0';
            break;
        }
        else
        {
            input[index++] = c;
        }
    }

    buffer = strtok(input, delims);

    strcpy(left, buffer);

    buffer = strtok(NULL, delims);

    strcpy(op, buffer);

    buffer = strtok(NULL, delims);

    strcpy(right, buffer);

    buffer = strtok(NULL, delims);

    // Check left operand for fraction
    if (strstr(left, "/") != NULL)
    {
        parseFraction(left, numer1, denom1);
    }
    else
    {
        *numer1 = atoi(left);
        *denom1 = 1;
    }

    // Check right operand for fraction
    if (strstr(right, "/") != NULL)
    {
        parseFraction(right, numer2, denom2);
    }
    else
    {
        *numer2 = atoi(right);
        *denom2 = 1;
    }
}

void parseFraction(char frac[], int *numer, int *denom)
{
    char *buffer;

    buffer = strtok(frac, "/");

    *numer = atoi(buffer);

    buffer = strtok(NULL, " \0");

    *denom = atoi(buffer);

    buffer = strtok(NULL, " \0");
}

void fractionAddition(int numer1, int denom1, int numer2, int denom2, int *resNumer, int *resDenom)
{
    // Check for same denominators
    if (denom1 != denom2)
    {
        numer1 *= denom2;
        numer2 *= denom1;

        *resDenom = denom1 * denom2;
    }
    else
        *resDenom = denom1;

    *resNumer = numer1 + numer2;
}

void fractionSubtraction(int numer1, int denom1, int numer2, int denom2, int *resNumer, int *resDenom)
{
    // Check for same denominators
    if (denom1 != denom2)
    {
        numer1 *= denom2;
        numer2 *= denom1;

        *resDenom = denom1 * denom2;
    }
    else
        *resDenom = denom1;

    *resNumer = numer1 - numer2;
}

void fractionMultiplication(int numer1, int denom1, int numer2, int denom2, int *resNumer, int *resDenom)
{
    *resNumer = numer1 * numer2;
    *resDenom = denom1 * denom2;
}

void fractionDivision(int numer1, int denom1, int numer2, int denom2, int *resNumer, int *resDenom)
{
    // Check for same denominators
    if (denom1 != denom2)
    {
        numer1 *= denom2;
        numer2 *= denom1;
    }

    *resNumer = numer1;
    *resDenom = numer2;
}

void reduceFraction(int *numer, int *denom)
{
    int divisor = GCD(*numer, *denom);

    *numer /= divisor;
    *denom /= divisor;
}

int GCD(int m, int n)
{
    if (n == 0)
        return m;

    return GCD(n, m % n);
}
