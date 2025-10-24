#ifndef PARSER_H
#define PARSER_H

#include "student.h"

#define RIP "\e[1;1H\e[2J"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define COMMAND_SEARCH_CALL "/search"
#define COMMAND_TRASE_SEARCH_CALL "/trace-search"
#define COMMAND_TRASE_CALL "/trace"
#define COMMAND_SORT_CALL "/sort"
#define COMMAND_EXIT_CALL "/exit"

typedef enum commands_list_ {
    COMMAND_EXIT = -1,
    COMMAND_SKIP = 0,
    COMMAND_SEARCH = 1,
    COMMAND_TS,
    COMMAND_TRACE,
    COMMAND_SORT,
} command_enum_t;

#define FLAG_ID "ID"
#define FLAG_NAME "N"
#define FLAG_SURNAME "S"
#define FLAG_GROUP "G"

typedef enum search_enum_ {
    WRONG = -1,
    ID = 0,
    NAME,
    SURNAME,
    GROUP
} search_enum_t;

void commands(FILE *stream);
void using(FILE *stream, char *run_command);
void unifreestring(int n, ...);
command_enum_t iterate(search_enum_t *search_by, char **value);

#endif // PARSER_H