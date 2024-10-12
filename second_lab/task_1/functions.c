#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <limits.h>

#include "functions.h"
#include "string.h"

void print_error_message(int return_code) {
    switch (return_code) {
        case MEMORY_ALLOC_ERROR:
            printf("Memory allocation error\n");
            break;
        case BAD_POINTER_ERROR:
            printf("Dereferencing NULL pointer\n");
            break;
        case BAD_INPUT_ERROR:
            printf("Invalid input");
            break;
        case OVERFLOW_ERROR:
            printf("Overflow detected\n");
            break;
        case NOT_AN_INTEGER_ERROR:
            printf("Invalid input");
            break;
        case OK:
            break;
        default:
            break;
    }
}

int transfer_to_int(const char* str, int* result) {
    if (!str || !result) {
        return BAD_POINTER_ERROR;
    }
    int len = my_strlen(str);
    int sign = 1;
    *result = 0;
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
    return OK;
}

enum return_code is_valid_flag(char const* flag) {
    if (my_strlen(flag) != 2 || !(flag[0] == '-' || flag[0] == '/')) {
        return BAD_INPUT_ERROR;
    }
    switch (flag[1]) {
        case 'l':
        case 'r':
        case 'u':
        case 'n':
        case 'c':
            return OK;
        default:
            return BAD_INPUT_ERROR;
    }
}

enum return_code shuffle(int* array_of_nums, int seed, int size_of_array) {
    // fisher-yates shuffle
    if (!array_of_nums) {
        return BAD_POINTER_ERROR;
    }
    if (size_of_array < 0) {
        return BAD_INPUT_ERROR;
    }
    srand(seed);
    for (int i = size_of_array - 1; i > 0; --i) {
        int rand_ind = rand() % i;

        int temp = array_of_nums[i];
        array_of_nums[i] = array_of_nums[rand_ind];
        array_of_nums[rand_ind] = temp;
    }
    return OK;
}

enum return_code transform_to_upper_on_odd_index(char const* str, char** transformed_result) {
    if (!str || !transformed_result) {
        return BAD_POINTER_ERROR;
    }
    int len_of_str = my_strlen(str);

    (*transformed_result) = (char*)malloc(len_of_str * sizeof(char) + 1);
    if (!(*transformed_result)) {
        return MEMORY_ALLOC_ERROR;
    }

    for (int i = 0; i < len_of_str; ++i) {
        if (i % 2 == 0) {
            (*transformed_result)[i] = str[i];
        } else {
            (*transformed_result)[i] = to_upper_char(str[i]);
        }
    }
    (*transformed_result)[len_of_str] = '\0';
    return OK;
}


enum return_code transform_str_to_numbers_letters_order(char const* str, char** transformed_result) {
    if (!str) {
        return BAD_POINTER_ERROR;
    }
    int len_of_str = my_strlen(str);

    char* splitted_str = (char*)malloc(len_of_str * 3 * sizeof(char) + 1);
    if (!splitted_str) {
        return MEMORY_ALLOC_ERROR;
    }

    (*transformed_result) = (char*)malloc(len_of_str * sizeof(char) + 1);
    if (!(*transformed_result)) {
        free(splitted_str);
        return MEMORY_ALLOC_ERROR;
    }
    (*transformed_result)[0] = '\0';

    int end_of_numbers = 0, end_of_letters = len_of_str, end_of_others = 2 * len_of_str;
    char c;
    for (int i = 0; i < len_of_str; ++i) {
        c = str[i];
        if (isdigit(c)) {
            splitted_str[end_of_numbers++] = c;
        } else if (isalpha(c)) {
            splitted_str[end_of_letters++] = c;
        } else {
            splitted_str[end_of_others++] = c;
        }
    }
    splitted_str[end_of_numbers] = '\0';
    splitted_str[end_of_letters] = '\0';
    splitted_str[end_of_others] = '\0';
    for (int i = 0; i < 3; ++i) {
        my_strcat(*transformed_result, splitted_str + i * len_of_str);
    }
    free(splitted_str);
    return OK;
}

enum return_code for_l(char const* str, int* len_result) {
    if (!len_result || !str) {
        return BAD_POINTER_ERROR;
    }
    *len_result = my_strlen(str);
    return OK;
}

enum return_code for_r(char const* str, char** reversed_result) {
    if (!str || !reversed_result) {
        return BAD_POINTER_ERROR;
    }
    int result_of_reversing = reverse_str(str, reversed_result);
    
    return result_of_reversing;
}

enum return_code for_u(char const* str, char** transformed_result) {
    if (!str || !transformed_result) {
        return BAD_POINTER_ERROR;
    }
    int result_of_transforming = transform_to_upper_on_odd_index(str, transformed_result);
    
    return result_of_transforming;
}

enum return_code for_n(char const* str, char** transformed_result) {
    if (!str || !transformed_result) {
        return BAD_POINTER_ERROR;
    }
    int result_of_transforming = transform_str_to_numbers_letters_order(str, transformed_result);
    return result_of_transforming;
}

enum return_code for_c(int argc, char* argv[], int seed, char** cat_result) {
    int amount_of_strings = argc - 4;
    if (amount_of_strings <= 0) {
        return BAD_INPUT_ERROR;
    }   
    int* order_to_concat = (int*)malloc(amount_of_strings * sizeof(int));
    if (!order_to_concat) {
        return MEMORY_ALLOC_ERROR;
    }

    int summary_len_of_all_strings = my_strlen(argv[2]);
    for (int i = 0; i < amount_of_strings; ++i) {
        order_to_concat[i] = i + 4;
        summary_len_of_all_strings += my_strlen(argv[i + 4]);
    }
    shuffle(order_to_concat, seed, amount_of_strings);
    
    char* result = (char*)malloc(summary_len_of_all_strings * sizeof(char) + 1);
    if (!result) {
        free(order_to_concat);
        return MEMORY_ALLOC_ERROR;
    }
    result[0] = '\0';

    my_strcat(result, argv[2]);
    for (int i = 0; i < amount_of_strings; ++i) {
        my_strcat(result, argv[order_to_concat[i]]);
    }
    free(order_to_concat);
    *cat_result = result;
    return OK;
}