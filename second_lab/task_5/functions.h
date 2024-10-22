#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "return_code.h"
#include <stdio.h>


#define INITIAL_SIZE_OF_FIB 10

typedef void (*printer) (void* stream, TYPE_OF_PRINT type, const char* format, ...);
typedef int (*format_for_char) (int);

void print_universal(void* stream, TYPE_OF_PRINT type, const char* format, ...);

enum return_code print_int_to_roman(void* stream, TYPE_OF_PRINT type_of_print, int num);
enum return_code print_zeckendorf(void* stream, TYPE_OF_PRINT type_of_print, unsigned int num);
enum return_code print_number_in_base(void* stream, TYPE_OF_PRINT type_of_print, format_for_char format_char, int number, int base);
enum return_code print_memory_dump(void* stream, TYPE_OF_PRINT type_of_print, void *ptr, long unsigned int size_of_data_under_pointer);

enum return_code handle_custom_flags(void* stream, TYPE_OF_PRINT type_of_print, const char* pos, va_list* args, int* to_move);

enum return_code overfprintf(FILE* stream, const char* format, ...);
enum return_code oversprintf(char* stream, char const* format, ...);


#endif