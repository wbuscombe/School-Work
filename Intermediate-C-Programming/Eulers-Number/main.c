#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

int main()
{
    int n;
    float epsilon;

    printf("Enter a value for n: ");
    scanf("%d", &n);

    printf("Enter a value for E (epsilon): ");
    scanf("%f", &epsilon);

    printf("\n");

    printf("e = %f\n", computeE(n, epsilon));

    return 0;
}
