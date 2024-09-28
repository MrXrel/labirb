#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int equal_doubles(double a, double b, double eps) {
    return (fabs(a - b) < eps);
}

int is_double_overflow(double to_check) {
    if ((fabs(to_check) > DBL_MAX) || to_check == HUGE_VAL || to_check == -HUGE_VAL || isnan(to_check) || isinf(to_check)) {
        return 1;
    }
    return 0;
}

int solve_quadratic(double a, double b, double c, double eps, double* x_values, int* current_ind) {
    if (!x_values || !current_ind) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }

    double discriminant = b * b - 4 * a * c;
    if (is_double_overflow(discriminant)) {
        return OVERFLOW_ERROR;
    }

    if (fabs(discriminant) < eps) {
        x_values[*current_ind] = -b / (2 * a);
        x_values[(*current_ind) + 1] = -b / (2 * a);
        *current_ind += 1;
    } else if (discriminant > eps) {
        x_values[2 * (*current_ind)] = (-b + sqrt(discriminant)) / (2 * a);
        x_values[2 * (*current_ind) + 1] = (-b - sqrt(discriminant)) / (2 * a);
        // printf("%d %d: %lf %lf\n", 2 *(*current_ind), 2 * (*current_ind) + 1, x_values[*current_ind], x_values[(*current_ind) + 1]);
        *current_ind += 1;
    }
    return OK;
}

int is_coefs_in_list(double a, double b, double c, double eps, int current_index_for_coefs, double* already_checked_coefs, int* is_in_list) {
    if (eps <= 0 || current_index_for_coefs < 0) {
        return BAD_INPUT_ERROR;
    }
    if (!already_checked_coefs || !is_in_list) {
        return BAD_POINTER_ERROR;
    }
    *is_in_list = 0;
    double a_in_list, b_in_list, c_in_list;
    for (int i = 0; i < current_index_for_coefs * 3; i += 3) {
        a_in_list = already_checked_coefs[i], b_in_list = already_checked_coefs[i + 1],
        c_in_list = already_checked_coefs[i + 2];

        if (equal_doubles(a, a_in_list, eps) && equal_doubles(b, b_in_list, eps) && equal_doubles(c, c_in_list, eps)) {
            *is_in_list = 1;
            return OK;
        }
    }
    return OK;
}

int add_solution(double a, double b, double c, double eps, int* current_index, double* already_checked, double* x_values) {
    if (!already_checked || !current_index || !x_values) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    int result_of_search_in_list, is_in_list;
    result_of_search_in_list = is_coefs_in_list(a, b, c, eps, *current_index, already_checked, &is_in_list);
    if (result_of_search_in_list != OK) 
    {
        return result_of_search_in_list;
    }
    int result_of_solve;
    int old_ind;
    if (!is_in_list) { 
        old_ind = *current_index;
        result_of_solve = solve_quadratic(a, b, c, eps, x_values, current_index);
        if (result_of_solve != OK) {
            return result_of_solve;
        }
        if (old_ind < *current_index) {
            already_checked[3 * old_ind] = a;
            already_checked[3 * old_ind + 1] = b;
            already_checked[3 * old_ind + 2] = c;
        }
    }
    return OK;
}

int for_q(double a, double b, double c, double eps, int* current_index_for_coefs, double* already_checked_coefs, double* x_values) {
    if (!x_values || !already_checked_coefs || !current_index_for_coefs) {
        return BAD_POINTER_ERROR;
    }

    *current_index_for_coefs = 0;
    int result_of_add;
    result_of_add = add_solution(a, b, c, eps, current_index_for_coefs, already_checked_coefs, x_values);
    if (result_of_add != OK) {
        return result_of_add;
    }
    result_of_add = add_solution(a, c, b, eps, current_index_for_coefs, already_checked_coefs, x_values);
    if (result_of_add != OK) {
        return result_of_add;
    }
    result_of_add = add_solution(b, a, c, eps, current_index_for_coefs, already_checked_coefs, x_values);
    if (result_of_add != OK) {
        return result_of_add;
    }
    result_of_add = add_solution(b, c, a, eps, current_index_for_coefs, already_checked_coefs, x_values);
    if (result_of_add != OK) {
        return result_of_add;
    }
    result_of_add = add_solution(c, a, b, eps, current_index_for_coefs, already_checked_coefs, x_values);
    if (result_of_add != OK) {
        return result_of_add;
    }
    result_of_add = add_solution(c, b, a, eps, current_index_for_coefs, already_checked_coefs, x_values);
    if (result_of_add != OK) {
        return result_of_add;
    }
    return OK;
}

int for_m(const int first_number, const int second_number, int* is_multiple) {
    if (is_multiple == NULL) {
        return BAD_POINTER_ERROR;
    }
    if (first_number == 0 || second_number == 0) {
        return BAD_INPUT_ERROR;
    }

    if (first_number % second_number == 0) {
        *is_multiple = 1;
    } else {
        *is_multiple = 0;
    }
    return OK;
}

int for_t(const double a, const double b, const double c, const double eps, int* is_rectangle_triag) {
    if (!is_rectangle_triag) {
        return BAD_POINTER_ERROR;
    }
    if (a <= 0 || b <= 0 || c <= 0 || eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    *is_rectangle_triag = 0;
    double aa = a * a, bb = b * b, cc = c * c;
    if (is_double_overflow(aa) || is_double_overflow(bb) || is_double_overflow(cc) || 
    is_double_overflow(aa + bb) || is_double_overflow(aa + cc) || is_double_overflow(cc + bb)) {
        return OVERFLOW_ERROR;
    }

    if ((fabs(aa + bb - cc) < eps) || (fabs(aa + cc - bb) < eps) || (fabs(bb + cc - aa) < eps)) {
        *is_rectangle_triag = 1;
        return OK;
    }
    *is_rectangle_triag = 0;
    return OK;
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

int transfer_to_int(const char* str, int* result) {
    int len = strlen(str);
    int sign = 1;
    for (int i = 0; i < len; i++) {
        if ((i == 0) && str[i] == '-') {
            sign = -1;
            continue;
        }
        if (!(str[i] >= '0' && str[i] <= '9')) {
            return NOT_AN_INTEGER_ERROR;
        }
        if (*result > (INT_MAX - (str[i] - '0')) / 10) {
            return OVERFLOW_ERROR;
        }

        *result *= 10;
        *result += (str[i] - '0');
    }
    *result *= sign;
    return 0;
}

int is_flag_valid(const char* flag) {
    if (!(flag[0] == '-' || flag[0] == '/') || strlen(flag) != 2) {
        return BAD_INPUT_ERROR;
    }
    switch (flag[1]) {
        case 'q':
        case 'm':
        case 't':
            return OK;
        default:
            return BAD_INPUT_ERROR;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("No flags\n");
        return WRONG_ARGUMENTS_ERROR;
    }

    if (is_flag_valid(argv[1]) != OK) {
        printf("No such flag\n");
        return WRONG_ARGUMENTS_ERROR;
    }

    switch (argv[1][1]) {
        case 't': ;
            if (argc != 6) {
                printf("Wrong amount of arguments\n");
                return WRONG_ARGUMENTS_ERROR;
            }
            double sides[4];

            for (int i = 0; i < 4; ++i) {
                if (transfer_to_double(argv[i + 2], &sides[i]) != OK) {
                    printf("Invalid input\n");
                    return WRONG_ARGUMENTS_ERROR;
                }
            }
            // check epsilon
            if (sides[0] <= 0) {
                printf("Invalid input\n");
                return WRONG_ARGUMENTS_ERROR;
            }
            int is_rectangular = 0;
            int result_t_func = for_t(sides[1], sides[2], sides[3], sides[0], &is_rectangular);

            switch (result_t_func) {
                case OVERFLOW_ERROR:
                    printf("Overflow detected\n");
                    return OVERFLOW_ERROR;
                case BAD_POINTER_ERROR:
                    printf("Pointer is NULL\n");
                    return BAD_POINTER_ERROR;
                case BAD_INPUT_ERROR:
                    printf("Wrong arguments\n");
                    return BAD_INPUT_ERROR;
                default:
                    break;
            }
            if (is_rectangular) {
                printf("Rectangular\n");
            }
            else {
                printf("Not rectangular\n");
            }
            break;
        case 'm': ;
            if (argc != 4) {
                printf("Wrong amount of arguments\n");
                return WRONG_ARGUMENTS_ERROR;
            }
            int nums[2];
            for (int i = 0; i < 2; ++i) {
                if (transfer_to_int(argv[i + 2], &nums[i]) != OK) {
                    printf("Invalid input\n");
                    return WRONG_ARGUMENTS_ERROR;
                }
            }
            int is_multiple = 0;
            int result_m_func = for_m(nums[0], nums[1], &is_multiple);
            switch (result_m_func) {
                case BAD_POINTER_ERROR:
                    printf("Pointer is NULL\n");
                    return BAD_POINTER_ERROR;
                case BAD_INPUT_ERROR:
                    printf("Zero divizion error\n");
                    return BAD_INPUT_ERROR;
                
                default:
                    break;
            }
            if (is_multiple) {
                printf("Multiple\n");
            }
            else {
                printf("Not multiple\n");
            }


            break;

        case 'q':;
            if (argc != 6) {
                printf("Wrong amount of arguments\n");
                return WRONG_ARGUMENTS_ERROR;
            }
            double coefs[4];

            for (int i = 0; i < 4; ++i) {
                if (transfer_to_double(argv[i + 2], &coefs[i]) != OK) {
                    printf("Invalid input\n");
                    return WRONG_ARGUMENTS_ERROR;
                }
            }
            // check epsilon
            if (coefs[0] <= 0) {
                printf("Invalid input\n");
                return WRONG_ARGUMENTS_ERROR;
            }

            double used_coefs[18];
            double x_values[12];
            int ind = 0;

            int result_q_func = for_q(coefs[1], coefs[2], coefs[3], coefs[0], &ind, used_coefs, x_values);
            switch (result_q_func) {
                case OVERFLOW_ERROR:
                    printf("Overflow detected\n");
                    return OVERFLOW_ERROR;
                case BAD_POINTER_ERROR:
                    printf("Pointer is NULL\n");
                    return BAD_POINTER_ERROR;
                case BAD_INPUT_ERROR:
                    printf("Wrong arguments\n");
                    return BAD_INPUT_ERROR;
                
                default:
                    break;
            }

            for (int i = 0; i < ind; ++i) {
                printf("a = %lf, b = %lf, c = %lf: x1 = %lf, x2 = %lf\n", used_coefs[i * 3], used_coefs[i * 3 + 1], used_coefs[i * 3 + 2], x_values[2 * i], x_values[2 * i + 1]);
            }
            break;
    }
    return OK;
}
