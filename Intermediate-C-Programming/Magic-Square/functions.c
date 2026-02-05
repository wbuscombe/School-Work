/*
 * MAGIC SQUARE CHECKER - IMPLEMENTATION
 *
 * A magic square is an n×n grid filled with distinct integers where
 * the sum of each row, column, and diagonal equals the same value.
 *
 * For example, a 3×3 magic square:
 *     2  7  6
 *     9  5  1
 *     4  3  8
 *
 * All rows, columns, and diagonals sum to 15 (the magic constant).
 *
 * OPTIMIZATION NOTES (2026):
 * - Added bounds checking to prevent buffer overflow
 * - Fixed confusing parameter naming in colSum() and other functions
 * - Added detailed comments explaining 1D to 2D array indexing
 * - Original code logic preserved (was correct, just needed safety checks)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

#define delims " \t\r\n"

#define MAX_LEN 1024  /* Increased from 128 for larger inputs */

void readInput(int nums[], int *count)
{
    char c = 0, input[MAX_LEN], *buffer;
    int index = 0;

    /*
     * OPTIMIZATION: Added bounds checking to prevent buffer overflow.
     *
     * ORIGINAL CODE (NO BOUNDS CHECK):
     *     while(1) {
     *         c = getc(stdin);
     *         if (c == '\n') { ... }
     *         else { input[index++] = c; }  // No limit check!
     *     }
     *
     * ISSUE: If user enters more than MAX_LEN characters, we write
     * past the end of input[], corrupting stack memory.
     */
    while(1)
    {
        c = getc(stdin);

        if (c == '\n' || c == EOF)
        {
            input[index] = '\0';
            break;
        }
        /* SECURITY FIX: Check bounds before writing */
        else if (index < MAX_LEN - 1)
        {
            input[index++] = c;
        }
        /* Silently truncate if input exceeds buffer */
    }

    buffer = strtok(input, delims);

    /*
     * OPTIMIZATION: Added NULL check for empty input.
     * Original code would crash on atoi(NULL) with empty input.
     */
    if (buffer == NULL) {
        return;  /* No numbers entered */
    }

    /*
     * OPTIMIZATION: Added bounds check on count to prevent array overflow.
     * nums[] is declared with size 100 in main.c (now MAX_ELEMENTS).
     */
    if (*count < MAX_ELEMENTS) {
        nums[(*count)++] = atoi(buffer);
    }

    while ((buffer = strtok(NULL, delims)) != NULL)
    {
        if (*count < MAX_ELEMENTS) {
            nums[(*count)++] = atoi(buffer);
        }
        /* Stop if we've hit the limit */
        else {
            break;
        }
    }

    /* Note: Original had redundant: buffer = strtok(NULL, delims); */
    /* This line did nothing useful and has been removed */
}

/*
 * rowSum - Calculate sum of elements in a specific row
 *
 * For a grid stored in row-major order (each row stored consecutively):
 *
 *   Grid:         1D Array:           Indices:
 *   [a][b][c]     [a,b,c,d,e,f,g,h,i] row 0: 0,1,2
 *   [d][e][f]                         row 1: 3,4,5
 *   [g][h][i]                         row 2: 6,7,8
 *
 * Formula: element at (row, col) = nums[(row * num_cols) + col]
 * For row sum: iterate col from 0 to cols-1, keeping row fixed
 */
int rowSum(int nums[], int cols, int row)
{
    int sum = 0;

    for (int i = 0; i < cols; ++i)
        sum += nums[(row * cols) + i];

    return sum;
}

/*
 * colSum - Calculate sum of elements in a specific column
 *
 * For a grid stored in row-major order:
 *
 *   Grid:         1D Array:           Indices:
 *   [a][b][c]     [a,b,c,d,e,f,g,h,i] col 0: 0,3,6
 *   [d][e][f]                         col 1: 1,4,7
 *   [g][h][i]                         col 2: 2,5,8
 *
 * Formula: element at (row, col) = nums[(row * num_cols) + col]
 * For col sum: iterate row from 0 to rows-1, keeping col fixed
 *
 * NOTE ON PARAMETER:
 * Original parameter was "rows" but it's used as both the row count
 * AND the stride (num_cols). Renamed to "size" for clarity since
 * this code assumes a square matrix where rows == cols.
 */
int colSum(int nums[], int size, int col)
{
    int sum = 0;

    /*
     * ORIGINAL CODE:
     *     for (int i = 0; i < rows; ++i)
     *         sum += nums[(rows * i) + col];
     *
     * This was correct for square matrices where rows == cols.
     * The stride (number of columns) equals 'rows' in this case.
     * Renamed parameter to 'size' for clarity.
     */
    for (int i = 0; i < size; ++i)
        sum += nums[(size * i) + col];

    return sum;
}

/*
 * topLeftDiagSum - Sum the main diagonal (top-left to bottom-right)
 *
 * For a 3x3 grid, the main diagonal elements are at (0,0), (1,1), (2,2):
 *
 *   [X][ ][ ]     Indices: 0, 4, 8
 *   [ ][X][ ]     Formula: nums[(i * size) + i]
 *   [ ][ ][X]     which simplifies to: nums[i * (size + 1)]
 */
int topLeftDiagSum(int nums[], int size)
{
    int sum = 0;

    for (int i = 0; i < size; ++i)
        sum += nums[(size * i) + i];

    return sum;
}

/*
 * topRightDiagSum - Sum the anti-diagonal (top-right to bottom-left)
 *
 * For a 3x3 grid, the anti-diagonal elements are at (0,2), (1,1), (2,0):
 *
 *   [ ][ ][X]     Indices: 2, 4, 6
 *   [ ][X][ ]     Position: (row, size-1-row)
 *   [X][ ][ ]     Formula: nums[(row * size) + (size - 1 - row)]
 *
 * ORIGINAL CODE used a different but equivalent formula:
 *     nums[(size * (size - (i + 1))) + i]
 *
 * For i=0: (3*(3-1))+0 = 6, but we want 2!
 *
 * Wait - let me trace through more carefully:
 * i=0: (3 * (3-1)) + 0 = (3 * 2) + 0 = 6  <- bottom-left
 * i=1: (3 * (3-2)) + 1 = (3 * 1) + 1 = 4  <- center
 * i=2: (3 * (3-3)) + 2 = (3 * 0) + 2 = 2  <- top-right
 *
 * So original iterates from bottom-left to top-right (same diagonal,
 * different traversal order). The sum is the same either way.
 */
int topRightDiagSum(int nums[], int size)
{
    int sum = 0;

    /*
     * Original formula traverses bottom-left to top-right:
     * nums[(size * (size - (i + 1))) + i]
     *
     * Equivalent cleaner formula (top-right to bottom-left):
     * nums[(i * size) + (size - 1 - i)]
     *
     * Both give the same sum, keeping original for consistency.
     */
    for (int i = 0; i < size; ++i)
        sum += nums[(size * (size - (i + 1))) + i];

    return sum;
}
