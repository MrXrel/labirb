#include "main.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("10 ^ 2 = %lf\n",power(10, 2));
    printf("10 ^ -2 = %lf\n",power(10, -2));
    printf("1.2 ^ 53 = %lf\n",power(1.2, 53));
    printf("0 ^ 0 = %lf\n", power(0, 0));
    printf("12 ^ 12 = %lf\n", power(12, 12));
    printf("-11.6 ^ 3 = %lf\n", power(-11.6, 3));
    printf("-7.2 ^ 2 = %lf\n", power(-7.2, 2));

    printf("\n");
    
    double result[4];
    geometric_mean(&result[0], 5, (double)1, (double)2, (double)3, (double)4, (double)5);
    geometric_mean(&result[1], 1, (double)2);
    geometric_mean(&result[2], 2, (double)2, (double)2);
    geometric_mean(&result[3], 4, (double)124, (double)122, (double)1, (double)5);
    
    printf("Geometric mean of (1 2 3 4 5) = %lf\n", result[0]);
    printf("Geometric mean of (2) = %lf\n", result[1]);
    printf("Geometric mean of (2 2) = %lf\n", result[2]);
    printf("Geometric mean of (124 122 1 5) = %lf\n", result[3]);
    
}