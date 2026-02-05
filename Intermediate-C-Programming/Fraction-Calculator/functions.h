#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

void readInput(int *numer1, int *denom1, char *op, int *numer2, int *denom2);

void parseFraction(char frac[], int *numer, int *denom);

void fractionAddition(int numer1, int denom1, int numer2, int denom2, int *resNumer, int *resDenom);

void fractionSubtraction(int numer1, int denom1, int numer2, int denom2, int *resNumer, int *resDenom);

void fractionMultiplication(int numer1, int denom1, int numer2, int denom2, int *resNumer, int *resDenom);

void fractionDivision(int numer1, int denom1, int numer2, int denom2, int *resNumer, int *resDenom);

void reduceFraction(int *numer, int *denom);

int GCD(int m, int n);

#endif // FUNCTIONS_H_INCLUDED
