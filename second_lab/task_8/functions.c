#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "functions.h"

int is_valid_symbol_in_base(char const c, int base) {
    if (base >= 2 && base <= 9) {
        return (c >= '0' && c < ('0' + base));
    }
    // base >= 10
    return isdigit(c) || (c >= 'A' && c < ('A' + base - 10));
}

int char_to_number(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    c = toupper(c);
    return c - 'A' + 10;
}

char number_to_char(int c) {
    if (c >= 0 && c <= 9) {
        return '0' + c;
    }
    return 'A' + c - 10;
}

int is_valid_base(int base) {
    return base >= 2 && base <= 36;
}

enum return_code write_reversed_to_buffer(char* buffer, int* buffer_ind, int base, char const* start_of_number, char const* end_of_number) {
    if (!buffer || !buffer_ind || !start_of_number || !end_of_number) {
        return BAD_POINTER_ERROR;
    }
    if (!is_valid_base(base)) {
        return BAD_INPUT_ERROR;
    }

    if (*start_of_number == '\0') {
        buffer[0] = '0';
        buffer[1] = '\0';
        return OK;
    }

    do {
        if (!is_valid_symbol_in_base(*end_of_number, base)) {
            return BAD_INPUT_ERROR;
        }
        buffer[*buffer_ind] = *end_of_number;
        --end_of_number;
        ++(*buffer_ind);
        if (*buffer_ind > LENGTH_OF_NUMBER) {
            return OVERFLOW_ERROR;
        }
    } while (end_of_number >= start_of_number);
    return OK;
}

enum return_code sum_reversed_numbers(int base, char* num1, char const* num2) {
    if (!num1 || !num2) {
        return BAD_POINTER_ERROR;
    }
    if (!is_valid_base(base)) {
        return BAD_INPUT_ERROR;
    }
    char* result = num1;
    int biggest_num_in_base = base - 1;
    int to_add = 0;
    int ind = 0;

    while (*num1 != '\0' && *num2 != '\0') {
        if (!is_valid_symbol_in_base(*num1, base) || !is_valid_symbol_in_base(*num2, base)) {
            return BAD_INPUT_ERROR;
        }
        int first = char_to_number(*num1);
        int second = char_to_number(*num2);
        // sum to nums with to_add
        int res = first + second + to_add;
        to_add = 0;

        if (res > biggest_num_in_base) {
            to_add = 1;
            res -= base;
        }
        result[ind] = number_to_char(res);
        ++ind;
        if (ind > LENGTH_OF_NUMBER) {
            return OVERFLOW_ERROR;
        }
        ++num1;
        ++num2;
    }

    while (*num1 != '\0') {
        if (!is_valid_symbol_in_base(*num1, base)) {
            return BAD_INPUT_ERROR;
        }
        int res = char_to_number(*num1) + to_add;
        to_add = 0;
        if (res > biggest_num_in_base) {
            to_add = 1;
            res -= base;
        }
        result[ind] = number_to_char(res);

        ++ind;
        if (ind > LENGTH_OF_NUMBER) {
            return OVERFLOW_ERROR;
        }
        ++num1;
    }

    while (*num2 != '\0') {
        if (!is_valid_symbol_in_base(*num2, base)) {
            return BAD_INPUT_ERROR;
        }
        int res = char_to_number(*num2) + to_add;
        to_add = 0;
        if (res > biggest_num_in_base) {
            to_add = 1;
            res -= base;
        }
        result[ind] = number_to_char(res);
        ++ind;
        if (ind > LENGTH_OF_NUMBER) {
            return OVERFLOW_ERROR;
        }
        ++num2;
    }
    
    if (to_add != 0) {
        result[ind] = '1';
        ++ind;
        if (ind > LENGTH_OF_NUMBER) {
            return OVERFLOW_ERROR;
        }
        
    }
    result[ind] = '\0';
    return OK;
}

enum return_code reverse_str(char const* str, char** reversed_result) {
    if (!str) {
        return BAD_POINTER_ERROR;
    }
    int len_of_str = strlen(str);

    (*reversed_result) = (char*)malloc(len_of_str * sizeof(char) + 1);
    if (!(*reversed_result)) {
        return MEMORY_ALLOC_ERROR;
    }

    for (int i = len_of_str - 1; i >= 0; --i) {
        (*reversed_result)[len_of_str - i - 1] = str[i];
    }
    (*reversed_result)[len_of_str] = '\0';
    return OK;
}

enum return_code skip_zeros_signs(char const** start) {
    if (!start) {
        return BAD_POINTER_ERROR;
    }
    int has_plus = 0;
    int has_digits = 0;
    while ((*(*start) == '0' || **start == '+') && *(*start) != '\0') {
        if (**start == '+' && has_plus) {
            return BAD_INPUT_ERROR;
        }
        if (**start == '+' && has_digits) {
            return BAD_INPUT_ERROR;
        }
    
        if (**start == '+') {
            has_plus = 1;
            ++(*start);
            continue;
        }
        has_digits = 1;
        ++(*start);
    }
    return OK;
}

enum return_code calculate_sum(char** sum_result, int base, int count, ...) {
    if (!sum_result) {
        return BAD_POINTER_ERROR;
    }
    if (!is_valid_base(base) || count <= 0) {
        return BAD_INPUT_ERROR;
    }
    char* result = (char*)malloc(LENGTH_OF_NUMBER * sizeof(char) + 1);
    if (!result) {
        return MEMORY_ALLOC_ERROR;
    }
    result[0] = '0';
    result[1] = '\0';

    char* buffer = (char*)malloc(LENGTH_OF_NUMBER * sizeof(char) + 1);
    if (!buffer) {
        free(result);
        return MEMORY_ALLOC_ERROR;
    }
    int buffer_ind = 0;

    va_list args;
    va_start(args, count);

    do {
        char const* start_num = va_arg(args, char*);
        int len_of_num = strlen(start_num);
        char const* end_of_num = start_num + len_of_num - 1;

        int res_of_skip = skip_zeros_signs(&start_num);
        if (res_of_skip != OK) {
            free(buffer);
            free(result);
            va_end(args);
            return res_of_skip;
        }

        int result_of_write = write_reversed_to_buffer(buffer, &buffer_ind, base, start_num, end_of_num);
        if (result_of_write != OK) {
            free(buffer);
            free(result);
            va_end(args);
            return result_of_write;
        }
        
        buffer[buffer_ind] = '\0';
        
        int result_of_sum = sum_reversed_numbers(base, result, buffer);
        if (result_of_sum != OK) {
            free(buffer);
            free(result);
            va_end(args);
            return result_of_sum;
        }

        buffer_ind = 0;
        --count;
    } while (count > 0);

    int result_of_reverse = reverse_str(result, sum_result);

    va_end(args);
    free(buffer);
    free(result);
    return result_of_reverse;
}
