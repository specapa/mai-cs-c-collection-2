#include <stdio.h>
#include <stdlib.h>
#include "../../include/utils_2.h"
#include "parser.h"
#include "task_math.h"

void using(FILE *stream, char *run_command) {
    fprintf(stream, "Using: %s -b <base> <arg_1> ... <arg_n>\n", run_command);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        using(stderr, argv[0]);
        return 1152;
    }

    // Initialize data variables
    u_status_t status;
    usgshrt_vec_t *usv = NULL; status = usgshrt_vec_init(&usv); 
    if (status != U_OK) { fprintf(stderr, "Error at init usgshrt_vec_t: "); u_status_print(stderr, status, u_status_to_string); }
    double_vec_t *dv = NULL; status = double_vec_init(&dv); 
    if (status != U_OK) { fprintf(stderr, "Error at init double_vec_t: "); u_status_print(stderr, status, u_status_to_string); }
    unsigned long long base = 0;
    
    status = parse_arg_vector(argc, argv, dv, &base);
    if (status != U_OK) {
        u_status_print(stderr, status, u_status_to_string);
        return 1000;
    }
    if (base == 0) {
        using(stderr, argv[0]);
        return 1001;
    }

    rep_state_t rep = 0;
    for (size_t i = 0; i < dv->size; ++i) {
        status = has_a_finite_representation(dv->vec[i], base, &rep, &usv);
        if (status != U_OK) {
            u_status_print(stderr, status, u_status_to_string);
            fprintf(stderr, "%Lf skipped...\n", dv->vec[i]);
            continue;
        }
        if (rep == REP_FINITE) {
            fprintf(stdout, "%Lf has a finite representation: .", dv->vec[i]);
            write_digits(stdout, usv);
            putc('\n', stdout);
        } else if (rep == REP_INFINITE) {
            fprintf(stdout, "%Lf hasn't a finite representation: .(", dv->vec[i]);
            write_digits(stdout, usv);
            fprintf(stdout, ")\n");
        } else {
            fprintf(stdout, "%Lf can't be checked and maybe has infinity represintation: ", dv->vec[i]);
            write_digits(stdout, usv);
            putc('\n', stdout);
        }
    }
    return 0;
}