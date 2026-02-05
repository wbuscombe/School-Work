# Magic Square Checker

Verifies if a set of numbers forms a magic square by calculating row, column, and diagonal sums.

## Description

Write a program that asks the user to enter the numbers from 1 to 16 (in any order) and then displays the numbers in a 4 by 4 arrangement, followed by the sums of the rows, columns, and diagonals.

## About Magic Squares

A magic square is an n×n grid filled with distinct positive integers where the sum of each row, column, and both main diagonals are all equal. This sum is called the "magic constant."

For a magic square using numbers 1 to n², the magic constant is:
`M = n(n² + 1) / 2`

Examples:
- 3×3: M = 3(9+1)/2 = 15
- 4×4: M = 4(16+1)/2 = 34

Classic 3×3 magic square:
```
  2   7   6
  9   5   1
  4   3   8
```
All rows, columns, and diagonals sum to 15.

## Optimizations (2026)

The following improvements were made for robustness and usability:

### Security: Buffer Overflow Protection
**File:** `functions.c` - `readInput()`

Added bounds checking when reading input to prevent writing past array boundaries. Also added a maximum elements constant (`MAX_ELEMENTS = 100`).

### Input Validation: Perfect Square Check
**File:** `main.c`

**The Problem:**
Original floating-point comparison was unreliable:
```c
if (fmod(sqrt(count), 1) != 0.0)  // Can fail due to precision!
```

`sqrt(9)` might return `2.9999999999` instead of exactly `3.0`.

**The Fix:**
Use integer comparison after rounding:
```c
int size = (int)(sqrtCount + 0.5);  // Round to nearest
if (size * size != count)  // Integer comparison is exact
```

### New Feature: Magic Square Verification
**File:** `main.c`

Added automatic verification that checks if all sums are equal and displays a clear result message.

### Code Clarity: Documentation
**File:** `functions.c`

Added extensive comments explaining:
- Row-major array indexing (how 2D grids map to 1D arrays)
- The formulas for calculating each sum type
- Why the original code worked correctly despite confusing parameter naming

************************************************************************************************************************************************************
Sample run:
************************************************************************************************************************************************************

    $ make
        
        gcc main.c functions.c -Wall -std=c99 -lm -o magic-square

    $ ./magic-square 

        16 3 2 13 5 10 11 8 9 6 7 12 4 15 14 1

        16   3   2  13 
        5  10  11   8 
        9   6   7  12 
        4  15  14   1 

        Row sums: 34 34 34 34 
        Column sums: 34 34 34 34 
        Diagonal sums: 34 34
