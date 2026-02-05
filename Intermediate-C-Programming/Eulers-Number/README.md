# Euler's Number Calculator

Approximates the mathematical constant *e* (≈ 2.71828...) using the Taylor series expansion.

## Description

The value of the mathematical constant *e* can be expressed as an infinite series:

*e* = 1 + 1/1! + 1/2! + 1/3! + ...

Write a program that approximates *e* by computing the value of

1 + 1/1! + 1/2! + 1/3! + ... + 1/*n*!

where *n* is an integer entered by the user.

Continue adding terms until the current term becomes less than *E*, where *E* is a small (floating-point) number entered by the user.

## Mathematical Background

Euler's number *e* is one of the most important constants in mathematics:
- It's the base of natural logarithms
- It appears in compound interest, probability, and calculus
- Its value is approximately 2.718281828459045...

## Optimizations (2026)

The following critical bug fixes and improvements were made:

### Critical Bug Fix: Type Mismatch
**Files:** `functions.h`, `functions.c` - `computeE()`

**The Problem:**
The epsilon parameter was declared as `int` instead of `float`:
```c
// BUGGY - epsilon should be float!
float computeE(int n, int epsilon);
```

Since epsilon values are typically small decimals (0.0001, 0.00001), declaring it as `int` caused them to truncate to 0, making the convergence check `(term < epsilon)` always false for valid terms!

**The Fix:**
Changed parameter type to `float`:
```c
float computeE(int n, float epsilon);
```

### Bug Fix: Integer Overflow in Factorial
**Files:** `functions.h`, `functions.c` - `factorial()`

**The Problem:**
`int` can only hold values up to ~2.1 billion:
- 12! = 479,001,600 ✓ (fits in int)
- 13! = 6,227,020,800 ✗ (OVERFLOW!)

**The Fix:**
Changed return type to `long long` (64-bit), which can hold up to 20!

### Optimization: Incremental Factorial
**File:** `functions.c` - `computeE()`

Original code recalculated factorial from scratch each iteration:
```c
for (int i = 0; i <= n; ++i)
    term = 1.0f / factorial(i);  // Recalculates 1×2×3×...×i each time!
```

Optimized to calculate incrementally:
```c
long long fact = 1;
for (int i = 0; i <= n; ++i) {
    if (i > 0) fact *= i;  // Just one multiplication!
    term = 1.0f / fact;
}
```

This reduces from O(n²) multiplications to O(n).

************************************************************************************************************************************************************
Sample run:
************************************************************************************************************************************************************

    $ make

        gcc main.c functions.c -Wall -std=c99 -lm -o eulers-number

    $ ./eulers-number 

        Enter a value for n: 15
        Enter a value for E (epsilon): .1

        e = 2.718282
