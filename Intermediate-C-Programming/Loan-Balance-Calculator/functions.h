/*
 * LOAN BALANCE CALCULATOR - HEADER FILE
 *
 * Provides functions for calculating loan amortization.
 *
 * FINANCIAL BACKGROUND:
 * When you take out a loan, you agree to repay the principal (borrowed amount)
 * plus interest over time. Each month, interest accrues on your remaining
 * balance, so early payments mostly go to interest while later payments
 * mostly go to principal.
 *
 * OPTIMIZATION NOTES (2026):
 * - Original implementation was clean and mathematically correct
 * - Added documentation explaining the financial concepts
 * - See functions.c for detailed formula breakdown
 */

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

/*
 * calculateBalance - Calculate new loan balance after a monthly payment
 *
 * Parameters:
 *   balance - Pointer to current loan balance (will be modified)
 *   rate    - Annual interest rate as percentage (e.g., 6.0 for 6%)
 *   payment - Monthly payment amount in dollars
 *
 * The function updates *balance in place using the formula:
 *   new_balance = old_balance - payment + (old_balance × monthly_rate)
 *
 * Where monthly_rate = rate / 100 / 12
 *
 * Note: If payment exceeds balance + interest, balance will go negative.
 *       In production code, you'd want to handle this case.
 */
void calculateBalance(float *balance, float rate, float payment);

#endif // FUNCTIONS_H_INCLUDED
