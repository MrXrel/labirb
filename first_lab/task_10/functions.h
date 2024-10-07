#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "return_code.h"

enum return_code find_biggest_num_in_stdin(long* decimal_result, int* base_of_number);

enum return_code print_in_base_to_stdout(long number, int base);
enum return_code print_reversed_str_to_stdout(char const* reversed_str, int const ind_of_end);
enum return_code print_in_base_to_stdout(long number, int base);

int is_delimiter(char c);
int is_valid_symbol_in_base(char c, int base);
enum return_code is_equal_to_stop(char const* str, int ind_of_end);

enum return_code get_base_from_stdin(int* result, char* last_char);
enum return_code skip_delimeters_until_symbol(char* c);
enum return_code write_in_buffer_str_while_delimeter_from_stdin(char* buffer, int* buff_ind, char* last_char);


enum return_code convert_to_decimal(const char* num_repr_in_base, int end_of_num_repr, int base, long* decimal_result);
enum return_code convert_decimal_to_base(long x, int base, char* representation, int* ind);

#endif