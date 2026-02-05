#include "functions.h"

float computeE(int n, int epsilon)
{
    float val = 0.0f;

    for (int i = 0; i <= n; ++i){
        float term = (1.0f / factorial(i));

        if (term < epsilon)
            break;

        val += term;
    }

    return val;
}

int factorial(int n)
{
    int val = 1;

    for (int i = 1; i <= n; ++i)
        val *= i;

    return val;
}
