#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void print_mistakes(enum return_code error) {
    switch (error) {
        case MEMORY_ALLOC_ERROR:
            printf("Memory allocation error\n");
            break;
        case BAD_INPUT_ERROR:
            printf("Invalid input\n");
            break;
        case OVERFLOW_ERROR:
            printf("Overflow detected\n");
            break;
        case BAD_POINTER_ERROR:
            printf("Dereferencing NULL pointer\n");
            break;
        case FILE_OPEN_ERROR:
            printf("Error in opening file\n");
            break;
        default:
            break;
    }
}

int main() {     
    int result = search_in_files_and_write_result("i", 2, "/home/artem/labirb/second_lab/task_3/test.txt", "test2.txt");
    if (result != OK) {
        print_mistakes(result);
        return result;
    }
    return OK;
}