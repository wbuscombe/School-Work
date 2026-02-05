/*
 * EULER'S NUMBER (e) CALCULATOR - IMPLEMENTATION
 *
 * Calculates e using the Taylor series: e = 1 + 1/1! + 1/2! + 1/3! + ...
 *
 * OPTIMIZATION NOTES (2026):
 * - Fixed epsilon type mismatch (int -> float)
 * - Fixed factorial overflow (int -> long long)
 * - Optimized computeE to use incremental factorial calculation
 * - Original inefficient code preserved in comments
 */

#include "functions.h"

/*
 * computeE - Calculate Euler's number e ≈ 2.71828...
 *
 * Uses the infinite series: e = Σ(1/n!) for n = 0 to infinity
 * Stops when either:
 *   1. We've computed n terms, OR
 *   2. The current term is smaller than epsilon (convergence)
 */
float computeE(int n, float epsilon)  /* FIXED: epsilon is now float */
{
    float val = 0.0f;

    /*
     * ORIGINAL INEFFICIENT CODE:
     * --------------------------
     *     for (int i = 0; i <= n; ++i){
     *         float term = (1.0f / factorial(i));
     *         if (term < epsilon)
     *             break;
     *         val += term;
     *     }
     *
     * INEFFICIENCY:
     * factorial(i) recalculates the entire factorial from 1 each iteration.
     * For n=10: factorial is called 11 times, computing:
     *   1, 1, 1*2, 1*2*3, 1*2*3*4, ... (55 multiplications total!)
     *
     * OPTIMIZATION: Use incremental calculation.
     * Since n! = (n-1)! * n, we can keep a running factorial value
     * and just multiply by i each iteration. This reduces to just
     * n multiplications total.
     */

    long long fact = 1;  /* Running factorial value, starts at 0! = 1 */

    for (int i = 0; i <= n; ++i)
    {
        /* Calculate factorial incrementally: fact = i! */
        if (i > 0) {
            fact *= i;  /* i! = (i-1)! * i */
        }

        float term = (1.0f / (float)fact);

        /*
         * Convergence check: stop if term is negligibly small.
         * This epsilon comparison only works correctly now that
         * epsilon is a float (was int before, causing the bug).
         */
        if (term < epsilon)
            break;

        val += term;
    }

    return val;
}

/*
 * factorial - Calculate n! (n factorial)
 *
 * Definition: n! = 1 × 2 × 3 × ... × n, with 0! = 1
 *
 * Note: This standalone function is kept for educational purposes
 * and potential external use. The optimized computeE() above uses
 * incremental calculation instead.
 */
long long factorial(int n)  /* FIXED: return type is now long long */
{
    /*
     * ORIGINAL CODE:
     * --------------
     *     int val = 1;
     *     for (int i = 1; i <= n; ++i)
     *         val *= i;
     *     return val;
     *
     * OVERFLOW ISSUE:
     * int val can only hold values up to 2,147,483,647 (2^31 - 1).
     *
     * Factorial growth:
     *   10! =       3,628,800     ✓ fits in int
     *   12! =     479,001,600     ✓ fits in int
     *   13! =   6,227,020,800     ✗ OVERFLOW! (int wraps to negative)
     *   20! = 2.4 × 10^18        ✗ needs long long
     *
     * FIX: Use long long (64-bit) which can hold up to 9.2 × 10^18.
     * This allows computing factorial up to 20! before overflow.
     * For larger values, consider using arbitrary precision libraries.
     */

    long long val = 1;  /* FIXED: Use 64-bit integer */

    /* Validate input - factorial undefined for negative numbers */
    if (n < 0) {
        return 0;  /* Or could return -1 to indicate error */
    }

    for (int i = 1; i <= n; ++i)
        val *= i;

    return val;
}
