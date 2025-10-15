#ifndef TASK_MATH_H
#define TASK_MATH_H

#include <stdbool.h>
#include "../../include/utils_2.h"
#include "vectors.h"

typedef enum {
    REP_INFINITE = 0,
    REP_FINITE = 1,
    REP_UNKNOWN = 2
} rep_state_t;

u_status_t has_a_finite_representation(usgshrt_vec_t *digits10, unsigned long long source_base, unsigned long long target_base, unsigned long long max_deep, rep_state_t *res, usgshrt_vec_t **digits, size_t *sft);
u_status_t multiply_fraction(const usgshrt_vec_t *fraction, unsigned long long source_base, unsigned long long target_base, usgshrt_vec_t **new_frac, unsigned short *out_digit);
void write_digits(FILE *strm, usgshrt_vec_t *digits);
void write_digits_with_cycle(FILE *strm, usgshrt_vec_t *digits, size_t shift);

#endif // TASK_MATH_H