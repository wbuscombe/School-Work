#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

#define delims " \t\r\n"

#define MAX_LEN 128

void readInput(int nums[], int *count)
{

    char c = 0, input[MAX_LEN], *buffer;
    int index = 0;

    while(1)
	{
		c = getc(stdin);

		if (c == '\n')
		{
			input[index] = '\0';
			break;
		}
		else
		{
			input[index++] = c;
		}
	}

    buffer = strtok(input, delims);

    nums[(*count)++] = atoi(buffer);

    while ((buffer = strtok(NULL, delims)) != NULL)
        nums[(*count)++] = atoi(buffer);

    buffer = strtok(NULL, delims);
}

int rowSum(int nums[], int cols, int row)
{
    int sum = 0;

    for (int i = 0; i < cols; ++i)
        sum += nums[(row * cols) + i];

    return sum;
}

int colSum(int nums[], int rows, int col)
{
    int sum = 0;

    for (int i = 0; i < rows; ++i)
        sum += nums[(rows * i) + col];

    return sum;
}

int topLeftDiagSum(int nums[], int rows)
{
    int sum = 0;

    for (int i = 0; i < rows; ++i)
        sum += nums[(rows * i) + i];

    return sum;
}

int topRightDiagSum(int nums[], int rows)
{
    int sum  = 0;

    for (int i = 0; i < rows; ++i)
        sum += nums[(rows * (rows - (i + 1))) + i];

    return sum;
}
