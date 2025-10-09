#ifndef UTILS_H
#define UTILS_H

#include <limits.h>
#include <stddef.h>
#include <stdio.h>

typedef enum {
    U_OK = 0,
    U_INVALID_FORMAT,
    U_OVERFLOW,
    U_MEMORY_ALLOCATE_ERROR,
    U_MEMORY_REALLOCATE_ERROR,
    U_NEGATIVE_NOT_ALLOWED,
} u_status_t;

u_status_t parse_signed_ll(const char *s, long long *out_value, long long min_allowed, long long max_allowed);
u_status_t parse_unsigned_ll(const char *s, unsigned long long *out_value, unsigned long long min_allowed, unsigned long long max_allowed);
u_status_t parse_double(const char *s, double *out_value, double min_allowed, double max_allowed);
u_status_t read_string(FILE *fp, char **out);

const char *u_status_to_string(int st);
void print_error_prefix(FILE *fp);
void u_status_print(FILE *fp, u_status_t st, const char *(*error_description)(int));

#endif /* UTILS_H */
