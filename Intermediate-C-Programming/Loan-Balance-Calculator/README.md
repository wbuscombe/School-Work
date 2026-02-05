# Loan Balance Calculator

Calculates the remaining balance on a loan after monthly payments, accounting for interest.

## Description

Write a program that calculates the remaining balance on a loan after the first, second and third monthly payments.

Display each balance with two digits after the decimal point. *Hint:* Each month, the balance is decreased by the amount of the payment, but increased by the balance times the monthly interest rate. To find the monthly interest rate, convert the interest rate entered by the user to a percentage and divide it by 12.

## Financial Concepts

### Amortization
When you make loan payments, part goes to interest (the cost of borrowing) and part goes to principal (the actual debt). Early payments are mostly interest; later payments are mostly principal.

### Formula
```
new_balance = old_balance - payment + (old_balance × monthly_rate)
monthly_rate = (annual_rate / 100) / 12
```

### Example
$10,000 loan at 6% APR, $500/month payment:
- Monthly rate: 6/100/12 = 0.005 (0.5%)
- Month 1: $10,000 - $500 + ($10,000 × 0.005) = $9,550
- Month 2: $9,550 - $500 + ($9,550 × 0.005) = $9,097.75

## Optimizations (2026)

The original implementation was clean and mathematically correct. The following enhancements were added for educational value:

### Documentation
**Files:** `functions.c`, `functions.h`

Added detailed comments explaining:
- The financial concepts (principal, interest, amortization)
- How the formula works step by step
- Why interest is calculated on the old balance (simple interest model)

### Input Validation
**File:** `main.c`

Added validation for:
- scanf() return values (catch invalid input)
- Negative values (loan, rate, payment should be positive)

### Enhanced Output
**File:** `main.c`

- Added loan summary display before calculations
- Shows monthly interest rate in addition to annual
- Added breakdown of next payment (interest vs. principal portions)

### Code Clarity
**File:** `functions.c`

Refactored calculation to use named intermediate variables for readability:
```c
float monthlyRate = (rate / 100.0f) / 12.0f;
float interestThisMonth = *balance * monthlyRate;
*balance = *balance - payment + interestThisMonth;
```

************************************************************************************************************************************************************
Sample run:
************************************************************************************************************************************************************

    $ make

        gcc main.c functions.c -Wall -std=c99 -lm -o loan-balance

    $ ./loan-balance 
    
        Enter amount of loan: 20000.00
        Enter interest rate: 6.0
        Enter monthly payment: 386.66

        Balance remaining after first payment: $19713.34
        Balance remaining after second payment: $19425.25
        Balance remaining after third payment: $19135.71
