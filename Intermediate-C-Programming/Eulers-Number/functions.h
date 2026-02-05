/*
 * EULER'S NUMBER (e) CALCULATOR - HEADER FILE
 *
 * OPTIMIZATION NOTES (2026):
 * - Fixed type mismatch: epsilon parameter changed from int to float
 * - Changed factorial return type to long long to prevent overflow
 * - See functions.c for detailed explanations of fixes
 */

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

/*
 * computeE - Calculate Euler's number using the series: e = sum(1/n!)
 *
 * Parameters:
 *   n       - Maximum number of terms to compute
 *   epsilon - Convergence threshold (stop when term < epsilon)
 *
 * ORIGINAL SIGNATURE (BUGGY):
 *     float computeE(int n, int epsilon);
 *
 * BUG: epsilon was declared as int, but:
 *   1. main.c reads it as float with scanf("%f", &epsilon)
 *   2. It's compared against float term values (1/n!)
 *   3. Typical epsilon values are small decimals like 0.0001
 *
 * With int epsilon, any value < 1 becomes 0, making the convergence
 * check (term < epsilon) always false for valid terms!
 */
float computeE(int n, float epsilon);  /* FIXED: epsilon is now float */

/*
 * factorial - Calculate n! (n factorial)
 *
 * ORIGINAL SIGNATURE:
 *     int factorial(int n);
 *
 * ISSUE: int can only hold values up to ~2.1 billion (2^31 - 1)
 *   - 12! = 479,001,600 (fits in int)
 *   - 13! = 6,227,020,800 (OVERFLOW! wraps to negative)
 *   - 20! = 2,432,902,008,176,640,000 (needs 64-bit)
 *
 * FIX: Use long long which holds up to ~9.2 quintillion
 * This allows factorial up to 20! before overflow.
 */
long long factorial(int n);  /* FIXED: return type is now long long */

#endif // FUNCTIONS_H_INCLUDED
