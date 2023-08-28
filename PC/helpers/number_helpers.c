#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

float build_float(int integer, int decimal)
{
    int digits_qty = (int)log10(decimal) + 1; // Conta o número de dígitos na parte decimal
    float divisor = pow(10, digits_qty);
    return (float)integer + ((float)decimal / divisor);
}