#include "string.h"

#include "stdio.h"
#include <stdlib.h>
#include <string.h>


String string_create(const char *str)
{
    String instance;
    if (!str)
    {
        instance.size = -1;
        instance.arr = NULL;
        return instance;
    }

    instance.size = strlen(str);
    instance.arr = (char *)malloc((instance.size + 1) * sizeof(char));
    if (!instance.arr)
    {
        instance.size = -1;
        instance.arr = NULL;
        return instance;
    }
    strcpy(instance.arr, str);
    return instance;
}

void delete_string_content(String *str)
{
    free(str->arr);
    str->arr = NULL;
    str->size = 0;
}

int string_compare(const String *str_1, const String *str_2)
{
    if (str_1->size != str_2->size)
        return str_1->size - str_2->size;
    return strcmp(str_1->arr, str_2->arr);
}

int string_equivalence_relation(String *str_1, String *str_2)
{
    return strcmp(str_1->arr, str_2->arr) == 0;
}

enum return_code string_copy(String *dest, String *sourse)
{
    if (!dest || !sourse)
        return MEMORY_ALLOC_ERROR;

    dest->size = sourse->size;
    if (!sourse->arr)
    {
        dest->arr = NULL;
        return OK;
    }
    dest->arr = (char *)malloc((dest->size + 1) * sizeof(char));
    if (!dest->arr)
        return MEMORY_ALLOC_ERROR;
    strcpy(dest->arr, sourse->arr);
    return OK;
}

String *string_dup_new_dynamic(String *sourse)
{
    if (!sourse)
        return NULL;

    String *new_str = (String *)malloc(sizeof(String));
    if (!new_str)
        return NULL;

    if (string_copy(new_str, sourse) != OK)
    {
        free(new_str);
        return NULL;
    }
    return new_str;
}

enum return_code strings_concatenation(String *str_1, String *str_2)
{
    char *temp_realloc;
    if (!str_1 || !str_2 || !str_1->arr || !str_2->arr)
        return MEMORY_ALLOC_ERROR;

    str_1->size += str_2->size;
    temp_realloc = (char *)realloc(str_1->arr, (str_1->size + 1) * sizeof(char));
    if (!temp_realloc)
    {
        str_1->size -= str_2->size;
        return MEMORY_ALLOC_ERROR;
    }
    str_1->arr = temp_realloc;
    strcat(str_1->arr, str_2->arr);
    return OK;
}

String *string_destroy(String *str)
{
    free(str->arr);
    free(str);
    return NULL;
}