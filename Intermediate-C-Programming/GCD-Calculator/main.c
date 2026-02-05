/*
 * GCD CALCULATOR
 *
 * Calculates the Greatest Common Divisor (GCD) of two integers
 * using Euclid's algorithm, one of the oldest algorithms still in common use.
 *
 * The GCD of two numbers is the largest positive integer that divides
 * both numbers without leaving a remainder.
 *
 * Examples:
 *   GCD(48, 18) = 6   (both divisible by 6)
 *   GCD(100, 25) = 25
 *   GCD(17, 5) = 1    (coprime numbers)
 *
 * OPTIMIZATION NOTES (2026):
 * - Added input validation for edge cases
 * - Added handling for negative numbers
 * - Added explanation of undefined case (both inputs zero)
 */

#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

int main()
{
    int m, n;

    printf("Enter two integers: ");

    /*
     * Input validation: Check that scanf successfully read 2 integers.
     * scanf returns the number of successfully matched items.
     */
    if (scanf("%d %d", &m, &n) != 2) {
        printf("Error: Please enter two valid integers\n");
        return 1;
    }

    /*
     * Edge case: GCD(0, 0) is mathematically undefined.
     * Every integer divides 0, so there's no "greatest" divisor.
     * We handle this specially rather than returning a misleading result.
     */
    if (m == 0 && n == 0) {
        printf("Error: GCD(0, 0) is undefined\n");
        printf("The GCD of two numbers where both are zero has no mathematical meaning.\n");
        return 1;
    }

    int result = euclid(m, n);

    /*
     * Display result with explanation for negative inputs.
     * GCD is always positive, so we use absolute values internally.
     */
    if (m < 0 || n < 0) {
        printf("Note: GCD uses absolute values (GCD is always positive)\n");
    }

    printf("Greatest common divisor of %d and %d: %d\n", m, n, result);

    return 0;
}
