#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "return_code.h"
#include <stdio.h>

#define BUFFER_SIZE 1024

typedef int (*format_for_char) (int);

enum return_code overfscanf(FILE* stream, char const* format, ...);
enum return_code oversscanf(const char* stream, char const* format, ...);
 
#endif
