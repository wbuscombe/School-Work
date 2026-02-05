#include "functions.h"

int euclid(int m, int n)
{
    if (n == 0)
        return m;

    return euclid(n, m % n);
}
