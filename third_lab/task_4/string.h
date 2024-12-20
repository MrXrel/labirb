#ifndef STRING_H
#define STRING_H

#include "return_code.h"

typedef struct String {
    char* buffer;
    int length;
} String;

enum return_code create_string_object(char const* value_of_string, String* to_create);
enum return_code clear_string(String* to_clear);
enum return_code is_equal_strings(String const* str1, String const* str2, int* is_equal);
enum return_code copy_to_exist_string(String* dest, String const* src);
enum return_code copy_to_new_string(String** dest_to_create, String const* src);
enum return_code concat_strings(String* dest, String const* src);
enum return_code destroy_string(String* str);

#endif