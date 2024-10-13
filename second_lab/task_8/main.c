#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void good_tests() {
    char* sums[6] = {"", "", "", "", "", "", ""};

    enum return_code res[6];

    res[0] = calculate_sum(&sums[0], 2, 2, "1101", "11");
    printf("Base 2: 1101 + 11 = %s\n", sums[0]);

    res[1] = calculate_sum(&sums[1], 16, 4, "1101", "11", "51", "+00000C");
    printf("Base 16: 1101 + 11 +51 + 00000C = %s\n", sums[1]);

    res[2] = calculate_sum(&sums[2], 5, 5, "1101", "11", "41", "000", "004");
    printf("Base 5: 1101 + 11 + 41 + 000 + 004 = %s\n", sums[2]);

    res[3] = calculate_sum(&sums[3], 12, 4, "A", "11", "51", "1");
    printf("Base 12: A + 11 + 51 + 1 = %s\n", sums[3]);

    res[4] = calculate_sum(&sums[4], 10, 3, "1101", "11", "51");
    printf("Base 10: 1101 + 11 + 51 = %s\n", sums[4]);

    res[5] = calculate_sum(&sums[5], 12, 3, "0000", "000000", "0");
    printf("Base 12: 0000 + 000000 + 0 = %s\n", sums[5]);
    
    for (int i = 0; i < 6; ++i){
        if (res[i] == OK) {
            free(sums[i]);
        }
    }
}

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
        default:
            break;
    }
}

int main() { 
    // char* r;
    char* str;
    int rt = calculate_sum(&str, 16, 3, "122", "392", "329");
    if (rt != OK) {
        print_mistakes(rt);
        return rt;
    }
    printf("Sum = %s\n", str);
    free(str);
    good_tests();
}