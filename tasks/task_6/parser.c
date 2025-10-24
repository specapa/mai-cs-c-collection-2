#include "parser.h"
#include "stdio.h"
#include "string.h"
#include "../../include/utils_2.h"
#include "student.h"
#include "vectors.h"

void using(FILE *stream, char *run_command) {
    fprintf(stream, "Using: %s <-f> <-o>\n"
    "FLAGS:\n"
    "[-f] -- The input file (REQUIRED)\n"
    "[-o] -- The out trace file\n", run_command);
}

u_status_t seperate_string_to_args(string_t *string, string_vec_t *argv) {

}

command_enum_t command_processing(FILE *stream) {
    fprintf(stream,
    RIP
    COMMAND_SEARCH_CALL " [ID, N (for Name), S (for Surname), G (for Group)] [VALUE]\n"
    COMMAND_SORT_CALL " [ID, N (Name), S (Surname), G (Group)]\n"
    COMMAND_TS_CALL " [ID, N (for Name), S (for Surname), G (for Group)] [VALUE]\n"
    "/exit\n");

    u_status_t status;
    
    char * pch = strtok (stream," ");
    
    command_enum_t command = COMMAND_SKIP;
    if (strcmp(pch, COMMAND_SEARCH_CALL) == 0) {
        command = COMMAND_SEARCH;
    } else if (strcmp(pch, COMMAND_SORT_CALL) == 0) {
        command = COMMAND_SORT;
    } else if (strcmp(pch, COMMAND_TS_CALL) == 0) {
        command = COMMAND_TS;
    } else if (strcmp(pch, COMMAND_EXIT_CALL) == 0) {
        return COMMAND_EXIT;
    } else {
        return COMMAND_SKIP;
    }
    
    while (pch != NULL)
    {
        pch = strtok (NULL, " ");
    }
    return 0;

    char *scanned;
    status = read_string(stdin, &scanned);
    if (status != U_OK) {
        free(scanned);
        scanned = NULL;
        fprintf(stderr, "ERROR: can't init string");
        return status;
    }

    for (uint i = 0; scanned[i] != '\0'; ++i) {

    }

    free(scanned);

    return command;
}

int iterate(students_t **students) {
    command_enum_t command = command_processing(stdout);
    return command;
}