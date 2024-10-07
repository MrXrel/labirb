#include <stdio.h>
#include "main.h"

int main() {
    long biggest;
    int base;    
    int res = find_biggest_num_in_stdin(&biggest, &base);
    
    switch (res)
    {
    case BAD_POINTER_ERROR:
        printf("Pointer is NULL\n");
        return BAD_POINTER_ERROR;
    
    case BAD_INPUT_ERROR:
        printf("Invalid input\n");
        return BAD_INPUT_ERROR;
    
    case OVERFLOW_ERROR:
        printf("Overflow detected\n");
        return OVERFLOW_ERROR;

    case NO_NUMBERS_IN_STDIN:
        printf("No numbers after base\n");
        return NO_NUMBERS_IN_STDIN;
    default:
        break;
    }
    printf("Original:\n");
    print_in_base_to_stdout(biggest, base);
    printf("\n");
    print_in_base_to_stdout(biggest, 9);
    print_in_base_to_stdout(biggest, 18);
    print_in_base_to_stdout(biggest, 27);
    print_in_base_to_stdout(biggest, 36);
    return OK;
}