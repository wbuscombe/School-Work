/*
 * MAGIC SQUARE CHECKER - HEADER FILE
 *
 * Functions for reading a square grid of numbers and calculating
 * row, column, and diagonal sums to verify if it's a magic square.
 *
 * A magic square is a grid where all rows, columns, and diagonals
 * sum to the same value (the "magic constant").
 *
 * OPTIMIZATION NOTES (2026):
 * - Added bounds checking for input array
 * - Clarified parameter naming (size = grid dimension for square matrix)
 * - Added detailed comments explaining array indexing
 * - See functions.c for implementation details
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

/* Maximum number of elements allowed in the grid */
#define MAX_ELEMENTS 100

/*
 * readInput - Read space-separated integers from stdin into array
 *
 * Parameters:
 *   nums  - Array to store the numbers (must have room for MAX_ELEMENTS)
 *   count - Output: number of integers read
 *
 * Note: Input count must be a perfect square (4, 9, 16, 25, etc.)
 *       for subsequent magic square calculations to work.
 */
void readInput(int nums[], int *count);

/*
 * rowSum - Calculate the sum of a specific row in the grid
 *
 * Parameters:
 *   nums - 1D array representing the 2D grid (row-major order)
 *   cols - Number of columns (= grid size for square matrix)
 *   row  - Row index (0-based)
 *
 * Returns: Sum of all elements in the specified row
 *
 * Array indexing for row-major storage:
 *   element[row][col] = nums[(row * cols) + col]
 */
int rowSum(int nums[], int cols, int row);

/*
 * colSum - Calculate the sum of a specific column in the grid
 *
 * Parameters:
 *   nums - 1D array representing the 2D grid (row-major order)
 *   size - Grid dimension (rows = cols for square matrix)
 *   col  - Column index (0-based)
 *
 * Returns: Sum of all elements in the specified column
 *
 * NOTE ON PARAMETER NAME:
 * Original parameter was named "rows" but used as stride (cols).
 * Renamed to "size" since for a square matrix, rows == cols.
 */
int colSum(int nums[], int size, int col);

/*
 * topLeftDiagSum - Sum of main diagonal (top-left to bottom-right)
 *
 * For a 3x3 grid:    [0] [ ] [ ]
 *                    [ ] [4] [ ]
 *                    [ ] [ ] [8]
 *
 * Diagonal elements: indices 0, 4, 8 = (0,0), (1,1), (2,2)
 * Formula: nums[(i * size) + i] for i = 0 to size-1
 */
int topLeftDiagSum(int nums[], int size);

/*
 * topRightDiagSum - Sum of anti-diagonal (top-right to bottom-left)
 *
 * For a 3x3 grid:    [ ] [ ] [2]
 *                    [ ] [4] [ ]
 *                    [6] [ ] [ ]
 *
 * Diagonal elements: indices 2, 4, 6 = (0,2), (1,1), (2,0)
 * Formula: nums[(i * size) + (size - 1 - i)] for i = 0 to size-1
 */
int topRightDiagSum(int nums[], int size);

#endif // FUNCTIONS_H_INCLUDED
