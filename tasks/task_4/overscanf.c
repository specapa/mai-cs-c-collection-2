#include "overscanf.h"
#include "helpers.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <limits.h>


int handle_scan_Ro(const char **input, va_list *args) {
    int *out = va_arg(*args, int*);
    
    char token[128];
    if (sscanf(*input, "%127s", token) != 1) return 0;

    int mv = roman_to_int(token, out);
    *input += mv;
    return 1;
}

int handle_scan_Zr(const char **input, va_list *args) {
    unsigned int *out = va_arg(*args, unsigned int*);

    char token[256];
    if (sscanf(*input, "%255s", token) != 1) return 0;

    int mv = zeckendorf_to_uint(token, out);
    *input += mv;
    return 1;
}

int handle_scan_Cv(const char **input, va_list *args) {
    int base = va_arg(*args, int);
    int *out = va_arg(*args, int*);
    if (base < 2 || base > 36) base = 10;

    char token[128];
    if (sscanf(*input, "%127s", token) != 1) return 0;

    int mv = base_to_decimal(token, base, 1, out); 
    *input += mv;
    return 1;
}

int handle_scan_CV(const char **input, va_list *args) {
    int base = va_arg(*args, int);
    int *out = va_arg(*args, int*);
    if (base < 2 || base > 36) base = 10;

    char token[128];
    if (sscanf(*input, "%127s", token) != 1) return 0;

    int mv = base_to_decimal(token, base, 1, out); 
    *input += mv;
    return 1;
}

static const char *find_substring_len(const char *hay, const char *needle, size_t nlen) {
    if (nlen == 0) return hay;
    const char *h = hay;
    for (; *h; ++h) {
        size_t rem = strlen(h);
        if (rem < nlen) return NULL;
        if (h[0] == needle[0] && memcmp(h, needle, nlen) == 0)
            return h;
    }
    return NULL;
}

int oversvsscanf(const char *input, const char *format, va_list args) {
    const char *p = format;
    int assigned = 0;

    while (*p && *input) {
        if (*p != '%') {
            if (*p != *input) break;
            p++; input++;
            continue;
        }

        p++;

        int handled = 0;
        for (int i = 0; i < custom_scanflag_count; ++i) {
            size_t flen = strlen(custom_scanflags[i].flag);
            if (strncmp(p, custom_scanflags[i].flag, flen) == 0) {
                const char *fmt_after = p + flen;
                if (custom_scanflags[i].handler(&input, args)) {
                    assigned++;
                } else {
                    return assigned;
                }

                const char *next_pct = strchr(fmt_after, '%');
                size_t lit_len = next_pct ? (size_t)(next_pct - fmt_after) : strlen(fmt_after);

                if (lit_len > 0) {
                    const char *pos = find_substring_len(input, fmt_after, lit_len);
                    if (!pos) {
                        return assigned;
                    }
                    input = pos + lit_len;
                    p = fmt_after + lit_len;
                } else {
                    p = fmt_after;
                }

                handled = 1;
                break;
            }
        }
        if (handled) continue;

        const char *conv_start = p - 1;
        const char *next = p;

        while (*next && strchr("diufFeEgGxXoscp%", *next) == NULL) next++;
        if (*next) next++;
        size_t spec_len = (size_t)(next - conv_start);
        char spec[128];
        if (spec_len >= sizeof(spec)) spec_len = sizeof(spec) - 1;
        memcpy(spec, conv_start, spec_len);
        spec[spec_len] = '\0';

        va_list ap_copy;
        va_copy(ap_copy, args);
        int n = vsscanf(input, spec, ap_copy);
        va_end(ap_copy);

        if (n <= 0) {
            return assigned;
        }

        for (int k = 0; k < n; ++k) {
            (void)va_arg(args, void *);
        }
        assigned += n;

        const char *fmt_after = next;
        const char *next_pct = strchr(fmt_after, '%');
        size_t lit_len = next_pct ? (size_t)(next_pct - fmt_after) : strlen(fmt_after);

        if (lit_len > 0) {
            const char *pos = find_substring_len(input, fmt_after, lit_len);
            if (!pos) {
                return assigned;
            }
            input = pos + lit_len;
            p = fmt_after + lit_len;
        } else {
            p = fmt_after;
        }
    }

    return assigned;
}

int oversscanf(const char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int res = oversvsscanf(str, format, args);
    va_end(args);
    return res;
}

int overfscanf(FILE *stream, const char *format, ...) {
    char buffer[4096];
    if (!fgets(buffer, sizeof(buffer), stream))
        return 0;

    va_list args;
    va_start(args, format);
    int res = oversvsscanf(buffer, format, args);
    va_end(args);
    return res;
}

void register_scanformat(const char *flag, custom_scanner_t handler) {
    if (custom_scanflag_count < 32) {
        custom_scanflags[custom_scanflag_count].flag = flag;
        custom_scanflags[custom_scanflag_count].handler = handler;
        custom_scanflag_count++;
    }
}

void register_default_scanformats(void) {
    register_scanformat("Ro", handle_scan_Ro);
    register_scanformat("Zr", handle_scan_Zr);
    register_scanformat("Cv", handle_scan_Cv);
    register_scanformat("CV", handle_scan_CV);
}
