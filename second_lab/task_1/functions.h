#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "return_code.h"
#include "string.h"

void print_error_message(int return_code);
int transfer_to_int(const char* str, int* result);

enum return_code transform_to_upper_on_odd_index(char const* str, char** transformed_result);
enum return_code transform_str_to_numbers_letters_order(char const* str, char** transformed_result);

enum return_code is_valid_flag(char const* flag);
enum return_code shuffle(int* array_of_nums, int seed, int size_of_array);

enum return_code for_l(char const* str, int* len_result);
enum return_code for_r(char const* str, char** reversed_result);
enum return_code for_u(char const* str, char** transformed_result);
enum return_code for_n(char const* str, char** transformed_result);
enum return_code for_c(int argc, char* argv[], int seed, char** cat_result);


#endif