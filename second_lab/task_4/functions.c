#include "functions.h"

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

int is_valid_symbol_in_base(char c, int base) {
    if (c == '+' || c == '-') {
        return 1;
    }
    if (base >= 2 && base <= 9) {
        return (c >= '0' && c < ('0' + base));
    }
    // base >= 10
    return isdigit(c) || (toupper(c) >= 'A' && toupper(c) < ('A' + base - 10));
}

enum return_code convert_to_decimal(const char* num_repr_in_base, int len_of_num_repr, int base, int* decimal_result) {
    if (!num_repr_in_base || !decimal_result) {
        return BAD_POINTER_ERROR;
    }
    if (!(base >= 2 && base <= 36) || len_of_num_repr < 0) {
        return BAD_INPUT_ERROR;
    }

    *decimal_result = 0;
    int value = 0;
    int sign = 1;

    if (len_of_num_repr == 0) {
        return OK;
    }

    for (int i = 0; i < len_of_num_repr; i++) {
        char c = toupper(num_repr_in_base[i]);

        if (!is_valid_symbol_in_base(c, base)) {
            return BAD_INPUT_ERROR;
        }
        if (i == 0 && c == '+') {
            continue;
        }
        if (i == 0 && c == '-') {
            sign = -1;
            continue;
        }

        if (isdigit(c)) {
            value = c - '0';
        } else if (isalpha(c)) {
            value = c - 'A' + 10;
        } else {
            return BAD_INPUT_ERROR;
        }

        if (*decimal_result > INT_MAX / base) {
            return OVERFLOW_ERROR;
        }
        *decimal_result *= base;
        if (value > 0 && (*decimal_result > INT_MAX - value)) {
            return OVERFLOW_ERROR;
        }
        *decimal_result += value;
    }
    *decimal_result *= sign;
    return OK;
}

double power(double x, int n) {
    double result = 1.0;
    int exponent = n;

    if (n < 0) {
        x = 1.0 / x;
        exponent = -exponent;
    }

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result *= x;
        }
        x *= x;
        exponent /= 2;
    }

    return result;
}

enum return_code is_convex(int* result, int count, ...) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    va_list args;
    va_start(args, count);

    Point* points = (Point*)malloc(count * sizeof(Point));
    if (!points) {
        return MEMORY_ALLOC_ERROR;
    }

    for (int i = 0; i < count; i++) {
        points[i].x = va_arg(args, double);
        points[i].y = va_arg(args, double);
    }
    va_end(args);

    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            if (fabs(points[i].x - points[j].x) < EPS && fabs(points[i].y - points[j].y) < EPS) {
                *result = 0;  // equal points
                return OK;
            }
        }
    }

    int is_collinear = 1;  // all points on one line
    int sign = 0;
    for (int i = 0; i < count; i++) {
        double dx1 = points[(i + 2) % count].x - points[(i + 1) % count].x;
        double dy1 = points[(i + 2) % count].y - points[(i + 1) % count].y;
        double dx2 = points[i].x - points[(i + 1) % count].x;
        double dy2 = points[i].y - points[(i + 1) % count].y;

        double crossProduct = dx1 * dy2 - dy1 * dx2;

        if (fabs(crossProduct) > EPS) {
            is_collinear = 0;  // some points not on one line
            if (sign == 0) {
                sign = (crossProduct > EPS) ? 1 : -1;
            } else if (sign * crossProduct < 0) {
                free(points);
                *result = 0;
                return OK;
            }
        }
    }
    free(points);
    *result = !is_collinear;  // one of points isn't on line
    return OK;
}

enum return_code evaluate_polynomial(double* result, double x, int degree, ...) {
    va_list args;
    va_start(args, degree);

    double ans = 0;
    double x_value = 1;
    for (int i = 0; i <= degree; i++) {
        double coefficient = va_arg(args, double);
        ans += coefficient * x_value;
        x_value *= x;
    }

    va_end(args);
    *result = ans;
    return OK;
}

enum return_code is_kaprekar(int n, int base, int* is_kap) {
    if (!is_kap) {
        return BAD_POINTER_ERROR;
    }
    if (!(base >= 2 && base <= 36) || n <= 0) {
        return BAD_INPUT_ERROR;
    }

    if (n == 1) {
        *is_kap = 1;
        return OK;
    }
    int square = n * n;
    int len_of_number = 0;
    int temp = square;

    while (temp > 0) {
        temp /= base;
        len_of_number++;
    }

    for (int i = 1; i < len_of_number; i++) {
        int divisor = pow(base, i);
        int left = square / divisor;
        int right = square % divisor;

        if (left + right == n && right != 0) {
            *is_kap = 1;
            return OK;
        }
    }
    *is_kap = 0;
    return OK;
}

enum return_code find_kaprekar_nums(int** massive_of_ans, int base, int count, ...) {
    if (!massive_of_ans) {
        return BAD_POINTER_ERROR;
    }

    *massive_of_ans = (int*)malloc(count * sizeof(int));
    if (!(*massive_of_ans)) {
        return MEMORY_ALLOC_ERROR;
    }
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; ++i) {
        char* num_str_repr = va_arg(args, char*);
        int decimal = 0;
        int result_of_convert = convert_to_decimal(num_str_repr, strlen(num_str_repr), base, &decimal);
        if (result_of_convert != OK) {
            return result_of_convert;
        }
        int res_of_check = is_kaprekar(decimal, base, &(*massive_of_ans)[i]);
        if (res_of_check != OK) {
            return res_of_check;
        }
    }
    return OK;
}