#ifndef PARSER_H
#define PARSER_H

#include "student.h"

#define RIP "\e[1;1H\e[2J"

typedef enum commands_list_ {
    COMMAND_EXIT = -1,
    COMMAND_SKIP = 0,
    COMMAND_SEARCH = 1,
    COMMAND_SORT,
    COMMAND_TS
} command_enum_t;

#define COMMAND_SEARCH_CALL "/search"
#define COMMAND_SORT_CALL "/sort"
#define COMMAND_TS_CALL "/trace-search"
#define COMMAND_EXIT_CALL "/exit"

void using(FILE *stream, char *run_command);
int iterate(students_t **students);

#endif // PARSER_H