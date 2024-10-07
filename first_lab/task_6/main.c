#include <stdio.h>
#include "main.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Wrong amount of arguments\n");
        return WRONG_ARGUMENTS_ERROR;
    }
    
    double eps;
    if (transfer_to_double(argv[1], &eps) != OK || eps <= 0) {
        printf("Invalid input\n");
        return BAD_INPUT_ERROR;
    }

    function func_to_calculate[4] = {func_a, func_b, func_c, func_d};
    method_to_calculate methods[4] = {method_rect_center, method_sympson, method_rect_left, method_rect_right};
    char points[4] = {'a', 'b', 'c', 'd'};
    for (int i = 0; i < 4; ++i) {
        double result = 0;
        if (calculate_integral(func_to_calculate[i], 0, 1, eps, methods[i], &result) != OK) {
            printf("%c. Can't calculate\n", points[i]);
            continue;
        }
        printf("%c. %.15lf\n", points[i], result);
    }
    return OK;
}