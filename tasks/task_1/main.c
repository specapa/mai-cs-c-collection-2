#include <stdio.h>
#include <stdlib.h>
#include "../../include/utils_2.h"
#include "parser.h"
#include "task_math.h"

#define SOURCE_BASE_DEFAULT 10
#define MAX_SEARCH_DEEP_DEFAULT 256

void using(FILE *stream, char *run_command) {
    fprintf(stream, "Using: %s <arg_1> ... <arg_n>\n"
    "FLAGS:\n"
    "[-b] -- The base for the transfer. (2 <= x <= %hu)\n"
    "[-s] -- The source base (2 <= x <= 10)\n"
    "[-l] -- The maximum calculated digit for check after the transfer\n", run_command, USHRT_MAX);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        using(stderr, argv[0]);
        return 115;
    }

    // Initialize data variables
    u_status_t status;

    usgshrt_vec_t *new_base_digits = NULL; 
    status = usgshrt_vec_init(&new_base_digits); 
    if (status != U_OK) { 
        fprintf(stderr, "ERROR at init usgshrt_vec_t: "); 
        u_status_print(stderr, status, u_status_to_string); 
        usgshrt_vec_free(&new_base_digits);
        return 13; 
    }

    usgshrt_vec_vec_t *parsed_numbers = NULL; 
    status = usgshrt_vec_vec_init(&parsed_numbers); 
    if (status != U_OK) { 
        fprintf(stderr, "ERROR at init usgshrt_vec_vec_t: "); 
        u_status_print(stderr, status, u_status_to_string); 
        usgshrt_vec_vec_free(&parsed_numbers);
        usgshrt_vec_free(&new_base_digits);
        return 14; 
    }

    unsigned long long target_base = 0;
    unsigned long long source_base = 0;
    unsigned long long max_deep = 0;
    
    status = parse_arg_vector(argc, argv, parsed_numbers, &source_base, &target_base, &max_deep);
    if (status != U_OK) {
        usgshrt_vec_free(&new_base_digits);
        usgshrt_vec_vec_free(&parsed_numbers);
        return 100;
    }

    source_base = source_base != 0 ? source_base : SOURCE_BASE_DEFAULT;
    max_deep = max_deep != 0 ? max_deep : MAX_SEARCH_DEEP_DEFAULT;

    if (target_base < 2) {
        using(stderr, argv[0]);
        for (size_t i = 0; i < parsed_numbers->size; ++i) usgshrt_vec_free(&(parsed_numbers->vec[i]));
        usgshrt_vec_vec_free(&parsed_numbers);
        usgshrt_vec_free(&new_base_digits);
        return 101;
    }

    if (source_base < 2) {
        using(stderr, argv[0]);
        for (size_t i = 0; i < parsed_numbers->size; ++i) usgshrt_vec_free(&(parsed_numbers->vec[i]));
        usgshrt_vec_vec_free(&parsed_numbers);
        usgshrt_vec_free(&new_base_digits);
        return 102;
    }

    rep_state_t rep = 0;
    size_t c_shift = 0;
    for (size_t i = 0; i < parsed_numbers->size; ++i) {
        status = has_a_finite_representation(parsed_numbers->vec[i], source_base, target_base, max_deep, &rep, &new_base_digits, &c_shift);
        if (status != U_OK) {
            write_digits(stderr, parsed_numbers->vec[i]);
            fprintf(stderr, " skipped due %s\n", u_status_to_string(status));
            continue;
        }

        fprintf(stdout, "0.");
        write_digits(stdout, parsed_numbers->vec[i]);
        fprintf(stdout, " in %llu base", source_base);

        if (rep == REP_FINITE) {
            fprintf(stdout, " has a finite representation in %llu base: 0.", target_base);
            write_digits(stdout, new_base_digits);
        } else if (rep == REP_INFINITE) {
            fprintf(stdout, " hasn't a finite representation in %llu base: 0.", target_base);
            write_digits_with_cycle(stdout, new_base_digits, c_shift);
        } else {
            fprintf(stdout, " can't be checked and maybe has infinity represintation in %llu base: 0.", target_base);
            write_digits(stdout, new_base_digits);
        }
        putc('\n', stdout);
    }

    for (size_t i = 0; i < parsed_numbers->size; ++i) usgshrt_vec_free(&(parsed_numbers->vec[i]));
    usgshrt_vec_vec_free(&parsed_numbers);
    usgshrt_vec_free(&new_base_digits);

    return 0;
}