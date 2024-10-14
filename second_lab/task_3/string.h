#ifndef STRING_H
#define STRING_H

#include "return_code.h"
int my_strlen(char const* str);
int my_strcmp(char const* str1, char const* str2);

char to_upper_char(char c);

enum return_code strcmp_for_cycled_buffer(char* cycled_buffer, int start_of_cycle_buffer, int len_of_cycle_buffer_with_determinator, char const* substr, int* is_equal);
enum return_code find_substring(const char* str, const char* substr, int start, int* result_of_search);

enum return_code create_str(int len_of_str, char** pointer_to_memory);

enum return_code reverse_str(char const* str, char** reversed_result);
enum return_code my_strcat(char* dest, char const* src) ;

#endif