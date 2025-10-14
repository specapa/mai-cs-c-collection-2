#include "utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

const char *u_status_to_string(int st) {
    switch (st) {
        case U_OK:                     return "OK";
        case U_INVALID_FORMAT:         return "Invalid format";
        case U_OVERFLOW:               return "Overflow";
        case U_NEGATIVE_NOT_ALLOWED:   return "Negative value not allowed";
        case U_MEMORY_ALLOCATE_ERROR:  return "Memory allocation failed";
        case U_MEMORY_REALLOCATE_ERROR:return "Memory reallocation failed";
        default:                       return "Unknown error";
    }
}

void print_error_prefix(FILE *fp) {
    fprintf(fp, "Error: ");
}

void u_status_print(FILE *fp, u_status_t st, const char *(*error_description)(int)) {
    if (!fp) fp = stderr;
    fprintf(fp, "%s\n", error_description(st));
}

u_status_t parse_signed_ll(const char *s, long long *out_value, long long min_allowed, long long max_allowed) {
    if (s == NULL || out_value == NULL) return U_INVALID_FORMAT;
    const char *p = s;
    while (isspace((unsigned char)*p)) ++p;
    int neg = 0;
    if (*p == '+' || *p == '-') {
        if (*p == '-') neg = 1;
        ++p;
    }
    if (!isdigit((unsigned char)*p)) return U_INVALID_FORMAT;
    unsigned long long acc = 0ULL;
    while (*p) {
        if (!isdigit((unsigned char)*p)) return U_INVALID_FORMAT;
        int d = *p - '0';
        if (acc > (ULLONG_MAX / 10ULL)) return U_OVERFLOW;
        acc = acc * 10ULL + (unsigned long long)d;
        ++p;
    }
    long long val;
    if (neg) {
        unsigned long long max_mag = (unsigned long long)LLONG_MAX + 1ULL; /* == -(LLONG_MIN) */
        if (acc > max_mag) return U_OVERFLOW;
        if (acc == max_mag) {
            val = LLONG_MIN;
        } else {
            val = -(long long)acc;
        }
    } else {
        if (acc > (unsigned long long)LLONG_MAX) return U_OVERFLOW;
        val = (long long)acc;
    }
    if (val < min_allowed || val > max_allowed) return U_INVALID_FORMAT;
    *out_value = val;
    return U_OK;
}

u_status_t parse_unsigned_ll(const char *s, unsigned long long *out_value, unsigned long long min_allowed, unsigned long long max_allowed) {
    if (s == NULL || out_value == NULL) return U_INVALID_FORMAT;
    const char *p = s;
    while (isspace((unsigned char)*p)) ++p;
    int neg = 0;
    if (*p == '+' || *p == '-') {
        if (*p == '-') return U_INVALID_FORMAT;
        ++p;
    }
    if (!isdigit((unsigned char)*p)) return U_INVALID_FORMAT;
    unsigned long long acc = 0ULL;
    while (*p) {
        if (!isdigit((unsigned char)*p)) return U_INVALID_FORMAT;
        int d = *p - '0';
        if (acc > (ULLONG_MAX / 10ULL)) return U_OVERFLOW;
        acc = acc * 10ULL + (unsigned long long)d;
        ++p;
    }
    if (acc < min_allowed || acc > max_allowed) return U_INVALID_FORMAT;
    *out_value = acc;
    return U_OK;
}

u_status_t parse_double(const char *s, double *out_value, double min_allowed, double max_allowed) {
    if (s == NULL || out_value == NULL) return U_INVALID_FORMAT;
    const char *p = s;
    while (isspace((unsigned char)*p)) ++p;
    int neg = 0;
    if (*p == '+' || *p == '-') {
        if (*p == '-') neg = 1;
        ++p;
    }
    if (!isdigit((unsigned char)*p) && *p != '.') return U_INVALID_FORMAT;

    unsigned long long int_part = 0ULL;
    int int_digits = 0;
    while (isdigit((unsigned char)*p)) {
        int d = *p - '0';
        if (int_part > ULLONG_MAX / 10ULL) return U_OVERFLOW;
        int_part = int_part * 10ULL + (unsigned long long)d;
        ++p;
        int_digits = 1;
    }

    double frac_part = 0.0;
    double frac_scale = 1.0;
    if (*p == '.') {
        ++p;
        if (!isdigit((unsigned char)*p) && !int_digits)
            return U_INVALID_FORMAT;
        while (isdigit((unsigned char)*p)) {
            int d = *p - '0';
            frac_scale /= 10.0;
            frac_part += d * frac_scale;
            ++p;
        }
    }

    int exp_sign = 1;
    long exp_val = 0;
    if (*p == 'e' || *p == 'E') {
        ++p;
        if (*p == '+' || *p == '-') {
            if (*p == '-') exp_sign = -1;
            ++p;
        }
        if (!isdigit((unsigned char)*p)) return U_INVALID_FORMAT;
        while (isdigit((unsigned char)*p)) {
            int d = *p - '0';
            if (exp_val > LONG_MAX / 10) return U_OVERFLOW;
            exp_val = exp_val * 10 + d;
            ++p;
        }
    }

    while (isspace((unsigned char)*p)) ++p;
    if (*p != '\0') return U_INVALID_FORMAT;

    double value = (double)int_part + frac_part;
    if (neg) value = -value;

    if (exp_val != 0) {
        double pow10 = pow(10.0, exp_sign * exp_val);
        if (!isfinite(pow10)) return U_OVERFLOW;
        value *= pow10;
        if (!isfinite(value)) return U_OVERFLOW;
    }

    if (value < min_allowed || value > max_allowed)
        return U_INVALID_FORMAT;

    *out_value = value;
    return U_OK;
}

u_status_t read_string(FILE *fp, char **out) {
    if (!fp || !out) return U_INVALID_FORMAT;
    size_t size = 16;
    size_t len = 0;
    char *buffer = malloc(size);
    if (!buffer) return U_MEMORY_ALLOCATE_ERROR;
    int c;
    while ((c = fgetc(fp)) != EOF && c != '\n') {
        buffer[len++] = (char)c;
        if (len >= size) {
            size *= 2;
            char *new_buffer = realloc(buffer, size);
            if (!new_buffer) {
                free(buffer);
                return U_MEMORY_REALLOCATE_ERROR;
            }
            buffer = new_buffer;
        }
    }
    if (len == 0 && c == EOF) {
        free(buffer);
        return U_INVALID_FORMAT;
    }
    buffer[len] = '\0';
    *out = buffer;
    return U_OK;
}