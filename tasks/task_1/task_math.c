#include "task_math.h"
#include <math.h>

#define REPRESINTATION_EPS 1.0E-12
#define REPRESINTATION_MAX_DEPTH 256

static u_status_t has_a_value(double_vec_t *dv, long double dbl, bool *blr) {
    if (dv == NULL || blr == NULL) return U_INVALID_FORMAT;
    *blr = false;
    for (size_t i = 0; i < dv->size; ++i) {
        if (fabs(dv->vec[i] - dbl) < REPRESINTATION_EPS) {
            *blr = true;
            return U_OK;
        }
    }
    return U_OK;
}

u_status_t has_a_finite_representation(long double dbl, unsigned long long base, rep_state_t *res, usgshrt_vec_t **digits) {
    
    double_vec_t *dv = NULL;
    usgshrt_vec_t *usgshrtv = NULL;
    u_status_t status;

    status = double_vec_init(&dv);
    if (status != U_OK) return status;
    status = usgshrt_vec_init(&usgshrtv);
    if (status != U_OK) { double_vec_free(&dv); return status; }
    
    long double integer;
    dbl = modfl(dbl, &integer);
    bool is_cycled = false;

    for (size_t i = 0; i < REPRESINTATION_MAX_DEPTH; ++i) {
        dbl = modfl(dbl * base, &integer);

        status = has_a_value(dv, dbl, &is_cycled);
        if (status != U_OK) { double_vec_free(&dv); return status; }

        if (fabsl(dbl) < REPRESINTATION_EPS) {
            *res = REP_FINITE;
            *digits = usgshrtv;
            double_vec_free(&dv);
            return U_OK;
        }

        if (is_cycled) {
            *res = REP_INFINITE;
            *digits = usgshrtv;
            double_vec_free(&dv);
            return U_OK;
        }

        status = double_vec_push_back(dv, dbl);
        if (status != U_OK) { double_vec_free(&dv); return status; }
        status = usgshrt_vec_push_back(usgshrtv, (unsigned short)integer);
        if (status != U_OK) { double_vec_free(&dv); return status; }
    }

    *res = REP_UNKNOWN;
    *digits = usgshrtv;
    double_vec_free(&dv);
    return U_OK;
}

void write_digits(FILE *strm, usgshrt_vec_t *digits) {
    for (size_t i = 0; i < digits->size; ++i) {
        fprintf(strm, "%hu", digits->vec[i]);
    }
}