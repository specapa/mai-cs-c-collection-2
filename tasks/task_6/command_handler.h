#ifndef COMMANDS_HANDLER_H
#define COMMANDS_HANDLER_H

#include <stdio.h>
#include "student.h"
#include "parser.h"

void handle_command(command_enum_t command, search_enum_t search_by, const char *value, students_t *students, FILE *trace);

void handle_search(search_enum_t search_by, const char *value, students_t *students, FILE *trace);
void handle_sort(search_enum_t search_by, students_t *students);
void handle_trace(search_enum_t search_by, const char *value, students_t *students, FILE *trace);

#endif // COMMANDS_HANDLER_H
