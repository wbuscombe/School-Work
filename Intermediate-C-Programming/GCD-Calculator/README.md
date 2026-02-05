# GCD Calculator

Calculates the Greatest Common Divisor of two integers using Euclid's algorithm.

## Description

Write a program that asks the user to enter two integers, then calculates and displays their greatest common divisor (**GCD**).

## About Euclid's Algorithm

Euclid's algorithm is one of the oldest algorithms still in common use (dating back to ~300 BCE). It efficiently finds the GCD using the principle that:

`GCD(m, n) = GCD(n, m mod n)`

This is applied recursively until n becomes 0, at which point m is the GCD.

**Example:** GCD(48, 18)
```
GCD(48, 18) → GCD(18, 48%18) = GCD(18, 12)
GCD(18, 12) → GCD(12, 18%12) = GCD(12, 6)
GCD(12, 6)  → GCD(6, 12%6)   = GCD(6, 0)
GCD(6, 0)   → return 6
```

## Optimizations (2026)

The following improvements were made for robustness:

### Edge Case: Both Inputs Zero
**Files:** `main.c`, `functions.c`

**The Problem:**
GCD(0, 0) is mathematically undefined - every integer divides 0, so there's no "greatest" divisor. The original code would return 0 without explanation.

**The Fix:**
Added explicit handling in both main.c (user-friendly error message) and functions.c (returns 0 as error indicator).

### Edge Case: Negative Numbers
**File:** `functions.c` - `euclid()`

**The Problem:**
GCD is always positive by definition, but the modulo operation with negative numbers can produce negative results:
- `GCD(-12, 8)` should be `4`, not `-4`

**The Fix:**
Convert to absolute values at the start of the function:
```c
if (m < 0) m = -m;
if (n < 0) n = -n;
```

### Input Validation
**File:** `main.c`

Added validation for scanf() return value to catch non-integer input.

************************************************************************************************************************************************************
Sample run:
************************************************************************************************************************************************************

    $ make

        gcc main.c functions.c -Wall -std=c99 -lm -o gcd

    $ ./gcd 
    
        Enter two integers: 12 28
        Greatest common divisor: 4
