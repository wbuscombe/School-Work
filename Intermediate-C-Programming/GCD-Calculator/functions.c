/*
 * GCD CALCULATOR - IMPLEMENTATION
 *
 * Implements the Euclidean algorithm for finding the Greatest Common Divisor.
 *
 * The algorithm is based on the principle that:
 *   GCD(m, n) = GCD(n, m mod n)
 *
 * This works because any common divisor of m and n must also divide (m mod n),
 * and vice versa. We recursively apply this until n becomes 0.
 *
 * OPTIMIZATION NOTES (2026):
 * - Added handling for negative numbers
 * - Added handling for both-zero edge case
 * - Original code preserved in comments
 */

#include "functions.h"

int euclid(int m, int n)
{
    /*
     * ORIGINAL CODE:
     * --------------
     *     if (n == 0)
     *         return m;
     *     return euclid(n, m % n);
     *
     * ISSUES:
     * 1. Negative numbers: GCD(-12, 8) should be 4, not -4
     *    The modulo operation with negatives can produce negative results,
     *    and returning a negative m when n=0 gives wrong sign.
     *
     * 2. Both zero: GCD(0, 0) is mathematically undefined because every
     *    integer divides 0, so there's no "greatest" divisor.
     *    Original code returns 0, which is misleading (0 isn't the GCD).
     *
     * FIXES:
     * 1. Use absolute values - GCD is always positive by definition
     * 2. Handle GCD(0,0) explicitly and return 0 as error indicator
     */

    /* Handle negative numbers - GCD is always positive */
    if (m < 0) m = -m;
    if (n < 0) n = -n;

    /*
     * Special case: GCD(0, 0) is undefined.
     * Every integer divides 0, so there's no "greatest" common divisor.
     * We return 0 as an error indicator.
     */
    if (m == 0 && n == 0) {
        return 0;  /* Undefined - caller should check for this */
    }

    /*
     * Base case: GCD(m, 0) = m
     * Any number's GCD with 0 is itself (since n divides 0 for all n).
     */
    if (n == 0)
        return m;

    /*
     * Recursive case: Euclidean algorithm
     * GCD(m, n) = GCD(n, m mod n)
     *
     * Example trace for GCD(48, 18):
     *   GCD(48, 18) -> GCD(18, 48%18) = GCD(18, 12)
     *   GCD(18, 12) -> GCD(12, 18%12) = GCD(12, 6)
     *   GCD(12, 6)  -> GCD(6, 12%6)   = GCD(6, 0)
     *   GCD(6, 0)   -> return 6
     */
    return euclid(n, m % n);
}
