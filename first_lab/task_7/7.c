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

enum return_code read_next_symbol_of_file(FILE* input, char* c) {
    if (!input) {
        return FILE_OPEN_ERROR;
    }
    do {
        *c = fgetc(input);
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
        if (*last_char  == EOF) {
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
                if (c  == EOF) {
                    return OK;
                }
            } while (!is_delimiter(c));

        }
        if (current_count % 5 == 0 && current_count % 10 != 0) {
            skip_delimeters_until_symbol(input_file, &c);
            do {
                fprintf(output_file, "%o", c);
                c = fgetc(input_file);
                if (c  == EOF) {
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
                if (c  == EOF) {
                    return OK;
                }
            } while (!is_delimiter(c));
        }
        if (current_count % 10 != 0 && current_count % 2 != 0 && current_count % 5 != 0) {
            skip_delimeters_until_symbol(input_file, &c);
            do {
                fputc(c, output_file);
                c = fgetc(input_file);
                if (c  == EOF) {
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
        if (current_count % 2 == 0) {
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

int main(int argc, char* argv[]) {
    // if (argc < 2) {
    //     printf("Not enough arguments");
    //     return WRONG_ARGUMENTS_ERROR;
    // }

    // if (is_valid_flag(argv[1]) != OK) {
    //     printf("Invalid flag\n");
    //     return BAD_INPUT_ERROR;
    // }
    

    FILE* first_file = fopen("7.txt", "r");
    FILE* second_file = fopen("71.txt", "r");
    FILE* result = fopen("test.txt", "w");
    for_a(first_file, result);
    // while (c != EOF) {
        // write_symbol_from_input_file_to_output_until_delimeter(first_file, result, &c);
    // }

    fclose(first_file);
    fclose(second_file);
    fclose(result);
}