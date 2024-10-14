#include "string.h"

#include <ctype.h>
#include <stdlib.h>

int my_strlen(char const* str) {
    if (!str) {
        return -1;
    }
    char const* last_el = str;
    while (*last_el != '\0') {
        ++last_el;
    }
    return last_el - str;
}

int my_strcmp(char const* str1, char const* str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return *str1 - *str2;
        }
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

enum return_code strcmp_for_cycled_buffer(char* cycled_buffer, int start_of_cycle_buffer, int len_of_cycle_buffer_with_determinator, char const* substr, int* is_equal) {
    /*cycled_buffer and substr should be same size*/
    if (!cycled_buffer || !substr || !is_equal) {
        return BAD_POINTER_ERROR;
    }
    if (start_of_cycle_buffer < 0 || len_of_cycle_buffer_with_determinator <= 0) {
        return BAD_INPUT_ERROR;
    }
    if (len_of_cycle_buffer_with_determinator - 1 != my_strlen(substr)) {
        return BAD_INPUT_ERROR;
    }
    *is_equal = 0;
    int pos_of_cb = start_of_cycle_buffer;
    while (*substr != '\0') {
        if (cycled_buffer[pos_of_cb] != *substr) {
            *is_equal = 0;
            return OK;
        }
        pos_of_cb = (pos_of_cb + 1) % len_of_cycle_buffer_with_determinator;
        ++substr;
    } 
    *is_equal = 1;
    return OK;
}

enum return_code find_substring(const char *str, const char *substr, int start, int* result_of_search) {
    if (!str || !substr || !result_of_search) {
        return BAD_POINTER_ERROR;
    }
    if (start >= my_strlen(str)) {
        return BAD_INPUT_ERROR;
    }

    int i = start, j = 0;
    while (str[i] != '\0') {
        if (str[i] == substr[j]) {
            int k = i;
            while (str[k] != '\0' && substr[j] != '\0' && str[k] == substr[j]) {
                k++;
                j++;
            }
            if (substr[j] == '\0') {
                *result_of_search = i;
                return OK;
            }
            j = 0;
        }
        i++;
    }
    *result_of_search = -1;
    return OK;
} 

enum return_code create_str(int len_of_str, char** pointer_to_memory) {
    *pointer_to_memory = (char*)malloc(len_of_str * sizeof(char) + 1);
    if (!(*pointer_to_memory)) {
        return MEMORY_ALLOC_ERROR;
    }
    (*pointer_to_memory)[len_of_str] = '\0';
    return OK;
}

enum return_code reverse_str(char const* str, char** reversed_result) {
    if (!str) {
        return BAD_POINTER_ERROR;
    }
    int len_of_str = my_strlen(str);

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

enum return_code my_strcat(char* dest, char const* src) {
    if (!dest || !src) {
        return BAD_POINTER_ERROR;
    }
    char* d = dest + my_strlen(dest);
    char const* s = src;
    while (*s != '\0') {
        *d = *s;
        ++d;
        ++s;
    }
    *d = '\0';
    return OK;
}

char to_upper_char(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - ('a' - 'A');
    }
    return c;
}
