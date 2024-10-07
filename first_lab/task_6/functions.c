#include "functions.h"

#include <math.h>
#include <stdio.h>
#include <float.h>
#include <stdlib.h>

typedef double (*function)(double x);
typedef double (*method_to_calculate)(function f, double a, double b);

int is_double_overflow(double to_check) {
    if ((fabs(to_check) > DBL_MAX) || to_check == HUGE_VAL || to_check == -HUGE_VAL || isnan(to_check) || isinf(to_check)) {
        return 1;
    }
    return 0;
}

enum return_code transfer_to_double(const char* str, double* num) {
    if (!str || !num) {
        return BAD_POINTER_ERROR;
    }
    char* endptr;
    double value = strtod(str, &endptr);

    if (*endptr != '\0') {
        return NOT_A_DOUBLE_ERROR;
    }
    if (is_double_overflow(value)) {
        return OVERFLOW_ERROR;
    }
    *num = value;
    return OK;
}

double func_a(double x) {
    return log(1 + x) / x;
}

double func_b(double x) {
    return exp(-x * x * 0.5);
}

double func_c(double x) {
    return -log(1 - x);
}

double func_d(double x) {
    return pow(x, x);
}

double method_sympson(function f, double a, double b) {
    return ((b - a) / 6) * (f(a) + 4 * f((a + b) * 0.5) + f(b));
}

double method_rect_center(function f, double a, double b) {
    return f((a + b) * 0.5) * fabs(b - a);
}

double method_rect_left(function f, double a, double b) {
    return f(a) * fabs(b - a);
}

double method_rect_right(function f, double a, double b) {
    return f(b) * fabs(b - a);
}

enum return_code calculate_integral(function f, double a, double b, double eps,
                                    method_to_calculate method_to_calculate, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (a > b || eps <= 0) {
        return BAD_INPUT_ERROR;
    }

    double prev = 0, current = 0;
    double step = (b - a);
    do {
        prev = current;
        step /= 2;
        current = 0;
        for (double i = a; fabs(i - b) > eps; i += step) {
            current += method_to_calculate(f, i, i + step);
        }
    } while (fabs(current - prev) > eps);
    *result = current;
    if (isnan(*result)) {
        return BAD_CALCULATIONS_ERROR;
    }
    return OK;
}

enum return_code calculate_point(function f, double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    calculate_integral(f, 0, 1, eps, method_rect_center, result);
    return OK;
}
