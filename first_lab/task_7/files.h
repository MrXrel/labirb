#include <stdio.h>
#include <string.h>
#include <unistd.h>

enum file_errors_code {
    FILE_OK = 0,
    FILE_BAD_POINTER_ERROR = 3,
    FILE_PATH_IS_TOO_LONG_ERROR = 7,

};

// #define _WIN64
// #define _WIN32

int is_absolute_path(const char* path) {
#if defined _WIN32 || defined _WIN64
    return (strlen(path) > 2 && path[1] == ':' && path[2] == '\\');
#else
    return (path[0] == '/');
#endif
}

enum file_errors_code modify_path_to_absolute(const char* way_to_current_directory, const char* old_path, char* new_absolute_path_result) {
    if (!way_to_current_directory || !old_path || !new_absolute_path_result) {
        return FILE_BAD_POINTER_ERROR;
    }
    if (strlen(old_path) > FILENAME_MAX) {
        return FILE_PATH_IS_TOO_LONG_ERROR;
    }

    if (is_absolute_path(old_path)) {
        strcpy(new_absolute_path_result, old_path);
        return FILE_OK;
    }

    if (strlen(way_to_current_directory) + strlen(old_path) > FILENAME_MAX) {
        return FILE_PATH_IS_TOO_LONG_ERROR;
    }
    strcat(new_absolute_path_result, way_to_current_directory);
    strcat(new_absolute_path_result, old_path);
    return FILE_OK;
}

enum file_errors_code get_path_of_current_directory(char* path_to_directory_result) {
    if (!path_to_directory_result) {
        return FILE_BAD_POINTER_ERROR;
    }
    getcwd(path_to_directory_result, FILENAME_MAX - 1);
    if (!path_to_directory_result) {
        return FILE_PATH_IS_TOO_LONG_ERROR;
    }
    strcat(path_to_directory_result, "/");
    return FILE_OK;
}

enum file_errors_code get_output_filename(char const* path_input_file, char* path_output_file_result) {
    if (!path_output_file_result || !path_input_file) {
        return FILE_BAD_POINTER_ERROR;
    }
    int size_of_path_to_file = strlen(path_input_file);
    if (size_of_path_to_file > FILENAME_MAX) {
        return FILE_PATH_IS_TOO_LONG_ERROR;
    }
    char const* ch_of_path = path_input_file;
    ch_of_path += size_of_path_to_file;
    // do not get seg f because path_input_file is absolute => always have /
    do {
        --ch_of_path;
    } while (*ch_of_path != '/');
    strncat(path_output_file_result, path_input_file, ch_of_path - path_input_file + 1);
    strcat(path_output_file_result, "out_");
    strcat(path_output_file_result, ch_of_path + 1);

    return FILE_OK;
}