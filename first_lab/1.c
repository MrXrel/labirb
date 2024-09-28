#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum return_code {
    OK = 0,
    BAD_INPUT_ERROR = 1,
    OVERFLOW_ERROR = 2,
    BAD_POINTER_ERROR = 3,
    MEMORY_ALLOC_ERROR = 4,
    WRONG_ARGUMENTS_ERROR = 5,

};

int convert_str_to_num(const char* str, int* num) {
    if (str == NULL || num == NULL) {
        return BAD_POINTER_ERROR;
    }

    char* end_ptr = NULL;

    long tmp = strtol(str, &end_ptr, 10);
    if (*end_ptr != '\0') {
        return BAD_INPUT_ERROR;
    }
    if (tmp > INT_MAX || tmp < INT_MIN) {
        return OVERFLOW_ERROR;
    }

    *num = (int)tmp;

    return OK;
}

int for_h(int num, int* range_100_list, int* count) {
    if (range_100_list == NULL || count == NULL) {
        return BAD_POINTER_ERROR;
    }
    if (num == 0) {
        return BAD_INPUT_ERROR;
    }
    int i = 1;
    *count = 0;
    num = abs(num);

    while (num * i <= 100) {
        range_100_list[*count] = num * i;
        ++(*count);
        ++i;
    }
    return OK;
}

int for_p(const int x, int* is_prime) {
    if (is_prime == NULL) {
        return BAD_POINTER_ERROR;
    }
    if (x <= 0) {
        return BAD_INPUT_ERROR;
    }
    *is_prime = 1;
    if (x == 1) {
        *is_prime = 2;
        return OK;
    }

    for (int i = 2; i * i <= x; ++i) {
        if (x % i == 0) {
            *is_prime = 0;
            break;
        }
    }
    return OK;
}

int for_s(int x, char* hex_repr, int* ind) {
    if (hex_repr == NULL || ind == NULL) {
        return BAD_POINTER_ERROR;
    }
    *ind = 0;
    if (x == 0) {
        hex_repr[0] = '0';
        ++(*ind);
        return OK;
    }

    while (x > 0) {
        int digit = x % 16;
        if (digit < 10) {
            hex_repr[*ind] = '0' + digit;
        } else {
            hex_repr[*ind] = 'A' + (digit - 10);
        }
        ++(*ind);
        x /= 16;
    }
    return OK;
}

int for_e(int x, long*** list_of_nums) {
    if (x > 10 || x <= 0) {
        return BAD_INPUT_ERROR;
    }
    *list_of_nums = (long**)malloc(10 * sizeof(long*));
    if (*list_of_nums == NULL) {
        return MEMORY_ALLOC_ERROR;
    }

    for (int i = 0; i < 10; ++i) {
        (*list_of_nums)[i] = (long*)malloc(x * sizeof(long));
        if ((*list_of_nums)[i] == NULL) {
            for (int j = 0; j < i; ++j) {
                free((*list_of_nums)[j]);
            }
            free(*list_of_nums);
            *list_of_nums = NULL;
            return MEMORY_ALLOC_ERROR;
        }
    }
    for (int i = 1; i <= 10; ++i) {
        long res = i;
        for (int j = 1; j <= x; ++j) {
            (*list_of_nums)[i - 1][j - 1] = res;
            res *= i;
        }
    }
    // print_list_of_nums
    // free
    return OK;
}

int for_f(const int x, long* fact) {
    if (fact == NULL) {
        return BAD_POINTER_ERROR;
    }
    if (x < 0) {
        return BAD_INPUT_ERROR;
    }
    *fact = 1;
    if (x == 0 || x == 1) {
        return OK;
    }

    for (int i = 2; i <= x; ++i) {
        if (*fact > LLONG_MAX / i) {
            return OVERFLOW_ERROR;
        }
        *fact *= i;
    }
    return OK;
}

int for_a(const int x, int* result) {
    if (x <= 0) {
        return BAD_INPUT_ERROR;
    }
    if (result == NULL) {
        return BAD_POINTER_ERROR;
    }

    *result = 0;
    for (int i = 1; i <= x; ++i) {
        if (*result > INT_MAX - i) {
            return OVERFLOW_ERROR;
        }
        *result += i;
    }
    return OK;
}

int is_flag_valid(const char* flag) {
    if (!(flag[0] == '-' || flag[0] == '/') || strlen(flag) != 2) {
        return BAD_INPUT_ERROR;
    }
    switch (flag[1]) {
        case 'h':
        case 'p':
        case 's':
        case 'e':
        case 'a':
        case 'f':
            return OK;
        default:
            return BAD_INPUT_ERROR;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong value of arguments\n");
        return WRONG_ARGUMENTS_ERROR;
    }

    int num = 0;
    if (convert_str_to_num(argv[1], &num) != OK || is_flag_valid(argv[2]) != OK) {
        printf("Invalid input\n");
        return WRONG_ARGUMENTS_ERROR;
    }
    switch (argv[2][1]) {
        case 'h':;
            int list_of_nums[100];
            int count = 0;
            int func_result_h = for_h(num, list_of_nums, &count);

            if (func_result_h == BAD_POINTER_ERROR) {
                printf("Pointer is equal NULL\n");
                return BAD_POINTER_ERROR;
            }
            if (func_result_h == BAD_INPUT_ERROR) {
                printf("Zero divizion\n");
                return BAD_INPUT_ERROR;
            }

            for (int i = 0; i < count; ++i) {
                printf("%d ", list_of_nums[i]);
            }
            if (count == 0) {
                printf("No such numbers in range 1-100\n");
            }
            printf("\n");
            break;

        case 'p':;
            int is_prime = 0;
            int func_res_p = for_p(num, &is_prime);
            if (func_res_p == BAD_POINTER_ERROR) {
                printf("Pointer is equal NULL\n");
                return BAD_POINTER_ERROR;
            }
            if (func_res_p == BAD_INPUT_ERROR) {
                printf("Negative or zero number\n");
                return BAD_INPUT_ERROR;
            }
            if (is_prime == 1) {
                printf("Prime\n");
            } else if (is_prime == 0) {
                printf("Composite\n");
            } else {
                printf("Not a prime and not a composite.\n");
            }
            break;

        case 'f':;
            long fact = 1;
            int func_res_f = for_f(num, &fact);

            switch (func_res_f) {
                case OVERFLOW_ERROR:
                    printf("Overflow detected\n");
                    return OVERFLOW_ERROR;
                case BAD_INPUT_ERROR:
                    printf("Negative number\n");
                    return BAD_INPUT_ERROR;
                case BAD_POINTER_ERROR:
                    printf("Pointer is NULL\n");
                    return BAD_POINTER_ERROR;
                default:
                    break;
            }
            printf("%ld\n", fact);
            break;
        case 'a':;
            int sum_a = 0;
            switch (for_a(num, &sum_a)) {
                case OVERFLOW_ERROR:
                    printf("Overflow detected\n");
                    return OVERFLOW_ERROR;
                case BAD_INPUT_ERROR:
                    printf("Negative or zero number\n");
                    return BAD_INPUT_ERROR;
                case BAD_POINTER_ERROR:
                    printf("Pointer is NULL\n");
                    return BAD_POINTER_ERROR;
                default:
                    break;
            }
            printf("%d\n", sum_a);
            break;

        case 's':;
            char hex_repr[20];
            int ind = 0;
            if (for_s(num, hex_repr, &ind) == BAD_POINTER_ERROR) {
                printf("Pointer is NULL\n");
                return BAD_POINTER_ERROR;
            }
            for (int i = ind - 1; i >= 0; --i) {
                printf("%c", hex_repr[i]);
            }
            printf("\n");
            break;

        case 'e': ;
            long** nums;
            switch (for_e(num, &nums)) {
                case BAD_INPUT_ERROR:
                    printf("Number should be in range 1-10");
                    return BAD_INPUT_ERROR;
                case MEMORY_ALLOC_ERROR:
                    printf("Memory allocation error\n");
                    return MEMORY_ALLOC_ERROR;
                default:
                    break;
            }
            for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < num; ++j) {
                    printf("%ld ", nums[i][j]);
                }
                printf("\n");
            }
            for (int i = 0; i < 10; ++i) {
                free(nums[i]);
            }
            free(nums);
            break;
        default:
            printf("Invalid input\n");
            return BAD_INPUT_ERROR;
    }
}