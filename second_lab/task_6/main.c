#include "main.h"

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

// VIII Hello 12 00101000011 FF ff 0.11
int main() {
    int roman = 0;
    char string[50];
    string[0] = '\0';
    int just_digit = 0;
    int zeck = 0;
    int in_16_base = 0;
    int in_17_base = 0;
    float float_value = 0;
    overfscanf(stdin, "%Ro %s %d %Zr %CV %Cv %f", &roman, string, &just_digit, &zeck, &in_16_base, 16, &in_17_base, 17, &float_value);
    printf("Roman %d\n", roman);
    printf("String %s\n", string);
    printf("Digit %d\n", just_digit);
    printf("Zeck %d\n", zeck);
    printf("FF in 16 base %d\n", in_16_base);
    printf("ff in 17 base %d\n", in_17_base);
    printf("Float value %f\n", float_value);
}
