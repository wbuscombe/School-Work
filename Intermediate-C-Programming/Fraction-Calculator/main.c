/*
 * FRACTION CALCULATOR
 *
 * A simple calculator that performs arithmetic operations on fractions.
 * Supports: addition (+), subtraction (-), multiplication (*), division (/)
 *
 * Input format: operand operator operand
 * Examples: 1/2 + 1/4
 *           3 * 2/5
 *           7/8 / 1/2
 *
 * OPTIMIZATION NOTES (2026):
 * - Fixed critical division bug in functions.c
 * - Added input validation and error handling
 * - Improved fraction reduction to handle edge cases
 * - Added division by zero protection
 */

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

    /*
     * OPTIMIZATION: Added validation for zero denominators.
     * Original code would proceed with invalid fractions, causing
     * undefined behavior in calculations.
     */
    if (denom1 == 0 || denom2 == 0) {
        printf("Error: Denominator cannot be zero!\n");
        return 1;
    }

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
        /*
         * OPTIMIZATION: Added error message for invalid operator.
         * Original code silently did nothing, leaving result uninitialized.
         */
        printf("Error: Invalid operator '%c'. Use +, -, *, or /\n", op);
        return 1;
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
