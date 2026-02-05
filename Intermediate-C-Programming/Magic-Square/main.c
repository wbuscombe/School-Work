/*
 * MAGIC SQUARE CHECKER
 *
 * A magic square is an n×n grid of distinct positive integers where
 * the sum of each row, column, and both main diagonals are all equal.
 * This sum is called the "magic constant."
 *
 * For example, the classic 3×3 magic square:
 *
 *     2   7   6      Row sums:      15, 15, 15
 *     9   5   1      Column sums:   15, 15, 15
 *     4   3   8      Diagonal sums: 15, 15
 *
 * The magic constant for an n×n grid using numbers 1 to n² is:
 *     M = n(n² + 1) / 2
 *
 * For n=3: M = 3(9+1)/2 = 15
 * For n=4: M = 4(16+1)/2 = 34
 *
 * USAGE:
 * Enter space-separated integers. The count must be a perfect square.
 * Example: 2 7 6 9 5 1 4 3 8
 *
 * OPTIMIZATION NOTES (2026):
 * - Added bounds checking for input array
 * - Added magic square verification (checks if all sums are equal)
 * - Fixed grid printing index calculation for clarity
 * - Added input validation and helpful error messages
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "functions.h"

int main()
{
    int nums[MAX_ELEMENTS];  /* Use constant from header instead of magic 100 */
    int count = 0;
    int size;  /* Grid dimension (rows = cols for square) */

    printf("Enter space-separated integers (count must be a perfect square): ");
    readInput(nums, &count);

    printf("\n");

    /*
     * Validate that we have a perfect square count.
     * A magic square must be n×n, so we need 1, 4, 9, 16, 25... numbers.
     */
    if (count == 0) {
        printf("Error: No numbers entered.\n");
        return 1;
    }

    double sqrtCount = sqrt((double)count);
    /*
     * ORIGINAL CODE:
     *     if (fmod(sqrt(count), 1) != 0.0)
     *
     * ISSUE: Floating-point comparison is unreliable due to precision.
     * sqrt(9) might return 2.9999999999 instead of exactly 3.0.
     *
     * FIX: Check if sqrt rounds to an integer that squares back to count.
     */
    size = (int)(sqrtCount + 0.5);  /* Round to nearest integer */
    if (size * size != count)
    {
        printf("Invalid input: %d numbers entered, but count must be a perfect square.\n", count);
        printf("Valid counts: 1, 4, 9, 16, 25, 36, 49, 64, 81, 100\n");
        return 1;
    }

    /* Print the grid */
    printf("Grid (%d x %d):\n", size, size);
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
            /*
             * ORIGINAL: nums[(i * rows) + j]
             * Note: Used 'rows' but should be 'cols' for clarity.
             * Since rows == cols (square), result is same. Using 'size' now.
             */
            printf("%4d ", nums[(i * size) + j]);
        printf("\n");
    }

    printf("\n");

    /* Calculate and display row sums */
    printf("Row sums:      ");
    int firstRowSum = rowSum(nums, size, 0);
    int isMagic = 1;  /* Assume it's magic until proven otherwise */

    for (int i = 0; i < size; ++i) {
        int sum = rowSum(nums, size, i);
        printf("%d ", sum);
        if (sum != firstRowSum) isMagic = 0;
    }
    printf("\n");

    /* Calculate and display column sums */
    printf("Column sums:   ");
    for (int i = 0; i < size; ++i) {
        int sum = colSum(nums, size, i);
        printf("%d ", sum);
        if (sum != firstRowSum) isMagic = 0;
    }
    printf("\n");

    /* Calculate and display diagonal sums */
    int diagSum1 = topLeftDiagSum(nums, size);
    int diagSum2 = topRightDiagSum(nums, size);
    printf("Diagonal sums: %d %d\n", diagSum1, diagSum2);
    if (diagSum1 != firstRowSum || diagSum2 != firstRowSum) isMagic = 0;

    printf("\n");

    /*
     * NEW FEATURE: Magic square verification
     * A valid magic square has all rows, columns, and diagonals
     * summing to the same value (the magic constant).
     */
    if (isMagic) {
        printf("✓ This IS a magic square! Magic constant = %d\n", firstRowSum);
    } else {
        printf("✗ This is NOT a magic square (sums are not all equal).\n");
    }

    return 0;
}
