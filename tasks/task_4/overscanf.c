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

    *input += strlen(token);
    roman_to_int(token, out);
    return 1;
}

int handle_scan_Zr(const char **input, va_list *args) {
    unsigned int *out = va_arg(*args, unsigned int*);
    char token[256];
    if (sscanf(*input, "%255s", token) != 1) return 0;

    *input += strlen(token);
    zeckendorf_to_uint(token, out);
    return 1;
}

int handle_scan_Cv(const char **input, va_list *args) {
    int base = va_arg(*args, int);
    int *out = va_arg(*args, int*);
    if (base < 2 || base > 36) base = 10;

    char token[128];
    if (sscanf(*input, "%127s", token) != 1) return 0;
    *input += strlen(token);
    base_to_decimal(token, base, 0, out);
    return 1;
}

int handle_scan_CV(const char **input, va_list *args) {
    int base = va_arg(*args, int);
    int *out = va_arg(*args, int*);
    if (base < 2 || base > 36) base = 10;

    char token[128];
    if (sscanf(*input, "%127s", token) != 1) return 0;
    *input += strlen(token);
    base_to_decimal(token, base, 1, out);
    return 1;
}

int oversvsscanf(const char *input, const char *format, va_list args) {
    const char *p = format;
    int assigned = 0;

    while (*p) {
        const char *percent = strchr(p, '%');
        if (!percent) break;
        p = percent + 1;

        int handled = 0;

        for (int i = 0; i < custom_scanflag_count; ++i) {
            size_t len = strlen(custom_scanflags[i].flag);
            if (strncmp(p, custom_scanflags[i].flag, len) == 0) {
                if (custom_scanflags[i].handler(&input, args))
                    assigned++;
                p += len;
                handled = 1;
                break;
            }
        }

        if (!handled) {
            const char *next = p;
            while (*next && strchr("diufFeEgGxXoscp%", *next) == NULL) next++;
            if (*next) next++;

            size_t len = next - percent;
            char spec[64];
            strncpy(spec, percent, len);
            spec[len] = '\0';

            printf("1 | format: \"%s\" string: \"%s\"\n", spec, input);

            int n = vsscanf(input, spec, args);
            va_arg(args, void*);

            if (n == 1)
                assigned++;

            const char *fmt_part = percent;
            const char *inp_part = input;

            const char *after_percent = strchr(fmt_part, '%');
            if (after_percent) {
                after_percent++;
                while (*after_percent && strchr("diufFeEgGxXoscp%", *after_percent) == NULL)
                    after_percent++;
                if (*after_percent) after_percent++;
            } else {
                after_percent = fmt_part;
            }

            while (*inp_part && *after_percent && *inp_part != *after_percent)
                inp_part++;

            while (*inp_part && *after_percent && *after_percent != '%') {
                if (*inp_part != *after_percent)
                    break;
                inp_part++;
                after_percent++;
            }

            printf("2 | format: \"%s\" string: \"%s\"\n", fmt_part, inp_part);
            input = inp_part;
            p = next;
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
