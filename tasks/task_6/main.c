#include <stdio.h>
#include <stdlib.h>
#include "../../include/utils_2.h"
#include "student.h"
#include "parser.h"
#include "command_handler.h"

int main(int argc, char *argv[]) {
    if (argc != 3 && argc != 5) {
        using(stderr, argv[0]);
        return 115;
    }

    const char *students_file = argv[2];
    FILE *trace;

    if (argc == 3) {
        trace = stdout;
    } else {
        const char *trace_file = argv[4];
        trace = fopen(trace_file, "w");
        if (!trace) {
            fprintf(stderr, "ERROR: cannot open trace file '%s'\n", trace_file);
            return 116;
        }
    }

    commands(stdout);

    u_status_t status;
    students_t *students = NULL;

    status = students_init(&students); 
    if (status != U_OK) { 
        fprintf(stderr, "ERROR at init students_t: "); 
        u_status_print(stderr, status, u_status_to_string); 
        students_free(&students);
        return 13;
    }

    status = students_load(students, students_file);
    if (status != U_OK) {
        fprintf(stderr, "ERROR: cannot load students from '%s'\n", students_file);
        u_status_print(stderr, status, u_status_to_string);
        students_free(&students);
        fclose(trace);
        return 14;
    }

    printf("Loaded %zu students from '%s'\n\n", students->size, students_file);

    command_enum_t command;
    search_enum_t search_by;
    char *value = NULL;

    while ((command = iterate(&search_by, &value)) != COMMAND_EXIT) {
        if (command == COMMAND_SKIP) continue;
        
        handle_command(command, search_by, value, students, trace);

        unifreestring(1, &value);
    }

    students_free2(&students);
    if (trace && trace != stdout)
        fclose(trace);

    return 0;
}