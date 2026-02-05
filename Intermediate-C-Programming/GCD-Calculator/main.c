#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

int main()
{
    int m, n;

    printf("Enter two integers: ");
    scanf("%d %d", &m, &n);

    printf("Greatest common divisor: %d\n", euclid(m,n));

    return 0;
}
