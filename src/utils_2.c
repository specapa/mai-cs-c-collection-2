#include "utils_2.h"

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>

u_status_t parse_long_double(const char *s, long double *out_value, long double min_allowed, long double max_allowed) {
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

    long double frac_part = 0.0L;
    long double frac_scale = 1.0L;
    if (*p == '.') {
        ++p;
        if (!isdigit((unsigned char)*p) && !int_digits)
            return U_INVALID_FORMAT;
        while (isdigit((unsigned char)*p)) {
            int d = *p - '0';
            frac_scale /= 10.0L;
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

    long double value = (long double)int_part + frac_part;
    if (neg) value = -value;

    if (exp_val != 0) {
        long double pow10 = powl(10.0L, exp_sign * exp_val);
        if (!isfinite(pow10)) return U_OVERFLOW;
        value *= pow10;
        if (!isfinite(value)) return U_OVERFLOW;
    }

    if (value < min_allowed || value > max_allowed)
        return U_INVALID_FORMAT;

    *out_value = value;
    return U_OK;
}
