#include "parser.h"
#include <../../include/utils_2.h>
#include <string.h>

u_status_t flag_parse(char *flag, char *value, unsigned long long *adr_to_wrt) {
    u_status_t status;
    status = parse_unsigned_ll(value, adr_to_wrt, 0, ULONG_MAX);
    if (status != U_OK) fprintf(stderr, "At parsing base value (value after flag %s: %s) ", flag, value);
    return status;
}

u_status_t parse_arg_vector(int argc, char *argv[], double_vec_t *dv, unsigned long long *base) {
    u_status_t current_status;
    long double current_double;

    if (dv == NULL) {
        current_status = double_vec_init(&dv);
        if (current_status != U_OK) return current_status;
    }

    for (size_t i = 1; i < argc; ++i) {
        if ((argv[i][0] == '-') && (argv[i][1] == 'b')) {
            if (i + 1 < argc) {
                current_status = flag_parse(argv[i], argv[++i], base);
                if (current_status != U_OK) {
                    fprintf(stderr, "ERROR: can't parse flag: ");
                    return current_status;
                }
                continue;
            } else {
                fprintf(stderr, "ERROR: wrong flag format, can't read value after flag: ");
                current_status = U_INVALID_FORMAT;
                return current_status;
            }
        }
        current_status = parse_long_double(argv[i], &current_double, -__LDBL_MAX__, __LDBL_MAX__);
        if (current_status != U_OK) {
            fprintf(stderr, "INFO: can't parse arg with id %ld [%s]. Error code: ", i, argv[i]);
            continue;
        }
        double_vec_push_back(dv, current_double);
    }

    return U_OK;
}