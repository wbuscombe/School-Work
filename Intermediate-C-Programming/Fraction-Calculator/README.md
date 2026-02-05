# Fraction Calculator

A four-function calculator that performs arithmetic on fractions and displays results in fractional form.

## Description

Create a four function calculator that prints results in fractional form and accepts fractions as terms. Focus on creating a user friendly command line calculator.

## Features

- Supports addition (+), subtraction (-), multiplication (*), and division (/)
- Accepts both whole numbers and fractions as operands
- Automatically reduces results to lowest terms
- Handles negative numbers and edge cases

## Optimizations (2026)

The following optimizations and bug fixes were made for educational purposes:

### Critical Bug Fix: Division Operation
**File:** `functions.c` - `fractionDivision()`

The original division implementation was conceptually flawed. It tried to equalize denominators (the algorithm for addition/subtraction) rather than multiplying by the reciprocal.

**Correct algorithm:** `(a/b) ÷ (c/d) = (a/b) × (d/c) = (a×d)/(b×c)`

Original code happened to work in some cases by mathematical coincidence, but was confusing and didn't follow the standard fraction division algorithm.

### Security: Buffer Overflow Protection
**File:** `functions.c` - `readInput()`

Added bounds checking when reading input to prevent writing past the buffer boundary. Original code had no limit check on the input index.

### Robustness: Input Validation
- Added NULL checks for strtok() results
- Added division by zero protection
- Added handling for invalid operators
- Fixed operator parsing (was copying string to char, now correctly extracts single character)

### Improvement: GCD for Negative Numbers
**File:** `functions.c` - `GCD()` and `reduceFraction()`

The GCD function now uses absolute values to ensure proper handling of negative fractions. Added edge case handling for zero numerators and denominators.

## Building

************************************************************************************************************************************************************
Sample run:
************************************************************************************************************************************************************

    $ make
    
        gcc main.c functions.c -Wall -std=c99 -lm -o fraction-calc
        
    $ ./fraction-calc
    
        ******************************    Calculator    ******************************
        *                                                                            *
        *  Please enter an equation in the form: (operand) (operator) (operand)      *
        *                                                                            *
        *  Fractions must include a / with no additional whitespace                  *
        *                                                                            *
        *  Featured operations: + , - , * , /                                        *
        ******************************************************************************
        : 5/8 + 7/11

        5/8 + 7/11 = 111/88
        
    $ ./fraction-calc 
        
        ******************************    Calculator    ******************************
        *                                                                            *
        *  Please enter an equation in the form: (operand) (operator) (operand)      *
        *                                                                            *
        *  Fractions must include a / with no additional whitespace                  *
        *                                                                            *
        *  Featured operations: + , - , * , /                                        *
        ******************************************************************************
        : 7/10 - 1/2

        7/10 - 1/2 = 1/5
        
    $ ./fraction-calc 
    
        ******************************    Calculator    ******************************
        *                                                                            *
        *  Please enter an equation in the form: (operand) (operator) (operand)      *
        *                                                                            *
        *  Fractions must include a / with no additional whitespace                  *
        *                                                                            *
        *  Featured operations: + , - , * , /                                        *
        ******************************************************************************
        : 1/2 * 5/6

        1/2 * 5/6 = 5/12
        
    $ ./fraction-calc 
    
        ******************************    Calculator    ******************************
        *                                                                            *
        *  Please enter an equation in the form: (operand) (operator) (operand)      *
        *                                                                            *
        *  Fractions must include a / with no additional whitespace                  *
        *                                                                            *
        *  Featured operations: + , - , * , /                                        *
        ******************************************************************************
        : 5/8 / 1/4

        5/8 / 1/4 = 5/2
