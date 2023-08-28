#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>

int hex_to_decimal(int hex_value)
{
    int decimal = 0;
    int base = 1;

    while (hex_value > 0)
    {
        int remainder = hex_value % 16;
        decimal += remainder * base;
        hex_value /= 16;
        base *= 10;
    }

    return decimal;
}