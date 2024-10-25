#include "main.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    int* ans;
    int res = find_kaprekar_nums(&ans, 16, 5, "1", "6", "FF", "88", "191A");
    if (res != OK) {
        return res;
    }
    for (int i = 0; i < 5; i++) {
        printf("%d ", ans[i]);
    }
    free(ans);
    printf("\n");

    double res_p;
    evaluate_polynomial(&res_p, -0.5, 4, 0., 1., 4., 23., 5.);
    printf("%.15lf ", res_p);
    printf("\n");

    int result_of_convex;
    is_convex(&result_of_convex, 4, 0., .5, 0., 1., 0.5, 22., 1., -2.);
    printf("%d\n", result_of_convex);

}
