#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

int main()
{
    float loan, rate, monthlyPayment;
    float balance;

    printf("Enter amount of loan: ");
    scanf("%f", &loan);

    printf("Enter interest rate: ");
    scanf("%f", &rate);

    printf("Enter monthly payment: ");
    scanf("%f", &monthlyPayment);

    printf("\n");

    balance = loan; // Initialize the balance to the value of the loan

    calculateBalance(&balance, rate, monthlyPayment);
    printf("Balance remaining after first payment: $%.2f\n", balance);

    calculateBalance(&balance, rate, monthlyPayment);
    printf("Balance remaining after second payment: $%.2f\n", balance);

    calculateBalance(&balance, rate, monthlyPayment);
    printf("Balance remaining after third payment: $%.2f\n", balance);

    return 0;
}
