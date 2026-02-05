/*
 * GCD CALCULATOR - HEADER FILE
 *
 * Calculates the Greatest Common Divisor using Euclid's algorithm.
 *
 * OPTIMIZATION NOTES (2026):
 * - Added handling for negative numbers (GCD is always positive)
 * - Added handling for edge case where both inputs are 0 (undefined)
 * - See functions.c for detailed algorithm explanation
 */

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

/*
 * euclid - Calculate GCD(m, n) using Euclidean algorithm
 *
 * Parameters:
 *   m, n - Two integers (can be negative, will use absolute values)
 *
 * Returns:
 *   The greatest common divisor (always positive)
 *   Returns 0 if both m and n are 0 (GCD undefined in this case)
 *
 * Examples:
 *   euclid(48, 18) = 6
 *   euclid(-12, 8) = 4
 *   euclid(0, 5) = 5
 *   euclid(0, 0) = 0 (undefined, returns 0 as error indicator)
 */
int euclid(int m, int n);

#endif // FUNCTIONS_H_INCLUDED
