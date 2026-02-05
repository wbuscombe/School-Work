#Description

Write a program that asks the user to enter the numbers from 1 to 16 (in any order) and then displays the numbers in a 4 by 4 arrangement, followed by the sums of the rows, columns, and diagonals.

************************************************************************************************************************************************************
Sample run:
************************************************************************************************************************************************************

    $ make
        
        gcc main.c functions.c -Wall -std=c99 -lm -o magic-square

    $ ./magic-square 

        16 3 2 13 5 10 11 8 9 6 7 12 4 15 14 1

        16   3   2  13 
        5  10  11   8 
        9   6   7  12 
        4  15  14   1 

        Row sums: 34 34 34 34 
        Column sums: 34 34 34 34 
        Diagonal sums: 34 34
