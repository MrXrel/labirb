#include <stdio.h>
#include <math.h>
#include <stdlib.h> 
#include <time.h> 
#include <limits.h>
#include <string.h>

enum return_code {
    OK = 0,
    BAD_INPUT_ERROR = 1,
    OVERFLOW_ERROR = 2,
    BAD_POINTER_ERROR = 3,
    MEMORY_ALLOC_ERROR = 4,
    WRONG_ARGUMENTS_ERROR = 5,
    NOT_A_DOUBLE_ERROR = 6,
    NOT_AN_INTEGER_ERROR = 7,
    SIZE_OF_MASSIVE_IS_DIFFERENT_ERROR = 8,

};

#define SIZE_OF_MASSIVE 10
#define LEFT_BORDER_DYNAMIC_ARRAY 1
#define RIGHT_BORDER_DYNAMIC_ARRAY 10

#define MIN_RANGE -10
#define MAX_RANGE 10

int transfer_to_int(const char* str, int* result) {
    int len = strlen(str);
    int sign = 1;
    *result = 0;
    for (int i = 0; i < len; i++) {
        if ((i == 0) && str[i] == '-') {
            sign = -1;
            continue;
        }
        if (!(str[i] >= '0' && str[i] <= '9')) {
            return NOT_AN_INTEGER_ERROR;
        }
        if (*result > (INT_MAX - (str[i] - '0')) / 10) {
            return OVERFLOW_ERROR;
        }

        *result *= 10;
        *result += (str[i] - '0');
    }
    *result *= sign;
    return 0;
}

enum return_code fill_static_massive(int arr[SIZE_OF_MASSIVE], int left_border, int right_border) {
    if (!arr) {
        return BAD_POINTER_ERROR;
    }
    for (int i = 0; i < SIZE_OF_MASSIVE; ++i) {
        arr[i] = left_border + rand() % (right_border - left_border + 1);
    }
    return OK;
}

enum return_code swap_min_max_value(int arr[SIZE_OF_MASSIVE], int* min_val, int* max_val) {
    if (!arr || !min_val || !max_val) {
        return BAD_POINTER_ERROR;
    }    
    int min_idx = 0, max_idx = 0;

    *min_val = arr[0];
    *max_val = arr[0];

    for (int i = 1; i < SIZE_OF_MASSIVE; ++i) {
        if (arr[i] < *min_val) {
            *min_val = arr[i];
            min_idx = i;
        }
        if (arr[i] > *max_val) {
            *max_val = arr[i];
            max_idx = i;
        }
    }

    int temp = arr[min_idx];
    arr[min_idx] = arr[max_idx];
    arr[max_idx] = temp;
    return OK;
}

int comp(const void* a, const void* b) {
    int f = *((int*)a);
    int s = *((int*)b);
    if (f > s) return 1;
    if (f < s) return -1;
    return 0;
}

enum return_code fill_dynamic_arrays(int* arr, int size_of_arr) {
    if (!arr) {
        return BAD_POINTER_ERROR;
    }
    if (size_of_arr <= 0) {
        return BAD_INPUT_ERROR;
    }
    for (int i = 0; i < size_of_arr; ++i) {
        arr[i] = MIN_RANGE + rand() % (MAX_RANGE - MIN_RANGE + 1);
    }
    return OK;
}

enum return_code print_dynamic_arrays(int* arr, int size_of_arr) {
    if (!arr) {
        return BAD_POINTER_ERROR;
    }
    if (size_of_arr < 0) {
        return BAD_POINTER_ERROR;
    }
    for (int i = 0; i < size_of_arr; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return OK;
}

int find_closest(int* arr, int size, int target) {
    int left = 0, right = size - 1;
    int closest = arr[0];
    int min_diff = abs(target - closest);

    while (left <= right) {
        int mid = (right + left) * 0.5;
        int diff = abs(target - arr[mid]);

        if (diff < min_diff) {
            min_diff = diff;
            closest = arr[mid];
        }

        if (arr[mid] < target) {
            left = mid + 1;
        } else if (arr[mid] > target) {
            right = mid - 1;
        } else {
            break;
        }
    }

    return closest;
}


enum return_code fill_array_c(int* A, int* B, int* C, int size_of_a, int size_of_b, int size_of_c) {
    if (!A || !B || !C) {
        return BAD_POINTER_ERROR;
    }
    if (size_of_a <= 0 || size_of_b <= 0 || size_of_c <= 0) {
        return BAD_INPUT_ERROR;
    }
    if (size_of_a != size_of_c) {
        return SIZE_OF_MASSIVE_IS_DIFFERENT_ERROR;
    }
    
    qsort(B, size_of_b, sizeof(int), &comp);

    int closest_value;
    for (int i = 0; i < size_of_a; ++i) {
        closest_value = find_closest(B, size_of_b, A[i]);
        C[i] = A[i] + closest_value;
    }
    return OK;
}

void print_static_massive(int arr[SIZE_OF_MASSIVE]) {
    for (int i = 0; i < SIZE_OF_MASSIVE; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

enum return_code modify_static_massive(int left_border, int right_border) {
    int arr[SIZE_OF_MASSIVE];
    fill_static_massive(arr, left_border, right_border);
    printf("Static array before modification\n");
    print_static_massive(arr);
    int max_value, min_value;
    swap_min_max_value(arr, &min_value, &max_value);
    printf("Static array after modification\n");
    print_static_massive(arr);

    return OK;
} 

enum return_code print_dynamic_results(int* A, int* B, int* C, int size_of_array) {
    if (!A || !B || !C) {
        return BAD_POINTER_ERROR;
    }
    if (size_of_array < 0) {
        return BAD_INPUT_ERROR;
    }
    printf("Array A: ");
    print_dynamic_arrays(A, size_of_array);
    printf("Array B: ");
    print_dynamic_arrays(B, size_of_array);
    printf("Array C: ");
    print_dynamic_arrays(C, size_of_array);
    return OK;
}

enum return_code modify_dynamic_arrays(enum return_code (*print_result_func)(int*, int*, int*, int)) {

    int size_of_array = LEFT_BORDER_DYNAMIC_ARRAY + rand() % (RIGHT_BORDER_DYNAMIC_ARRAY - LEFT_BORDER_DYNAMIC_ARRAY + 1);

    int* A = (int*)malloc(size_of_array * sizeof(int));
    if (!A) {
        return MEMORY_ALLOC_ERROR;
    }

    int* B = (int*)malloc(size_of_array * sizeof(int));
    if (!B) {
        free(A);
        return MEMORY_ALLOC_ERROR;
    }
    int* C = (int*)malloc(size_of_array * sizeof(int));
    if (!C) {
        free(A);
        free(B);
        return MEMORY_ALLOC_ERROR;
    }

    fill_dynamic_arrays(A, size_of_array);
    fill_dynamic_arrays(B, size_of_array);
    fill_array_c(A, B, C, size_of_array, size_of_array, size_of_array);
    
    print_result_func(A, B, C, size_of_array);
    free(A);
    free(B);
    free(C);
    return OK;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong amount of arguments\n");
        return WRONG_ARGUMENTS_ERROR;
    }

    int left_border, right_border;
    if (transfer_to_int(argv[1], &left_border) != OK || transfer_to_int(argv[2], &right_border) != OK) {
        printf("Invalid input\n");
        return BAD_INPUT_ERROR;
    }

    if (right_border <= left_border) {
        printf("Invalid input\n");
        return BAD_INPUT_ERROR;
    }

    srand(time(NULL));

    modify_static_massive(left_border, right_border);
    
    if (modify_dynamic_arrays(&print_dynamic_results) != OK){
        printf("Memory allocation error\n");
        return MEMORY_ALLOC_ERROR;
    }

    return OK;
}