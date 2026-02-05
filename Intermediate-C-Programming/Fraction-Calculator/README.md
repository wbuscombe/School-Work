#Description

Fraction Calculator:  Create a four function calculator that prints results in factional form and accepts fractions as terms.  Focus on creating a user friendly command line calculator.

************************************************************************************************************************************************************
Sample run:
************************************************************************************************************************************************************

    $ make
    
        gcc main.c functions.c -Wall -std=c99 -lm -o fraction-calc
        
    $ ./fraction-calc
    
        ******************************    Calculator    ******************************
        *                                                                            *
        *  Please enter an equation in the form: (operand) (operator) (operand)      *
        *                                                                            *
        *  Fractions must include a / with no additional whitespace                  *
        *                                                                            *
        *  Featured operations: + , - , * , /                                        *
        ******************************************************************************
        : 5/8 + 7/11

        5/8 + 7/11 = 111/88
        
    $ ./fraction-calc 
        
        ******************************    Calculator    ******************************
        *                                                                            *
        *  Please enter an equation in the form: (operand) (operator) (operand)      *
        *                                                                            *
        *  Fractions must include a / with no additional whitespace                  *
        *                                                                            *
        *  Featured operations: + , - , * , /                                        *
        ******************************************************************************
        : 7/10 - 1/2

        7/10 - 1/2 = 1/5
        
    $ ./fraction-calc 
    
        ******************************    Calculator    ******************************
        *                                                                            *
        *  Please enter an equation in the form: (operand) (operator) (operand)      *
        *                                                                            *
        *  Fractions must include a / with no additional whitespace                  *
        *                                                                            *
        *  Featured operations: + , - , * , /                                        *
        ******************************************************************************
        : 1/2 * 5/6

        1/2 * 5/6 = 5/12
        
    $ ./fraction-calc 
    
        ******************************    Calculator    ******************************
        *                                                                            *
        *  Please enter an equation in the form: (operand) (operator) (operand)      *
        *                                                                            *
        *  Fractions must include a / with no additional whitespace                  *
        *                                                                            *
        *  Featured operations: + , - , * , /                                        *
        ******************************************************************************
        : 5/8 / 1/4

        5/8 / 1/4 = 5/2
