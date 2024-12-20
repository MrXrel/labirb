#include "functions.h"
#include <stdio.h>
#include <linux/limits.h>

int main(int argc, char* argv[]) {
    char absolute_path_1[PATH_MAX];
    char absolute_path_2[PATH_MAX];
    FILE* input_file, *output_file;
    int size = 0;
    Student* students = NULL;

    if (argc != 3) {
        printf("Invalid amount of arguments\n");
        return INVALID_INPUT;
    }

    if (!realpath(argv[1], absolute_path_1) || !realpath(argv[2], absolute_path_2)) {
        return INVALID_INPUT;
    }

    if (strcmp(absolute_path_1, absolute_path_2) == 0) {
        printf ("The same paths\n");
        return INVALID_INPUT;
    }

    if (!(input_file = fopen(absolute_path_1, "r"))) {
        printf("Couldn't open file");
        return FILE_OPENNING_ERROR;
    }

    if (!(output_file = fopen(absolute_path_2, "w"))) {
        fclose(input_file);
        printf("Couldn't open file");
        return FILE_OPENNING_ERROR;
    }

    int res = upload_info(&students, &size, input_file);
    if (res != 0) {
        printf("Invalid Input\n");
        return res;
    }
    
    interactive_dialog(students, size, output_file);

    fclose(input_file);
    fclose(output_file);

    for (int i = 0; i < size; ++i) {
        free(students[i].marks);
    }
    free(students);
}