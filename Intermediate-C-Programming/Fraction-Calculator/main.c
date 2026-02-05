#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

int main()
{
    int numer1, numer2, denom1, denom2;
    char op;
    int resNumer, resDenom;

    printf("******************************    Calculator    ******************************\n");
    printf("*                                                                            *\n");
    printf("*  Please enter an equation in the form: (operand) (operator) (operand)      *\n");
    printf("*                                                                            *\n");
    printf("*  Fractions must include a / with no additional whitespace                  *\n");
    printf("*                                                                            *\n");
    printf("*  Featured operations: + , - , * , /                                        *\n");
    printf("******************************************************************************\n");
    printf(": ");

    readInput(&numer1, &denom1, &op, &numer2, &denom2);

    printf("\n");

    switch(op)
    {
    case '+':
    {
        fractionAddition(numer1, denom1, numer2, denom2, &resNumer, &resDenom);
        break;
    }
    case '-':
    {
        fractionSubtraction(numer1, denom1, numer2, denom2, &resNumer, &resDenom);
        break;
    }
    case '*':
    {
        fractionMultiplication(numer1, denom1, numer2, denom2, &resNumer, &resDenom);
        break;
    }
    case '/':
    {
        fractionDivision(numer1, denom1, numer2, denom2, &resNumer, &resDenom);
        break;
    }
    default:
        break;
    }

    reduceFraction(&resNumer, &resDenom);

    if (denom1 == 1)
        printf("%d ", numer1);
    else
        printf("%d/%d ", numer1, denom1);

    printf("%c ", op);

    if (denom2 == 1)
        printf("%d ", numer2);
    else
        printf("%d/%d ", numer2, denom2);

    printf("= ");

    if (resDenom == 1)
        printf("%d\n", resNumer);
    else
        printf("%d/%d\n", resNumer, resDenom);

    return 0;
}
