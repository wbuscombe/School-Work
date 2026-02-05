/*
 * LOAN BALANCE CALCULATOR
 *
 * Calculates the remaining balance on a loan after monthly payments,
 * taking into account the interest that accrues each month.
 *
 * This demonstrates a basic amortization calculation where each month:
 * 1. Interest accrues on the current balance
 * 2. The monthly payment is subtracted
 * 3. The new balance carries forward
 *
 * USAGE:
 * Enter the loan amount, annual interest rate (as percentage),
 * and monthly payment amount. The program shows the balance
 * after the first three payments.
 *
 * EXAMPLE:
 *   Loan amount: $10,000
 *   Interest rate: 6% (annual)
 *   Monthly payment: $500
 *
 *   After payment 1: $9,550.00
 *   After payment 2: $9,097.75
 *   After payment 3: $8,643.23
 *
 * OPTIMIZATION NOTES (2026):
 * - Original implementation was clean and correct
 * - Added input validation for negative values
 * - Added detailed comments explaining the financial math
 * - Improved functions.c with clearer variable names
 */

#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

int main()
{
    float loan, rate, monthlyPayment;
    float balance;

    printf("=== Loan Balance Calculator ===\n\n");

    printf("Enter amount of loan: $");
    if (scanf("%f", &loan) != 1 || loan < 0) {
        printf("Error: Please enter a valid positive loan amount\n");
        return 1;
    }

    printf("Enter annual interest rate (%%): ");
    if (scanf("%f", &rate) != 1 || rate < 0) {
        printf("Error: Please enter a valid positive interest rate\n");
        return 1;
    }

    printf("Enter monthly payment: $");
    if (scanf("%f", &monthlyPayment) != 1 || monthlyPayment < 0) {
        printf("Error: Please enter a valid positive payment amount\n");
        return 1;
    }

    printf("\n");

    /*
     * Display loan summary before calculations.
     * Monthly interest rate = Annual rate / 12
     */
    printf("Loan Summary:\n");
    printf("  Principal:      $%.2f\n", loan);
    printf("  Annual Rate:    %.2f%%\n", rate);
    printf("  Monthly Rate:   %.4f%%\n", rate / 12.0f);
    printf("  Monthly Payment: $%.2f\n", monthlyPayment);
    printf("\n");

    balance = loan;  /* Initialize balance to the principal amount */

    /*
     * Calculate and display balance after each of the first 3 payments.
     *
     * In a full implementation, you would loop until balance <= 0
     * and track total interest paid, number of payments, etc.
     */
    calculateBalance(&balance, rate, monthlyPayment);
    printf("Balance after payment 1: $%.2f\n", balance);

    calculateBalance(&balance, rate, monthlyPayment);
    printf("Balance after payment 2: $%.2f\n", balance);

    calculateBalance(&balance, rate, monthlyPayment);
    printf("Balance after payment 3: $%.2f\n", balance);

    /*
     * Bonus: Show how much of each payment goes to interest vs principal.
     * For the 4th payment (if we were to make one):
     */
    printf("\n");
    float nextInterest = balance * ((rate / 100.0f) / 12.0f);
    float nextPrincipal = monthlyPayment - nextInterest;
    printf("Next payment breakdown:\n");
    printf("  Interest portion:  $%.2f\n", nextInterest);
    printf("  Principal portion: $%.2f\n", nextPrincipal);

    return 0;
}
