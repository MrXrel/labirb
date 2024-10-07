#include "functions.h"

#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int is_delimiter(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == EOF;
}

int is_valid_symbol_in_base(char c, int base) {
    if (c == '+' || c == '-') {
        return 1;
    }
    if (base >= 2 && base <= 9) {
        return (c >= '0' && c < ('0' + base));
    }
    // base >= 10
    return isdigit(c) || (c >= 'A' && c < ('A' + base - 10));
}

enum return_code is_equal_to_stop(char const* str, int ind_of_end) {
    if (ind_of_end != 4) {
        return 0;
    }
    return (str[0] == 'S' && str[1] == 't' && str[2] == 'o' && str[3] == 'p');
}

enum return_code skip_delimeters_until_symbol(char* c) {
    if (!c) {
        return BAD_POINTER_ERROR;
    }
    do {
        *c = getc(stdin);
        if (*c == EOF) {
            return OK;
        }
    } while (is_delimiter(*c));
    return OK;
}

enum return_code convert_to_decimal(const char* num_repr_in_base, int end_of_num_repr, int base, long* decimal_result) {
    if (!num_repr_in_base || !decimal_result) {
        return BAD_POINTER_ERROR;
    }
    *decimal_result = 0;
    int value;
    int sign = 1;

    if (end_of_num_repr == 0) {
        return OK;
    }

    for (int i = 0; i < end_of_num_repr; i++) {
        char c = num_repr_in_base[i];

        if (i == 0 && c == '+') {
            continue;
        }
        if (i == 0 && c == '-') {
            sign = -1;
            continue;
        }

        if (!is_valid_symbol_in_base(c, base)) {
            return BAD_INPUT_ERROR;
        }

        if (isdigit(c)) {
            value = c - '0';
        } else if (isalpha(c)) {
            value = c - 'A' + 10;
        } else {
            return BAD_INPUT_ERROR;
        }

        if (*decimal_result > LLONG_MAX / base) {
            return OVERFLOW_ERROR;
        }
        *decimal_result *= base;
        if (value > 0 && (*decimal_result > LLONG_MAX - value)) {
            return OVERFLOW_ERROR;
        }
        *decimal_result += value;
    }
    *decimal_result *= sign;
    return OK;
}

enum return_code convert_decimal_to_base(long x, int base, char* representation, int* ind) {
    if (representation == NULL || ind == NULL) {
        return BAD_POINTER_ERROR;
    }
    if (!(base >= 2 && base <= 36)) {
        return BAD_INPUT_ERROR;
    }

    *ind = 0;
    if (x == 0) {
        representation[0] = '0';
        ++(*ind);
        return OK;
    }

    int is_negative = (x < 0) ? 1 : 0;
    x = labs(x);

    while (x > 0) {
        int digit = x % base;
        if (digit < 10) {
            representation[*ind] = '0' + digit;
        } else {
            representation[*ind] = 'A' + (digit - 10);
        }
        ++(*ind);
        x /= base;
    }
    if (is_negative) {
        representation[*ind] = '-';
        ++(*ind);
    }
    return OK;
}

enum return_code write_reversed_str_to_stdout(char const* reversed_str, int const ind_of_end) {
    if (!reversed_str || !ind_of_end) {
        return BAD_POINTER_ERROR;
    }
    if (ind_of_end < 0) {
        return BAD_INPUT_ERROR;
    }
    for (int i = ind_of_end; i >= 0; --i) {
        putc(reversed_str[i], stdout);
    }
    return OK;
}

enum return_code get_base_from_stdin(int* result, char* last_char) {
    char buff[5];
    int ind = 0;
    skip_delimeters_until_symbol(last_char);
    do {
        buff[ind] = *last_char;
        ++ind;
        // +34 - good
        if (ind >= 3) {
            return BAD_INPUT_ERROR;
        }
        *last_char = getc(stdin);
    } while (!is_delimiter(*last_char));

    long base = 0;
    if (convert_to_decimal(buff, ind, 10, &base) != OK) {
        return BAD_INPUT_ERROR;
    }
    if (!(base >= 2 && base <= 36)) {
        return BAD_INPUT_ERROR;
    }
    *result = (int)base;
    return OK;

}

enum return_code write_in_buffer_str_while_delimeter_from_stdin(char* buffer, int* buff_ind, char* last_char) {
    skip_delimeters_until_symbol(last_char);
    *buff_ind = 0;
    do {
        buffer[*buff_ind] = *last_char;
        ++(*buff_ind);
        if (*buff_ind >= BUFSIZ) {
            return OVERFLOW_ERROR;
        }
        *last_char = getc(stdin);
    } while (!is_delimiter(*last_char));
    return OK;
}

enum return_code print_in_base_to_stdout(long number, int base) {
    char repr[BUFSIZ];
    int ind_of_end = 0;
    convert_decimal_to_base(number, base, repr, &ind_of_end);

    printf("In %d base: ", base);
    write_reversed_str_to_stdout(repr, ind_of_end - 1);
    printf("\n");
    return OK;
}

enum return_code find_biggest_num_in_stdin(long* decimal_result, int* base_of_number) {
    if(!decimal_result || !base_of_number) {
        return BAD_POINTER_ERROR;
    }

    char buff[BUFSIZ];
    int buff_ind = 0;

    long biggest_number;
    int base, is_first_number = 1;

    char last_char;

    int res = get_base_from_stdin(&base, &last_char);
    if (res != OK) {
        return res;
    }

    while (last_char != EOF) {
        int result_of_writting_in_buffer = write_in_buffer_str_while_delimeter_from_stdin(buff, &buff_ind, &last_char);
        if (result_of_writting_in_buffer != OK) {
            return result_of_writting_in_buffer;
        }

        if (is_equal_to_stop(buff, buff_ind)) {
            break;
        }

        long decimal_repr = 0;
        int res = convert_to_decimal(buff, buff_ind, base, &decimal_repr);
        if (res != OK) {
            return res;
        }

        if (is_first_number) {
            is_first_number = 0;
            biggest_number = decimal_repr;
            continue;
        }
        if (labs(biggest_number) < labs(decimal_repr)) {
            biggest_number = decimal_repr;
        }
    }
    if (is_first_number) {
        return NO_NUMBERS_IN_STDIN;
    }
    *decimal_result = biggest_number;
    *base_of_number = base;
    return OK;
}