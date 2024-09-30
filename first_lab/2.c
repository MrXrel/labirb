#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>

enum return_code {
    OK = 0,
    BAD_INPUT_ERROR = 1,
    OVERFLOW_ERROR = 2,
    BAD_POINTER_ERROR = 3,
    MEMORY_ALLOC_ERROR = 4,
    WRONG_ARGUMENTS_ERROR = 5,
    NOT_A_DOUBLE_ERROR = 6,
    NOT_AN_INTEGER_ERROR = 7,
};

#define M_PI 3.14159265358979323846
const double EPS = 0.000000001;
typedef double (*function)(double n);

int is_double_overflow(double to_check) {
    if ((fabs(to_check) > DBL_MAX) || to_check == HUGE_VAL || to_check == -HUGE_VAL || isnan(to_check) || isinf(to_check)) {
        return 1;
    }
    return 0;
}

int transfer_to_double(const char* str, double* num) {
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
    return 0;
}
double calculate_e_limit(double eps) {
    double current_e = 1;
    double prev_e = 0;
    int i = 1;
    while (fabs(current_e - prev_e) > eps) {
        prev_e = current_e;
        current_e = pow(1 + 1.0 / i, i);
        ++i;
    }
    return current_e;
}

double calculate_pi_limit(double eps) {
    double current = 4, prev, buff = 16;
    long n = 2;
    long fact_2n = 2, fact_n = 1;
    do {
        prev = current;
        buff *= 16;
        
        current = buff / n;
    } while (fabs(current - prev) < eps);
    return current;
}

// Function to calculate e using the series expansion
double calculate_e_sum(double eps) {
    double e = 1.0;
    double fact = 1.0;
    int i = 1;
    double add = 1;
    while (1.0 / fact > eps) {
        e += add;
        add /= i;
        ++i;
    }
    return e;
}

double calculate_ln_sum(double eps) {
    double ln = 0;
    double add = 100;
    int i = 1;
    while (fabs(add) > eps) {
        add = ((double)1) / i;

        if ((i - 1) % 2 == 0) {
            ln += add;
        } else {
            ln -= add;
        }
        ++i;
    }
    return ln;
}

double calculate_pi_sum(double eps) {
    double pi = 0;
    double add = 100;
    int i = 1;
    do {
        add = ((double)1) / (2 * i - 1);

        if ((i - 1) % 2 == 0) {
            pi += add;
        } else {
            pi -= add;
        }
        ++i;
    } while (add > eps);
    return 4 * pi;
}

double calculate_sqrt2_sum(double eps) {
    double sqrt2 = 1;
    double degree2 = 0.25;
    while (fabs(degree2) > eps) {
        sqrt2 *= pow(2, degree2);
        degree2 *= 0.5;
    }
    return sqrt2;
}

double calculate_sigma_sum(double eps) {
    double sigma = 0, add = eps + 1;
    long k = 2, sqrt_floor;
    do {
        sqrt_floor = (long)sqrt(k);
        if (sqrt_floor * sqrt_floor == k) {
            ++k;
            sqrt_floor = (long)sqrt(k);
        }
        add = (1.0 / (sqrt_floor * sqrt_floor)) - (1.0 / k);
        sigma += add;
        ++k;
    } while (fabs(add) > eps);
    
    double pi = calculate_pi_sum(eps);
    return sigma - (pi * pi / 6);
}

double exp_function(double x) {
    return exp(x);
}

double square_function(double x) {
    return x * x;
}

double calculate_equation(double left, double right, double equals, function f, double eps) {
    double middle = (left + right) / 2;
    double res = f(middle);

    if (fabs(res - equals) < eps) {
        return middle;
    } else if (res > equals) {
        return calculate_equation(left, middle, equals, f, eps);
    } else {
        return calculate_equation(middle, right, equals, f, eps);
    }
}


int main() {
    double eps = EPS;
    printf("%.15lf\n", calculate_pi_limit(1e-6));

    // printf("%.15f\n", find_sol(1000, &calculate_e_limit, eps));
    // printf("%.15f\n", find_sol(1000, &calculate_e_sum, eps));
    // printf("%.15f\n", calculate_equation(-10, 10, 1, &log, eps));
    // double root = calculate_equation(0, 1, 2, &exp_function, eps);
    // printf("%.15f\n", root);
    // printf("%.15f\n", find_sol(1000, &calculate_ln_sum, eps));
    // printf("%.15f\n", find_sol(1000, &calculate_pi_sum, eps));
    // printf("%.15f\n", find_sol(1000, &calculate_square2_sum, eps));
    // printf("%.15f\n", find_sol(1000, &calculate_sigma_sum, eps));
    // root = calculate_equation(0, 2, 2, &square_function, eps);
    // printf("%.15f\n", root);
    // printf("%.15f\n", calculate_e_limit(eps));
    // printf("%.15f\n", calculate_sqrt2_sum(eps));
    // printf("%.15f\n", sqrt(2));
    // printf("%.15f\n", calculate_pi_sum(eps));
}