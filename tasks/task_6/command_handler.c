#include "command_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void handle_command(command_enum_t command, search_enum_t search_by, const char *value,
                    students_t *students, FILE *trace) {
    if (!students) {
        fprintf(stderr, "ERROR: students not initialized\n");
        return;
    }

    if (search_by == WRONG) {
        fprintf(stderr, "ERROR: empty search by type\n");
        return;
    }


    switch (command) {
        case COMMAND_SEARCH:
            handle_search(search_by, value, students, NULL);
            break;
        case COMMAND_TS:
            handle_search(search_by, value, students, trace);
            break;
        case COMMAND_TRACE:
            handle_trace(search_by, value, students, trace);
            break;
        case COMMAND_SORT:
            handle_sort(search_by, students);
            break;
        default:
            fprintf(stderr, "Unknown command\n");
            break;
    }
}

void handle_search(search_enum_t search_by, const char *value, students_t *students, FILE *trace) {
    if (!value) {
        fprintf(stderr, "ERROR: search value is NULL\n");
        return;
    }

    students_t *found = NULL;

    switch (search_by) {
        case ID: {
            size_t id = strtoul(value, NULL, 10);
            found = students_find_by_id(students, id);
            break;
        }
        case NAME:
            found = students_find_by_name(students, value);
            break;
        case SURNAME:
            found = students_find_by_surname(students, value);
            break;
        case GROUP:
            found = students_find_by_group(students, value);
            break;
    }

    if (found && found->size > 0) {
        printf("Found %zu student(s):\n", found->size);
        for (size_t i = 0; i < found->size; ++i) {
            const student_t *s = found->vec[i];
            printf("%zu. %s %s (Group: %s, Avg: %.2f)\n",
                   i + 1, s->name, s->surname, s->group, student_avg(s));
            if (trace) {
                fprintf(trace, "%zu. %s %s (Group: %s, Avg: %.2f)\n",
                        i + 1, s->name, s->surname, s->group, student_avg(s));
            }
        }
        fflush(trace);
    } else {
        printf("Can't find students with this condition\n");
    }

    if (found) {
        free(found->vec);
        free(found);
    }
}

void handle_sort(search_enum_t search_by, students_t *students) {
    const char *sort_type = "";

    switch (search_by) {
        case ID:
            students_sort(students, cmp_by_id);
            sort_type = "ID";
            break;
        case NAME:
            students_sort(students, cmp_by_name);
            sort_type = "name";
            break;
        case SURNAME:
            students_sort(students, cmp_by_surname);
            sort_type = "surname";
            break;
        case GROUP:
            students_sort(students, cmp_by_group);
            sort_type = "group";
            break;
        default: 
            return;
    }

    printf("Sorted students by %s:\n", sort_type);
    for (size_t i = 0; i < students->size; ++i) {
        const student_t *s = students->vec[i];
        printf("%zu. %s %s (Group: %s, ID: %zu, Avg: %.2f)\n",
               i + 1, s->name, s->surname, s->group, s->id, student_avg(s));
    }
}

void handle_trace(search_enum_t search_by, const char *value, students_t *students, FILE *trace) {
    (void) search_by;
    (void) value;

    double overall_avg = students_avg(students);
    printf("Overall average: %.2f\n", overall_avg);
    printf("Students above average:\n");

    size_t count = 1;
    for (size_t i = 0; i < students->size; ++i) {
        student_t *s = students->vec[i];
        double avg = student_avg(s);
        if (avg > overall_avg) {
            printf("%zu. %s %s (Group: %s, Avg: %.2f)\n",
                   count++, s->name, s->surname, s->group, avg);
        }
    }

    if (trace) {
        fprintf(trace, "Students above average (%.2f):\n", overall_avg);
        for (size_t i = 0; i < students->size; ++i) {
            student_t *s = students->vec[i];
            if (student_avg(s) > overall_avg) {
                fprintf(trace, "%s %s\n", s->surname, s->name);
            }
        }
        fflush(trace);
        printf("\nStudents have been traced to the trace file.\n");
    }
}
