#include <math.h>
#include <stdarg.h>
#include <float.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#include "functions.h"

enum return_code geometric_mean(double* result, int count, ...) {  
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (count <= 0) {
        return BAD_INPUT_ERROR;
    }

    va_list args;
    va_start(args, count);

    double product = 1.0;
    for (int i = 0; i < count; i++) {
        double num = va_arg(args, double);    
        if (num < 0) {
            return BAD_INPUT_ERROR;
        }    
        product *= num;
    }

    va_end(args);
    *result = pow(product, 1.0 / count);
    return OK;
}


double power(double x, int n) {
    if (n == 0) {
        return 1.0;
    }
    if (n < 0) {
        return 1.0 / power(x, -n);
    }
    if (n % 2 == 0) {
        double half = power(x, n / 2);
        return half * half;
    }
    if (n % 2 == 1) {
        return x * power(x, n - 1);
    }
    return 0;
}
