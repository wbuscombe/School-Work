#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "functions.h"

int main()
{
    int nums[100];
    int count = 0;
    int rows, cols;

    readInput(nums, &count);

    printf("\n");

    // Test for square number
    if (fmod(sqrt(count), 1) != 0.0)
    {
        printf("Invalid input. You did not enter an amount of numbers equal to a perfect square.\n");
        return 0;
    }

    rows = cols = sqrt(count);

    // Print rows x cols grid
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            printf("%3d ", nums[(i * rows) + j]);
        printf("\n");
    }

    printf("\n");

    printf("Row sums: ");
    for (int i = 0; i < rows; ++i)
        printf("%d ",rowSum(nums, cols, i));

    printf("\n");

    printf("Column sums: ");
    for (int i = 0; i < cols; ++i)
        printf("%d ", colSum(nums, rows, i));

    printf("\n");

    printf("Diagonal sums: %d %d\n", topLeftDiagSum(nums, rows), topRightDiagSum(nums, rows));

    return 0;
}
