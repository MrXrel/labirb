#include "functions.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

enum return_code search_in_files_and_write_result(char const* substring, int num_files, ...) {
    if (!substring) {
        return BAD_POINTER_ERROR;
    }
    if (num_files <= 0) {
        return BAD_INPUT_ERROR;
    }
    va_list files;
    va_start(files, num_files);
    
    int len_of_substing = my_strlen(substring);
    char* cycled_buffer = (char*)malloc(len_of_substing + 1);
    int len_of_cycled_buffer_with_determinator = len_of_substing + 1;

    if (!cycled_buffer) {
        va_end(files);
        return MEMORY_ALLOC_ERROR;
    }

    FILE* output = fopen("result.txt", "w");
    if (!output) {
        free(cycled_buffer);
        va_end(files);
        return FILE_OPEN_ERROR;
    }

    for (int i = 0; i < num_files; i++) {
        const char *file_path = va_arg(files, const char*);
        // to do open_files
        FILE* input = fopen(file_path, "r");
        if (!input) {
            free(cycled_buffer);
            fclose(output);
            va_end(files);
            return FILE_OPEN_ERROR;
        }
        fprintf(output, "%s:\n", file_path);
        char c = fgetc(input);
        
        // filling buffer
        int count = 0;
        while (c != EOF && count < len_of_cycled_buffer_with_determinator - 1) {
            cycled_buffer[count] = c;
            c = fgetc(input);
            ++count;
        }
        if (count != len_of_cycled_buffer_with_determinator - 1) {
            continue;
        }
        cycled_buffer[count] = '\0';
        int buffer_start = 0;

        int string_number = 1, position = 0;
        do {            
            int is_equal = 0;
            strcmp_for_cycled_buffer(cycled_buffer, buffer_start, len_of_cycled_buffer_with_determinator, substring, &is_equal);
            if (is_equal) {
                fprintf(output, "%d %d\n", string_number, position);
            }
            
            c = fgetc(input);
            if (c == EOF) {
                break;
            }
            
            if (cycled_buffer[buffer_start] == '\n') {
                string_number += 1;
                position = 0;
            }
            else {
                position += 1;
            }

            cycled_buffer[buffer_start] = '\0';
            cycled_buffer[(buffer_start + len_of_cycled_buffer_with_determinator - 1) % len_of_cycled_buffer_with_determinator] = c;
            buffer_start = (buffer_start + 1) % len_of_cycled_buffer_with_determinator;

        } while (c != EOF);
        fclose(input);
        fprintf(output ,"\n");
    }
    free(cycled_buffer);
    fclose(output);
    
    va_end(files);
    return OK;
}