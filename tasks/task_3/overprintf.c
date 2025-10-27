#include "overprintf.h"
#include "helpers.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <limits.h>


void register_custom_format(const char *flag, custom_formatter_t handler) {
    if (custom_flag_count < 32) {
        custom_flags[custom_flag_count].flag = flag;
        custom_flags[custom_flag_count].handler = handler;
        custom_flag_count++;
    }
}

int handle_Ro(char *buffer, size_t size, va_list *args) {
    int num = va_arg(*args, int);
    if (int_to_roman(num, buffer, size) == 0)
        return (int)strlen(buffer);
    return 0;
}

int handle_Zr(char *buffer, size_t size, va_list *args) {
    unsigned int num = va_arg(*args, unsigned int);
    if (uint_to_zeckendorf(num, buffer, size) == 0)
        return (int)strlen(buffer);
    return 0;
}

int handle_Cv(char *buffer, size_t size, va_list *args) {
    int base = va_arg(*args, int);
    int num = va_arg(*args, int);
    if (int_to_base(num, base, buffer, size, 0) == 0)
        return (int)strlen(buffer);
    return 0;
}

int handle_CV(char *buffer, size_t size, va_list *args) {
    int base = va_arg(*args, int);
    int num = va_arg(*args, int);
    if (int_to_base(num, base, buffer, size, 1) == 0)
        return (int)strlen(buffer);
    return 0;
}

int handle_to(char *buffer, size_t size, va_list *args) {
    const char *str = va_arg(*args, const char *);
    int base = va_arg(*args, int);
    int result;
    if (base_to_decimal(str, base, 0, &result) == 0)
        snprintf(buffer, size, "%d", result);
    return (int)strlen(buffer);
}

int handle_TO(char *buffer, size_t size, va_list *args) {
    const char *str = va_arg(*args, const char *);
    int base = va_arg(*args, int);
    int result;
    if (base_to_decimal(str, base, 1, &result) == 0)
        snprintf(buffer, size, "%d", result);
    return (int)strlen(buffer);
}

int handle_mi(char *buffer, size_t size, va_list *args) {
    int v = va_arg(*args, int);
    memory_dump(&v, sizeof(int), buffer, size);
    return (int)strlen(buffer);
}

int handle_mu(char *buffer, size_t size, va_list *args) {
    unsigned int v = va_arg(*args, unsigned int);
    memory_dump(&v, sizeof(unsigned int), buffer, size);
    return (int)strlen(buffer);
}

int handle_md(char *buffer, size_t size, va_list *args) {
    double v = va_arg(*args, double);
    memory_dump(&v, sizeof(double), buffer, size);
    return (int)strlen(buffer);
}

int handle_mf(char *buffer, size_t size, va_list *args) {
    float v = (float)va_arg(*args, double);
    memory_dump(&v, sizeof(float), buffer, size);
    return (int)strlen(buffer);
}

int oversvprintf(char *out, const char *format, va_list args) {
    char *buf = out;
    const char *p = format;

    while (*p) {
        const char *percent = strchr(p, '%');
        if (!percent) {
            strcpy(buf, p);
            buf += strlen(p);
            break;
        }

        size_t prefix_len = percent - p;
        memcpy(buf, p, prefix_len);
        buf += prefix_len;
        p = percent + 1;

        int handled = 0;
        for (int i = 0; i < custom_flag_count; ++i) {
            size_t len = strlen(custom_flags[i].flag);
            if (strncmp(p, custom_flags[i].flag, len) == 0) {
                char temp[256];
                int n = custom_flags[i].handler(temp, sizeof(temp), args);

                memcpy(buf, temp, n);
                buf += n;
                p += len;
                handled = 1;
                break;
            }
        }

        if (!handled) {
            const char *next = p;
            while (*next && strchr("diufFeEgGxXoscpn%", *next) == NULL) next++;
            if (*next) next++;

            size_t len = next - percent;
            char spec[64];
            strncpy(spec, percent, len);
            spec[len] = '\0';

            char temp[512];
            int n = vsnprintf(temp, sizeof(temp), spec, args);

            memcpy(buf, temp, n);
            buf += n;
            p = next;
        }
    }

    *buf = '\0';
    return (int)(buf - out);
}


int oversprintf(char *out, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = oversvprintf(out, format, args);
    va_end(args);
    return result;
}

int overfprintf(FILE *stream, const char *format, ...) {
    char buffer[4096];
    va_list args;
    va_start(args, format);
    int n = oversvprintf(buffer, format, args);
    va_end(args);
    return fwrite(buffer, 1, n, stream);
}


void register_default_formats(void) {
    register_custom_format("Ro", handle_Ro);
    register_custom_format("Zr", handle_Zr);
    register_custom_format("Cv", handle_Cv);
    register_custom_format("CV", handle_CV);
    register_custom_format("to", handle_to);
    register_custom_format("TO", handle_TO);
    register_custom_format("mi", handle_mi);
    register_custom_format("mu", handle_mu);
    register_custom_format("md", handle_md);
    register_custom_format("mf", handle_mf);
}