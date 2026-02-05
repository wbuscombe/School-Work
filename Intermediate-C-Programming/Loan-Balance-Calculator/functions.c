#include "functions.h"

void calculateBalance(float *balance, float rate, float payment)
{
    *balance = *balance - payment + (*balance * ((rate / 100.0f) / 12.0f)); // balance = balance - monthly payment + (balance * monthly interest rate)
}
