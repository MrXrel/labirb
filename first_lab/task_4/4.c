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


enum return_code for_d(FILE* read_file, FILE* write_file) {
    if (!read_file || !write_file) {
        return FILE_OPEN_ERROR;
    }
    char c;
    while ((c = fgetc(read_file)) != EOF) {
        if (isdigit(c)) {
            continue;
        }
        fputc(c, write_file);
    }
    return OK;
}

enum return_code for_i(FILE* read_file, FILE* write_file) {
    if (!read_file || !write_file) {
        return FILE_OPEN_ERROR;
    }
    char c;
    long count_letters = 0;
    while (!feof(read_file)) {
        c = fgetc(read_file);
        if (isalpha(c)) {
            ++count_letters;
        }
        if (c == '\n' || c == EOF) {
            fprintf(write_file, "%ld\n", count_letters);
            count_letters = 0;
        }
    }
    return OK;
}

enum return_code for_s(FILE* read_file, FILE* write_file) {
    if (!read_file || !write_file) {
        return FILE_OPEN_ERROR;
    }
    char c;
    long count_letters = 0;
    while (!feof(read_file)) {
        c = fgetc(read_file);
        // Should we count \n?
        if (!isalpha(c) && !isdigit(c) && c != ' ') {
            ++count_letters;
        }
        if (c == '\n' || c == EOF) {
            fprintf(write_file, "%ld\n", count_letters);
            count_letters = 0;
        }
    }
    return OK;
}

enum return_code for_a(FILE* read_file, FILE* write_file) {
    if (!read_file || !write_file) {
        return FILE_OPEN_ERROR;
    }
    char c;
    while (!feof(read_file)) {
        c = fgetc(read_file);
        if (isdigit(c)) {
            fputc(c, write_file);
            continue;
        }
        fprintf(write_file, "%X", c);
        if (c == '\n') {
            fputc(c, write_file);
        }
    }
    return OK;
}



enum return_code is_valid_flag(char const* flag) {
    if (strlen(flag) != 2 || !(flag[0] == '-' || flag[0] == '/')) {
        return BAD_INPUT_ERROR;
    }
    switch (flag[1]) {
        case 'd':
        case 'i':
        case 's':
        case 'a':
            return OK;
        default:
            return BAD_INPUT_ERROR;
    }
}

enum return_code is_valid_nflag(char const* flag) {
    if (strlen(flag) != 3 || !(flag[0] == '-' || flag[0] == '/') || flag[1] != 'n') {
        return BAD_INPUT_ERROR;
    }
    switch (flag[2]) {
        case 'd':
        case 'i':
        case 's':
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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Not enough arguments");
        return WRONG_ARGUMENTS_ERROR;
    }

    int is_good_nflag = is_valid_nflag(argv[1]), is_good_flag = is_valid_flag(argv[1]);
    if (is_good_flag != OK && is_good_nflag != OK) {
        printf("Invalid flag\n");
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

    char flag;
    // output: .../out_{input_name}.txt
    int result_of_input_file = 0, result_of_output_file = 0;
    if (is_good_flag == OK) {
        if (argc != 3) {
            printf("Wrong amount of arguments\n");
            return WRONG_ARGUMENTS_ERROR;
        }
        flag = argv[1][1];
        result_of_input_file = modify_path_to_absolute(path_of_current_directory, argv[2], path_of_input_file);
        if (result_of_input_file != OK) {
            print_file_errors(result_of_input_file);
            return result_of_input_file;
        }
        result_of_output_file = get_output_filename(path_of_input_file, path_of_output_file);
        if (result_of_output_file != OK) {
            print_file_errors(result_of_output_file);
            return result_of_output_file;
        }
    }
    if (is_good_nflag == OK) {
        if (argc != 4) {
            printf("Wrong amount of arguments\n");
            return WRONG_ARGUMENTS_ERROR;
        }
        flag = argv[1][2];
        result_of_input_file = modify_path_to_absolute(path_of_current_directory, argv[2], path_of_input_file);
        if (result_of_input_file != OK) {
            print_file_errors(result_of_input_file);
            return result_of_input_file;
        }
        result_of_output_file = modify_path_to_absolute(path_of_current_directory, argv[3], path_of_output_file);
        if (result_of_output_file != OK) {
            print_file_errors(result_of_output_file);
            return result_of_output_file;
        }
        if (strcmp(path_of_input_file, path_of_output_file) == 0) {
            printf("Same files error\n");
            return BAD_INPUT_ERROR;
        }
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
    
    
    switch (flag) {
        case 'd':
            for_d(input, output);
            break;
        case 'i':
            for_i(input, output);
            break;
        case 's':
            for_s(input, output);
            break;
        case 'a':
            for_a(input, output);
            break;
    }
    fclose(input);
    fclose(output);
    return OK;
}