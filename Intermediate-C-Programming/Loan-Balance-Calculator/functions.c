/*
 * LOAN BALANCE CALCULATOR - IMPLEMENTATION
 *
 * Calculates loan balance after each monthly payment, accounting for
 * interest that accrues on the remaining balance.
 *
 * FINANCIAL CONCEPTS:
 * - Principal: The original loan amount
 * - Interest Rate: Annual percentage rate (APR) charged on the balance
 * - Monthly Payment: Fixed amount paid each month
 * - Amortization: Process of paying off debt over time with regular payments
 *
 * The formula used here is simplified - real loan calculations may also
 * consider: compound interest timing, escrow, fees, etc.
 */

#include "functions.h"

/*
 * calculateBalance - Update loan balance after a monthly payment
 *
 * Parameters:
 *   balance - Pointer to current balance (modified in place)
 *   rate    - Annual interest rate as percentage (e.g., 5.0 for 5%)
 *   payment - Monthly payment amount
 *
 * Formula:
 *   new_balance = old_balance - payment + (old_balance × monthly_rate)
 *
 * Where monthly_rate = (annual_rate / 100) / 12
 *
 * Example with $10,000 loan at 6% APR, $500/month payment:
 *   Monthly rate = 6/100/12 = 0.005 (0.5%)
 *   Month 1: $10,000 - $500 + ($10,000 × 0.005) = $9,550
 *   Month 2: $9,550 - $500 + ($9,550 × 0.005) = $9,097.75
 *   ... and so on until paid off
 *
 * NOTE: This function applies interest to the balance BEFORE subtracting
 * the payment, which is one common method. Another approach applies
 * interest to the balance AFTER subtracting the payment. The order
 * affects the total interest paid over the life of the loan.
 *
 * IMPLEMENTATION NOTE:
 * The calculation order here is: balance - payment + interest
 * This means interest is calculated on the OLD balance, not the
 * balance after payment. This is typical for simple interest loans.
 */
void calculateBalance(float *balance, float rate, float payment)
{
    /*
     * Breaking down the formula:
     *
     * rate / 100.0f     -> Convert percentage to decimal (6% -> 0.06)
     * / 12.0f           -> Convert annual to monthly rate (0.06 -> 0.005)
     * *balance * ...    -> Calculate interest for this month
     * *balance - payment -> Subtract the payment
     * + interest        -> Add the accrued interest
     */
    float monthlyRate = (rate / 100.0f) / 12.0f;
    float interestThisMonth = *balance * monthlyRate;

    *balance = *balance - payment + interestThisMonth;

    /*
     * Note: Balance can go negative if payment exceeds balance + interest.
     * In real applications, you'd cap the final payment at the remaining
     * balance plus final interest to avoid overpayment.
     */
}
