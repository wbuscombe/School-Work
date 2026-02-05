#Description

The value of the mathemtical constant *e* can be expressed as an infinite series:

*e* = 1 + 1/1! + 1/2! + 1/3! + ...

Write a program that approximates *e* by computing the value of

1 + 1/1! + 1/2! + 1/3! + ... + 1/*n*!

where *n* is an integer entered by the user.

Continue adding terms until the current term becomes less than *E*, where *E* is a small (floaitng-point) number entered bby the user.

************************************************************************************************************************************************************
Sample run:
************************************************************************************************************************************************************

    $ make

        gcc main.c functions.c -Wall -std=c99 -lm -o project01_04

    $ ./project01_04 

        Enter a value for n: 15
        Enter a value for E (epsilon): .1

        e = 2.718282
