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

u_status_t has_a_finite_representation(long double dbl, unsigned long long base, rep_state_t *res, usgshrt_vec_t **digits);
void write_digits(FILE *strm, usgshrt_vec_t *digits);

#endif // TASK_MATH_H