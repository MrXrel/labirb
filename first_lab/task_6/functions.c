#include <stdio.h>
#include "functions.h"

enum return_code {
    OK = 0,
    BAD_INPUT_ERROR = 1,
    OVERFLOW_ERROR = 2,
    BAD_POINTER_ERROR = 3,
    MEMORY_ALLOC_ERROR = 4,
    WRONG_ARGUMENTS_ERROR = 5,
    NOT_A_DOUBLE_ERROR = 6,
    NOT_AN_INTEGER_ERROR = 7,
};

int get_number(int allo, double a) {
    return allo + a;
}




