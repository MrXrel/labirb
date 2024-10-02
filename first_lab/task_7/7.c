#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

int convert_decimal_to_base(int x, int base, char* representation, int* ind) {
    if (representation == NULL || ind == NULL) {
        return BAD_POINTER_ERROR;
    }
    *ind = 0;
    if (x == 0) {
        representation[0] = '0';
        ++(*ind);
        return OK;
    }

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
    return OK;
}

enum return_code write_reversed_str_to_file(FILE* file, char const* reversed_str, int const ind_of_end) {
    if (!file || !reversed_str || !ind_of_end) {
        return BAD_POINTER_ERROR;
    }
    for (int i = ind_of_end; i >= 0; --i) {
        fputc(reversed_str[i], file);
    }
    return OK;
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

enum return_code write_symbol_from_input_file_to_output_until_delimeter(FILE* input, FILE* output, char* last_char) {
    if (!input || !output) {
        return FILE_OPEN_ERROR;
    }
    if (!last_char) {
        return BAD_POINTER_ERROR;
    }

    skip_delimeters_until_symbol(input, last_char);
    do {
        fputc(*last_char, output);
        *last_char = fgetc(input);
        if (*last_char == EOF) {
            return OK;
        }
    } while (!is_delimiter(*last_char));
    return OK;
}

enum return_code for_a(FILE* input_file, FILE* output_file) {
    if (!input_file || !output_file) {
        return FILE_OPEN_ERROR;
    }
    // numeration again 0 or 1????
    int current_count = 1;
    char c = ' ';
    char base_repr[50];
    int ind = 0;

    while (c != EOF) {
        if (current_count % 10 == 0) {
            skip_delimeters_until_symbol(input_file, &c);
            do {
                ind = 0;
                convert_decimal_to_base(c, 4, base_repr, &ind);
                write_reversed_str_to_file(output_file, base_repr, ind - 1);
                c = fgetc(input_file);
                if (c == EOF) {
                    return OK;
                }
            } while (!is_delimiter(c));
        }
        if (current_count % 5 == 0 && current_count % 10 != 0) {
            skip_delimeters_until_symbol(input_file, &c);
            do {
                fprintf(output_file, "%o", c);
                c = fgetc(input_file);
                if (c == EOF) {
                    return OK;
                }
            } while (!is_delimiter(c));
        }
        if (current_count % 2 == 0 && current_count % 10 != 0) {
            skip_delimeters_until_symbol(input_file, &c);
            do {
                if (c >= 'A' && c <= 'Z') {
                    c += 32;
                }
                fputc(c, output_file);
                c = fgetc(input_file);
                if (c == EOF) {
                    return OK;
                }
            } while (!is_delimiter(c));
        }
        if (current_count % 10 != 0 && current_count % 2 != 0 && current_count % 5 != 0) {
            skip_delimeters_until_symbol(input_file, &c);
            do {
                fputc(c, output_file);
                c = fgetc(input_file);
                if (c == EOF) {
                    return OK;
                }
            } while (!is_delimiter(c));
        }

        fputc(' ', output_file);
        ++current_count;
        current_count %= 10;
    }
    return OK;
}

enum return_code for_r(FILE* input_file1, FILE* input_file2, FILE* output_file) {
    if (!input_file1 || !input_file2 || !output_file) {
        return FILE_OPEN_ERROR;
    }
    // numeration starts from 0 or 1?
    int current_count = 1;
    char first_file_char = ' ', second_file_char = ' ';

    while (first_file_char != EOF && second_file_char != EOF) {
        if (current_count % 2 == 1) {
            write_symbol_from_input_file_to_output_until_delimeter(input_file1, output_file, &first_file_char);
        } else {
            write_symbol_from_input_file_to_output_until_delimeter(input_file2, output_file, &second_file_char);
        }
        fputc(' ', output_file);
        ++current_count;
        current_count %= 2;
    }

    while (first_file_char != EOF) {
        write_symbol_from_input_file_to_output_until_delimeter(input_file1, output_file, &first_file_char);
        fputc(' ', output_file);
    }

    while (second_file_char != EOF) {
        write_symbol_from_input_file_to_output_until_delimeter(input_file2, output_file, &second_file_char);
        fputc(' ', output_file);
    }
    return OK;
}

enum return_code is_valid_flag(char const* flag) {
    if (strlen(flag) != 2 || !(flag[0] == '-' || flag[0] == '/')) {
        return BAD_INPUT_ERROR;
    }
    switch (flag[1]) {
        case 'r':
        case 'a':
            return OK;
        default:
            return BAD_INPUT_ERROR;
    }
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

int is_equal_path(char const* first_path, char const* second_path) {
    return (strcmp(first_path, second_path) == 0);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Not enough arguments");
        return WRONG_ARGUMENTS_ERROR;
    }

    if (is_valid_flag(argv[1]) != OK) {
        printf("Invalid flag\n");
        return BAD_INPUT_ERROR;
    }
    char flag = argv[1][1];

    char path_of_current_directory[FILENAME_MAX];
    memset(path_of_current_directory, '\0', FILENAME_MAX);

    char path_of_input_file_1[FILENAME_MAX];
    memset(path_of_input_file_1, '\0', FILENAME_MAX);
    char path_of_input_file_2[FILENAME_MAX];
    memset(path_of_input_file_2, '\0', FILENAME_MAX);

    char path_of_output_file[FILENAME_MAX];
    memset(path_of_output_file, '\0', FILENAME_MAX);

    FILE* first_input_file;
    FILE* second_input_file;
    FILE* output_file;

    int result_of_parsing_path_to_file;
    switch (flag) {
        case 'r':
            if (argc != 5) {
                printf("Wrong amount of arguments\n");
                return WRONG_ARGUMENTS_ERROR;
            }

            // current directory
            result_of_parsing_path_to_file = get_path_of_current_directory(path_of_current_directory);
            if (result_of_parsing_path_to_file != OK) {
                print_file_errors(result_of_parsing_path_to_file);
                return result_of_parsing_path_to_file;
            }

            // input 1 file
            result_of_parsing_path_to_file = modify_path_to_absolute(path_of_current_directory, argv[2], path_of_input_file_1);
            if (result_of_parsing_path_to_file != OK) {
                print_file_errors(result_of_parsing_path_to_file);
                return result_of_parsing_path_to_file;
            }
            // input 2 file
            result_of_parsing_path_to_file = modify_path_to_absolute(path_of_current_directory, argv[3], path_of_input_file_2);
            if (result_of_parsing_path_to_file != OK) {
                print_file_errors(result_of_parsing_path_to_file);
                return result_of_parsing_path_to_file;
            }
            if (is_equal_path(path_of_input_file_1, path_of_input_file_2)) {
                printf("Same file\n");
                return BAD_INPUT_ERROR;
            }
            // output file
            result_of_parsing_path_to_file = modify_path_to_absolute(path_of_current_directory, argv[4], path_of_output_file);
            if (result_of_parsing_path_to_file != OK) {
                print_file_errors(result_of_parsing_path_to_file);
                return result_of_parsing_path_to_file;
            }

            if (is_equal_path(path_of_input_file_1, path_of_output_file) ||
                is_equal_path(path_of_input_file_2, path_of_output_file)) {
                printf("Same file\n");
                return BAD_INPUT_ERROR;
            }

            first_input_file = fopen(path_of_input_file_1, "r");
            if (!first_input_file) {
                printf("File open error: %s\n", path_of_input_file_1);
                return FILE_OPEN_ERROR;
            }
            second_input_file = fopen(path_of_input_file_2, "r");
            if (!second_input_file) {
                printf("File open error: %s\n", path_of_input_file_2);
                fclose(first_input_file);
                return FILE_OPEN_ERROR;
            }
            output_file = fopen(path_of_output_file, "w");
            if (!output_file) {
                printf("File open error: %s\n", path_of_output_file);
                fclose(first_input_file);
                fclose(second_input_file);
                return FILE_OPEN_ERROR;
            }

            for_r(first_input_file, second_input_file, output_file);
            break;

        case 'a':
            if (argc != 4) {
                printf("Wrong amount of arguments\n");
                return WRONG_ARGUMENTS_ERROR;
            }

            // current directory
            result_of_parsing_path_to_file = get_path_of_current_directory(path_of_current_directory);
            if (result_of_parsing_path_to_file != OK) {
                print_file_errors(result_of_parsing_path_to_file);
                return result_of_parsing_path_to_file;
            }

            // input file
            result_of_parsing_path_to_file = modify_path_to_absolute(path_of_current_directory, argv[2], path_of_input_file_1);
            if (result_of_parsing_path_to_file != OK) {
                print_file_errors(result_of_parsing_path_to_file);
                return result_of_parsing_path_to_file;
            }

            // output file
            result_of_parsing_path_to_file = modify_path_to_absolute(path_of_current_directory, argv[3], path_of_output_file);
            if (result_of_parsing_path_to_file != OK) {
                print_file_errors(result_of_parsing_path_to_file);
                return result_of_parsing_path_to_file;
            }

            if (is_equal_path(path_of_input_file_1, path_of_output_file)) {
                printf("Same file\n");
                return BAD_INPUT_ERROR;
            }

            first_input_file = fopen(path_of_input_file_1, "r");
            if (!first_input_file) {
                printf("File open error: %s\n", path_of_input_file_1);
                return FILE_OPEN_ERROR;
            }

            output_file = fopen(path_of_output_file, "w");
            if (!output_file) {
                printf("File open error: %s\n", path_of_output_file);
                fclose(first_input_file);
                return FILE_OPEN_ERROR;
            }

            for_a(first_input_file, output_file);
            break;
        default:
            break;
    }

    fclose(output_file);
    fclose(first_input_file);
    if (flag == 'r') {
        fclose(second_input_file);
    }
}