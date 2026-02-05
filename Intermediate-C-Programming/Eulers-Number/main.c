/*
 * EULER'S NUMBER (e) CALCULATOR
 *
 * Calculates Euler's number (e ≈ 2.71828...) using the Taylor series expansion:
 *   e = 1 + 1/1! + 1/2! + 1/3! + 1/4! + ...
 *
 * User specifies:
 *   n       - Maximum number of terms to use
 *   epsilon - Convergence threshold (stop early if term < epsilon)
 *
 * OPTIMIZATION NOTES (2026):
 * - Fixed critical type mismatch bug in functions.c/h (epsilon was int)
 * - Fixed integer overflow in factorial calculation
 * - Added input validation
 * - Optimized factorial calculation to be incremental
 */

#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

int main()
{
    int n;
    float epsilon;

    printf("Enter a value for n (max terms): ");
    scanf("%d", &n);

    /*
     * Input validation: n should be non-negative.
     * Large n values work but may be slow and hit floating-point limits.
     */
    if (n < 0) {
        printf("Error: n must be non-negative\n");
        return 1;
    }

    printf("Enter a value for E (epsilon convergence threshold): ");
    scanf("%f", &epsilon);

    /*
     * Input validation: epsilon should be positive.
     * Common values: 0.0001, 0.00001, etc.
     * Smaller epsilon = more terms computed = more precision.
     */
    if (epsilon < 0) {
        printf("Error: epsilon must be non-negative\n");
        return 1;
    }

    printf("\n");

    /*
     * The mathematical constant e is approximately 2.718281828...
     * With n=20 and small epsilon, we get very close to this value.
     * Note: %f shows 6 decimal places by default, use %.10f for more precision.
     */
    printf("e = %.10f\n", computeE(n, epsilon));

    return 0;
}
