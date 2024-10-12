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
