#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

void readInput(int nums[], int *count);

int rowSum(int nums[], int cols, int row);

int colSum(int nums[], int rows, int col);

int topLeftDiagSum(int nums[], int rows);

int topRightDiagSum(int nums[], int rows);

#endif // FUNCTIONS_H_INCLUDED
