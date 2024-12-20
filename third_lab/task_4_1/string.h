#ifndef STRING_H_
#define STRING_H_

#include "return_code.h"

typedef struct String
{
    int size;
    char *arr;
} String;

String string_create(const char *str);
void delete_string_content(String *str);
int string_compare(const String *str_1, const String *str_2);
int string_equivalence_relation(String *str_1, String *str_2);
enum return_code string_copy(String *dest, String *sourse);
String *string_dup_new_dynamic(String *sourse);
enum return_code strings_concatenation(String *str_1, String *str_2);
String *string_destroy(String *str);

#endif