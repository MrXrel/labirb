#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "files.h"

enum return_code {
    OK = 0,
    BAD_INPUT_ERROR = 1,
    OVERFLOW_ERROR = 2,
    BAD_POINTER_ERROR = 3,
    MEMORY_ALLOC_ERROR = 4,
    WRONG_ARGUMENTS_ERROR = 5,
    FILE_OPEN_ERROR = 6,
};

int is_delimiter(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == EOF;
}

enum return_code skip_delimeters_until_symbol(FILE* input_file, char* c) {
    do {
        *c = fgetc(input_file);
        if (*c == EOF) {
            return OK;
        }
    } while (is_delimiter(*c));
    return OK;
}

enum return_code convert_to_decimal(const char* num_repr_in_base, int end_of_num_repr, int base, long* decimal_result) {
    *decimal_result = 0;
    int value;
    int sign = 1;

    if (end_of_num_repr == 0) {
        return OK;
    }

    for (size_t i = 0; i < end_of_num_repr; i++) {
        char c = toupper(num_repr_in_base[i]);
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

enum return_code find_min_base_for_numbers(FILE* input_file, FILE* output_file) {
    if (!input_file || !output_file) {
        return FILE_OPEN_ERROR;
    }
    char c = ' ';
    int base_of_number = 2;
    char biggest_char_in_number = '1';
    int has_digits_before_not_zero = 0;
    int has_digits_before = 0;

    char number_str[BUFSIZ];
    int index_end_of_number_str;
    do {
        skip_delimeters_until_symbol(input_file, &c);
        base_of_number = 2;
        biggest_char_in_number = '1';
        has_digits_before_not_zero = 0;
        has_digits_before = 0;
        index_end_of_number_str = 0;
        do {
            if (c == '+' && !has_digits_before) {
                c = fgetc(input_file);
                continue;
            }
            // convert to upper
            if (c >= 'a' && c <= 'z') {
                c -= 32;
            }

            if (c > biggest_char_in_number) {
                if (c >= 'A') {
                    base_of_number = c - 'A' + 10 + 1;
                } else {
                    base_of_number = c - '0' + 1;
                }
                biggest_char_in_number = c;
            }
            // avoid situation -1a-2
            if ((c == '-' || c == '+') && has_digits_before) {
                return BAD_INPUT_ERROR;
            }

            // skipping all leading zeros
            if (c != '0' || has_digits_before_not_zero) {
                fputc(c, output_file);

                number_str[index_end_of_number_str] = c;
                ++index_end_of_number_str;
                if (c != '-') {
                    has_digits_before_not_zero = 1;
                }
            }

            if (c == '0') {
                has_digits_before = 1;
            }

            c = fgetc(input_file);
        } while (!is_delimiter(c));
        // means number was containing only zeroes -0000
        if (!has_digits_before_not_zero) {
            fputc('0', output_file);
        }
        // make decimal repr
        long decimal_repr = 0;
        int result_of_convertion = convert_to_decimal(number_str, index_end_of_number_str, base_of_number, &decimal_repr);
        if (result_of_convertion != OK) {
            return result_of_convertion;
        }

        fprintf(output_file, " %d %ld\n", base_of_number, decimal_repr);
    } while (c != EOF);
    return OK;
}

enum return_code print_file_errors(int error_num) {
    switch (error_num) {
        case BAD_POINTER_ERROR:
            printf("One of the pointer is NULL\n");
            return BAD_POINTER_ERROR;
        case FILE_PATH_IS_TOO_LONG_ERROR:
            printf("The path of directory is too long\n");
            return FILE_PATH_IS_TOO_LONG_ERROR;
        case OK:
            return OK;
        default:
            return OK;
    }
    return OK;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong amount of arguments");
        return WRONG_ARGUMENTS_ERROR;
    }

    char path_of_current_directory[FILENAME_MAX];
    memset(path_of_current_directory, '\0', FILENAME_MAX);
    char path_of_input_file[FILENAME_MAX];
    memset(path_of_input_file, '\0', FILENAME_MAX);
    char path_of_output_file[FILENAME_MAX];
    memset(path_of_output_file, '\0', FILENAME_MAX);

    int result_of_getting_current_directory = get_path_of_current_directory(path_of_current_directory);
    if (result_of_getting_current_directory != OK) {
        print_file_errors(result_of_getting_current_directory);
        return result_of_getting_current_directory;
    }

    int result_of_input_file, result_of_output_file;
    result_of_input_file = modify_path_to_absolute(path_of_current_directory, argv[1], path_of_input_file);
    if (result_of_input_file != OK) {
        print_file_errors(result_of_input_file);
        return result_of_input_file;
    }
    result_of_output_file = modify_path_to_absolute(path_of_current_directory, argv[2], path_of_output_file);
    if (result_of_output_file != OK) {
        print_file_errors(result_of_output_file);
        return result_of_output_file;
    }

    FILE* input;
    FILE* output;

    input = fopen(path_of_input_file, "r");
    if (!input) {
        printf("Error in opening file: %s\n", path_of_input_file);
        return FILE_OPEN_ERROR;
    }
    output = fopen(path_of_output_file, "w");
    if (!output) {
        printf("Error in opening file: %s\n", path_of_input_file);
        fclose(input);
        return FILE_OPEN_ERROR;
    }

    int result_of_function = find_min_base_for_numbers(input, output);

    switch (result_of_function) {
        case OVERFLOW_ERROR:
            printf("Overflow detected. One of the numbers is too big\n");
            break;
        case BAD_INPUT_ERROR:
            printf("Invalid number in file\n");
            break;
        default:
            break;
    }
    fclose(input);
    fclose(output);
    return result_of_function;
}