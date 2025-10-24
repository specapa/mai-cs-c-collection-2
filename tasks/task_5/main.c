#include <stdio.h>
#include <stdlib.h>
#include "../../include/utils_2.h"
#include "process_file.h"

int main(void) {
    char *input_file_name = NULL;
    char *output_file_name = NULL;
    u_status_t status;

    status = U_NEGATIVE_NOT_ALLOWED;
    while (status != U_OK) {
        printf("Input file: ");
        status = read_string(stdin, &input_file_name);
    }

    status = U_NEGATIVE_NOT_ALLOWED;
    while (status != U_OK) {
        printf("Output file: ");
        status = read_string(stdin, &output_file_name);
    }

    status = process_file(input_file_name, output_file_name);
    if (status != U_OK) {
        free(input_file_name);
        free(output_file_name);
        return 145;
    }

    printf("Writed to '%s'\n", output_file_name);
    free(input_file_name);
    free(output_file_name);
    return 0;
}