#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "return_code.h"
#include "string.h"
#include "files.h"

#define INITIAL_LENGTH_OF_STRING 512

enum return_code search_in_files_and_write_result(const char *substring, int num_files, ...);


#endif