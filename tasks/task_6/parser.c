#include "parser.h"
#include <stdarg.h>
#include "stdio.h"
#include "string.h"
#include "../../include/utils_2.h"
#include "student.h"
#include "vectors.h"

void commands(FILE *stream) {
    fprintf(stream,
    ANSI_COLOR_MAGENTA
    RIP
    COMMAND_SEARCH_CALL " [" FLAG_ID " (ID), " FLAG_NAME " (Name), " FLAG_SURNAME " (Surname), " FLAG_GROUP " (Group)] [VALUE]\n"
    COMMAND_TRASE_SEARCH_CALL " [" FLAG_ID " (ID), " FLAG_NAME " (Name), " FLAG_SURNAME " (Surname), " FLAG_GROUP " (Group)] [VALUE]\n"
    COMMAND_TRASE_CALL "\n"
    COMMAND_SORT_CALL " [" FLAG_ID " (ID), " FLAG_NAME " (Name), " FLAG_SURNAME " (Surname), " FLAG_GROUP " (Group)]\n"
    COMMAND_EXIT_CALL "\n"
    ANSI_COLOR_RESET);
}

void using(FILE *stream, char *run_command) {
    fprintf(stream, "Using: %s <-f> <-o>\n"
    "FLAGS:\n"
    "[-f] -- The input file (REQUIRED)\n"
    "[-o] -- The out trace file\n", run_command);
}

void unifreestring(int n, ...) {
    char **string;
    va_list factor;
    va_start(factor, n);
    for(int i = 0; i < n; ++i) {
        string = va_arg(factor, char **);
        free(*string);
        *string = NULL;
    }
    va_end(factor);
}

command_enum_t command_processing(FILE *stream, search_enum_t *search_by_p, char **value_p) {
    u_status_t status;
    
    char *line;
    status = read_string(stdin, &line);
    if (status != U_OK) {
        unifreestring(1, &line);
        fprintf(stderr, "ERROR: can't init string");
        return status;
    }
    char *pch = strtok(line, " ");
    
    command_enum_t command = COMMAND_SKIP;
    if (strcmp(pch, COMMAND_SEARCH_CALL) == 0) {
        command = COMMAND_SEARCH;
    } else if (strcmp(pch, COMMAND_TRASE_SEARCH_CALL) == 0) {
        command = COMMAND_TS;
    } else if (strcmp(pch, COMMAND_TRASE_CALL) == 0) {
        unifreestring(1, &line);
        return COMMAND_TRACE;
    } else if (strcmp(pch, COMMAND_SORT_CALL) == 0) {
        command = COMMAND_SORT;
    } else if (strcmp(pch, COMMAND_EXIT_CALL) == 0) {
        unifreestring(1, &line);
        return COMMAND_EXIT;
    } else {
        printf(ANSI_COLOR_RED "Wrong command!\n" ANSI_COLOR_RESET);
        unifreestring(1, &line);
        return COMMAND_SKIP;
    }

    pch = strtok(NULL, " ");
    search_enum_t search = WRONG;

    if (strcmp(pch, FLAG_ID) == 0) {
        search = ID;
    } else if (strcmp(pch, FLAG_NAME) == 0) {
        search = NAME;
    } else if (strcmp(pch, FLAG_SURNAME) == 0) {
        search = SURNAME;
    } else if (strcmp(pch, FLAG_GROUP) == 0) {
        search = GROUP;
    } else {
        printf(ANSI_COLOR_RED "Wrong command format!\n" ANSI_COLOR_RESET);
        unifreestring(1, &line);
        return COMMAND_SKIP;
    }

    *search_by_p = search;
    char *value = strtok(NULL, " ");
    if (value) {
        *value_p = strdup(value);
    } else {
        *value_p = NULL;
    }

    unifreestring(1, &line);
    return command;
}

command_enum_t iterate(search_enum_t *search_by, char **value) {
    command_enum_t command = command_processing(stdout, search_by, value);
    return command;
}