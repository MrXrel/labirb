#include "main.h"

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_ACCURACY 500
#define EPSILON 1e-9

enum return_code is_finite_representation(double fraction, int base, int* result) {
    int cnt = 0;
    int cap = 128;
    double *remainders = malloc(cap * sizeof(double)); 
    if (!remainders)
        return MEMORY_ALLOC_ERROR;  
    
    while (fraction > EPSILON && cnt < MAX_ACCURACY) {
        fraction *= base;
        int digit = (int)fraction;
        fraction -= digit;
        if (fraction < EPSILON) {
            *result = 1;
            return OK;
        }
        for (int i = 0; i < cnt; i++) {
            if (fabs(fraction - remainders[i]) < EPSILON) {
                *result = 0;
                free(remainders);
                return OK; 
            }
        }

        if (cnt == cap) {
            cap *= 2;
            double *temp = realloc(remainders, cap * sizeof(double));
            if (!temp) {
                free(remainders);
                return MEMORY_ALLOC_ERROR;  
            }
            remainders = temp;
        }
        remainders[cnt++] = fraction;
    }

    free(remainders);   
    *result = fraction < EPSILON; 

    return OK;
}


int check_numbers(int** massive_of_ans, int count, int base, ...) {
    if (!massive_of_ans) {
        return BAD_POINTER_ERROR;
    }
    if (!(base >= 2 && base <= 36)) {
        return BAD_INPUT_ERROR;
    }

    *massive_of_ans = (int*)malloc(count * sizeof(int));
    if (!(*massive_of_ans)) {
        return MEMORY_ALLOC_ERROR;
    }

    va_list args;
    va_start(args, base);
    for (int i = 0; i < count; ++i) {
        
        double number = va_arg(args, double);
        int digit = (int)number;
        double fraction = number - digit;

        if (number < 0 || number > 1) {
            return BAD_INPUT_ERROR;
        }

        int status = is_finite_representation(fraction, base, &(*massive_of_ans)[i]);
        if (status == MEMORY_ALLOC_ERROR) {
            return MEMORY_ALLOC_ERROR;
        }
    }
    va_end(args);
    return OK;
}

int main() {
    int* ans;
    check_numbers(&ans, 4, 10, 0.3, 0.1, 0.2, 0.545678765); 
    for (int i = 0; i < 4; ++i) {
        printf("%d ", ans[i]);
    }
    printf("\n");
    free(ans);

    check_numbers(&ans, 2, 2, 0.1, 0.5);
    for (int i = 0; i < 2; ++i) {
        printf("%d ", ans[i]);
    }
    printf("\n");
    free(ans);

    check_numbers(&ans, 5, 3, 0.1, 1.0/3, 0.33333, 0.5, 0.0);
    for (int i = 0; i < 5; ++i) {
        printf("%d ", ans[i]);
    }
    printf("\n");
    free(ans);
}