#include "parser.h"
#include <../../include/utils_2.h>
#include <string.h>
#include <ctype.h>

u_status_t flag_parse(char *flag, char *value, unsigned long long *adr_to_wrt) {
    u_status_t status;
    status = parse_unsigned_ll(value, adr_to_wrt, 0, ULONG_MAX);
    if (status != U_OK) fprintf(stderr, "At flag_parse(%s, %s, %p) ", flag, value, adr_to_wrt);
    return status;
}

u_status_t parse_arg_vector(int argc, char *argv[], usgshrt_vec_vec_t *vec, unsigned long long *source_base, unsigned long long *target_base, unsigned long long *max_deep) {
    
    u_status_t current_status;

    if (vec == NULL) {
        current_status = usgshrt_vec_vec_init(&vec);
        if (current_status != U_OK) return current_status;
    }

    for (size_t i = 1; i < argc; ++i) {
        if ((strlen(argv[i]) == 2) && (argv[i][0] == '-')) {
            if (i + 1 < argc) {
                unsigned long long tmp;
                current_status = flag_parse(argv[i], argv[i + 1], &tmp);
                if (current_status != U_OK) {
                    fprintf(stderr, "ERROR: can't parse flag due %s\n", u_status_to_string(current_status));
                    return current_status;
                }
                switch (argv[i][1]) {
                    case 's': *source_base = tmp; break;
                    case 'b': *target_base = tmp; break;
                    case 'l': *max_deep = tmp; break;
                }
                ++i;
                continue;
            } else {
                current_status = U_INVALID_FORMAT;
                fprintf(stderr, "ERROR: wrong flag format, can't read value after flag due %s\n", u_status_to_string(current_status));
                return current_status;
            }
        }

        usgshrt_vec_t *tmp = NULL;
        current_status = usgshrt_vec_init(&tmp);
        if (current_status != U_OK) {
            fprintf(stderr, "INFO: can't init tmp usgshrt_vec [%s] due %s\n", argv[i], u_status_to_string(current_status));
            continue;
        }
        current_status = parse_number_to_vector(argv[i], &tmp);
        if (current_status != U_OK) {
            fprintf(stderr, "INFO: can't parse arg with id %ld [%s] due %s\n", i, argv[i], u_status_to_string(current_status));
            continue;
        }
        usgshrt_vec_vec_push_back(vec, tmp);
    }

    return U_OK;
}

u_status_t parse_number_to_vector(char *string, usgshrt_vec_t **vec) {

    if (string == NULL || vec == NULL) return U_INVALID_FORMAT;
    usgshrt_vec_free(vec);

    u_status_t stat;
    usgshrt_vec_t *tmp = NULL;
    const char *p = string;

    stat = usgshrt_vec_init(&tmp);
    if (stat != U_OK) return stat;
    
    while (isspace((unsigned char)*p)) ++p;
    if (*p == '+' || *p == '-') {
        ++p;
    }
    while (*p && *p != '.') {
        if (!isdigit((unsigned char)*p)) return U_INVALID_FORMAT;
        ++p;
    }
    if (*p && (unsigned char)*p == '.') { 
        ++p;
    } else return U_INVALID_FORMAT;

    while (*p) {
        if (!isdigit((unsigned char)*p)) return U_INVALID_FORMAT;
        int d = *p - '0';
        stat = usgshrt_vec_push_back(tmp, d);
        if (stat != U_OK) return stat;
        ++p;
    }

    *vec = tmp;

    return U_OK;
}