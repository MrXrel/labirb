#include "string.h"

#include <stdlib.h>
#include <string.h>

enum return_code create_string_object(char const* value_of_string, String* to_create) {
    if (!value_of_string || !to_create) {
        BAD_POINTER_ERROR;
    }
    to_create->buffer = NULL;
    to_create->length = 0;

    int len_to_create = strlen(value_of_string);
    to_create->buffer = (char*)malloc(len_to_create + 1);
    if (!to_create->buffer) {
        return MEMORY_ALLOC_ERROR;
    }

    memcpy(to_create->buffer, value_of_string, len_to_create + 1);
    to_create->length = len_to_create;
    return OK;
}

enum return_code clear_string(String* to_clear) {
    if (!to_clear || !to_clear->buffer) {
        return BAD_POINTER_ERROR;
    }
    to_clear->buffer[0] = '\0';
    to_clear->length = 0;
    return OK;
}

enum return_code is_equal_strings(String const* str1, String const* str2, int* is_equal) {
    if (!str1 || !str2 || !is_equal || !str1->buffer || !str2->buffer) {
        return BAD_POINTER_ERROR;
    }

    *is_equal = 1;
    if (str1->length != str2->length) {
        *is_equal = 0;
        return OK;
    }
    char* buff1 = str1->buffer;
    char* buff2 = str2->buffer;

    while (*buff1 != '\0') {
        if (*buff1 != *buff2) {
            *is_equal = 0;
            return OK;
        }
        ++buff1;
        ++buff2;
    }
    return OK;
}

enum return_code copy_to_exist_string(String* dest, String const* src) {
    if (!dest || !src || !dest->buffer || !src->buffer) {
        return BAD_POINTER_ERROR;
    }
    int len_of_dest = dest->length;
    int len_of_src = src->length;
    if (len_of_dest >= len_of_src) {
        memcpy(dest->buffer, src->buffer, len_of_src + 1);
        return OK;
    }
    char* new_buffer = (char*)realloc(dest->buffer, len_of_src + 1);
    if (!new_buffer) {
        return MEMORY_ALLOC_ERROR;
    }
    dest->buffer = new_buffer;
    dest->length = len_of_src;
    memcpy(dest->buffer, src->buffer, len_of_src + 1);
    return OK;
}

enum return_code copy_to_new_string(String** dest_to_create, String const* src) {
    if (!src || !dest_to_create) {
        return BAD_POINTER_ERROR;
    }
    int len_of_src = src->length;
    (*dest_to_create) = (String*)malloc(sizeof(String));
    if (!(*dest_to_create)) {
        return MEMORY_ALLOC_ERROR;
    }
    (*dest_to_create)->buffer = (char*)malloc(len_of_src + 1);
    if (!(*dest_to_create)->buffer) {
        free(*dest_to_create);
        return MEMORY_ALLOC_ERROR;
    }
    memcpy((*dest_to_create)->buffer, src->buffer, len_of_src + 1);
    return OK;
}

enum return_code concat_strings(String* dest, String const* src) {
    if (!dest || !src || !dest->buffer || !src->buffer) {
        return BAD_POINTER_ERROR;
    }
    int new_len = dest->length + src->length;

    char* new_buffer = (char*)realloc(dest->buffer, new_len + 1);
    if (!new_buffer) {
        return MEMORY_ALLOC_ERROR;
    }
    strcat(new_buffer, src->buffer);
    new_buffer[new_len] = '\0';
    dest->buffer = new_buffer;
    return OK;
}

enum return_code destroy_string(String* str) {
    if (!str) {
        return BAD_POINTER_ERROR;
    } 
    free(str->buffer);
    str->buffer = NULL;
    str->length = 0;
    return OK;
}