#include "task_math.h"
#include <math.h>
#include <unistd.h>

static bool vec_equal(const usgshrt_vec_t *a, const usgshrt_vec_t *b) {
    if (a->size != b->size) return false;
    for (size_t i = 0; i < a->size; ++i)
        if (a->vec[i] != b->vec[i])
            return false;
    return true;
}

u_status_t multiply_fraction(const usgshrt_vec_t *fraction, unsigned long long source_base, unsigned long long target_base, usgshrt_vec_t **new_frac, unsigned short *out_digit) {
    if (!fraction || !new_frac || !out_digit) return U_INVALID_FORMAT;
    if (fraction->size == 0) {
        u_status_t st = usgshrt_vec_init(new_frac);
        if (st != U_OK) return st;
        *out_digit = 0;
        return U_OK;
    }

    usgshrt_vec_t *res = NULL;
    u_status_t status = usgshrt_vec_init(&res);
    if (status != U_OK) return status;

    unsigned long long carry = 0;

    unsigned long long *tmp = malloc(sizeof(unsigned long long) * fraction->size);
    if (!tmp) { usgshrt_vec_free(&res); return U_MEMORY_ALLOCATE_ERROR; }

    for (ssize_t i = (ssize_t)fraction->size - 1; i >= 0; --i) {
        unsigned long long digit = fraction->vec[i];
        unsigned long long val = digit * target_base + carry;
        tmp[i] = val % source_base;
        carry = val / source_base;
    }

    for (size_t i = 0; i < fraction->size; ++i) {
        status = usgshrt_vec_push_back(res, (unsigned short)tmp[i]);
        if (status != U_OK) { 
            free(tmp); usgshrt_vec_free(&res); 
            return status;
        }
    }
    free(tmp);

    if (carry > USHRT_MAX) { 
        usgshrt_vec_free(&res); 
        return U_OVERFLOW; 
    }
    *out_digit = (unsigned short)carry;

    while (res->size > 0 && res->vec[res->size - 1] == 0) {
        res->size--;
    }

    *new_frac = res;
    return U_OK;
}

u_status_t has_a_finite_representation(usgshrt_vec_t *digits10, unsigned long long source_base, unsigned long long target_base, unsigned long long max_deep, rep_state_t *res, usgshrt_vec_t **digits, size_t *sft) {
    if (!digits10 || digits10->size == 0 || target_base < 2)
        return U_INVALID_FORMAT;

    usgshrt_vec_t *seen[max_deep];
    size_t seen_count = 0;

    usgshrt_vec_t *cur_frac = NULL;
    usgshrt_vec_init(&cur_frac);

    for (size_t i = 0; i < digits10->size; ++i)
        usgshrt_vec_push_back(cur_frac, digits10->vec[i]);

    usgshrt_vec_t *result_digits = NULL;
    usgshrt_vec_init(&result_digits);

    for (size_t step = 0; step < max_deep; ++step) {
        for (size_t j = 0; j < seen_count; ++j) {
            if (vec_equal(cur_frac, seen[j])) {
                *res = REP_INFINITE;
                *sft = j;
                if (digits != NULL && *digits != NULL) usgshrt_vec_free(digits);
                *digits = result_digits;
                for (size_t k = 0; k < seen_count; ++k) usgshrt_vec_free(&seen[k]);
                usgshrt_vec_free(&cur_frac);
                return U_OK;
            }
        }

        usgshrt_vec_t *copy = NULL;
        usgshrt_vec_init(&copy);
        for (size_t i = 0; i < cur_frac->size; ++i)
            usgshrt_vec_push_back(copy, cur_frac->vec[i]);
        seen[seen_count++] = copy;

        unsigned short digit;
        usgshrt_vec_t *next_frac = NULL;
        multiply_fraction(cur_frac, source_base, target_base, &next_frac, &digit);

        usgshrt_vec_push_back(result_digits, digit);

        bool all_zero = true;
        for (size_t i = 0; i < next_frac->size; ++i)
            if (next_frac->vec[i] != 0) { all_zero = false; break; }

        if (all_zero) {
            *res = REP_FINITE;
            if (digits && *digits != NULL) usgshrt_vec_free(digits);
            *digits = result_digits;
            for (size_t k = 0; k < seen_count; ++k) usgshrt_vec_free(&seen[k]);
            usgshrt_vec_free(&next_frac);
            usgshrt_vec_free(&cur_frac);
            return U_OK;
        }

        usgshrt_vec_free(&cur_frac);
        cur_frac = next_frac;
    }

    *res = REP_UNKNOWN;
    if (digits && *digits != NULL) {
        usgshrt_vec_free(digits);
    }
    *digits = result_digits;

    for (size_t k = 0; k < seen_count; ++k) usgshrt_vec_free(&seen[k]);
    usgshrt_vec_free(&cur_frac);
    return U_OK;
}

void write_digits(FILE *strm, usgshrt_vec_t *digits) {
    for (size_t i = 0; i < digits->size; ++i) {
        fprintf(strm, "%hu", digits->vec[i]);
    }
}

void write_digits_with_cycle(FILE *strm, usgshrt_vec_t *digits, size_t shift) {
    for (size_t i = 0; i < digits->size; ++i) {
        if (i == shift) {
            putc('(', strm);
        }
        fprintf(strm, "%hu", digits->vec[i]);
    }
    putc(')', strm);
}