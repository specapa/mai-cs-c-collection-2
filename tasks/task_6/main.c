#include <stdio.h>
#include <stdlib.h>
#include "../../include/utils_2.h"
#include "student.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        using(stderr, argv[0]);
        return 115;
    }

    u_status_t status;
    students_t *students = NULL;

    status = students_init(&students); 
    if (status != U_OK) { 
        fprintf(stderr, "ERROR at init students_t: "); 
        u_status_print(stderr, status, u_status_to_string); 
        students_free(&students);
        return 13;
    }

    while (iterate(&students) != COMMAND_EXIT) {}

    return 0;
}