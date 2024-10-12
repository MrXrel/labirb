#include "main.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("No flag\n");
        return WRONG_ARGUMENTS_ERROR;
    }

    if (is_valid_flag(argv[1]) != OK) {
        printf("Wrong flag\n");
        return WRONG_ARGUMENTS_ERROR;
    }
    char flag = argv[1][1];
    if (flag != 'c' && argc != 3) {
        printf("Wrong amount of arguments\n");
        return WRONG_ARGUMENTS_ERROR;
    }

    int res;
    switch (flag) {
        case 'l':;
            int length = 0;
            for_l(argv[2], &length);
            printf("Len of str: %d\n", length);
            break;
        case 'r':;
            char* reversed_str;
            res = for_r(argv[2], &reversed_str);
            if (res != OK) {
                print_error_message(res);
                return res;
            }

            printf("Reversed: %s\n", reversed_str);
            free(reversed_str);
            break;
        case 'u':;
            char* transformed_u_str;
            res = for_u(argv[2], &transformed_u_str);
            if (res != OK) {
                print_error_message(res);
                return res;
            }
            printf("Transformed string: %s\n", transformed_u_str);

            free(transformed_u_str);
            break;
        case 'n':;
            char* transformed_n_str;
            res = for_n(argv[2], &transformed_n_str);
            if (res != OK) {
                print_error_message(res);
                return res;
            }
            printf("Transformed string: %s\n", transformed_n_str);

            free(transformed_n_str);
            break;

        case 'c':;
            if (argc < 5) {
                printf("Wrong amount of arguments\n");
                return WRONG_ARGUMENTS_ERROR;
            }
            int seed;
            int result_of_transfer_to_int = transfer_to_int(argv[3], &seed);
            if (result_of_transfer_to_int != OK) {
                print_error_message(result_of_transfer_to_int);
                return result_of_transfer_to_int;
            }
            char* result;
            res = for_c(argc, argv, seed, &result);
            if (res != OK) {
                print_error_message(res);
                return res;
            }

            printf("Concanated result: %s\n", result);
            free(result);
            break;
        default:
            break;
    }
    return OK;
}