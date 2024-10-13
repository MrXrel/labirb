#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "return_code.h"

#define INITIAL_LENGTH_OF_NUMBER 100

enum return_code skip_zeros_signs(char const** start);
enum return_code reverse_str(char const* str, char** reversed_result);
enum return_code write_reversed_to_buffer(char** buffer, int* buffer_ind, int base, char const* start_of_number, char const* end_of_number, int* buffer_capacity);

int is_valid_base(int base);
int is_valid_symbol_in_base(char const c, int base);

char number_to_char(int c);
int char_to_number(char c);

enum return_code calculate_sum(char** sum_result, int base, int count, ...);
enum return_code sum_reversed_numbers(int base, char** num1, char const* num2, int* num1_capacity);

#endif