#ifndef OVERSCANF_H
#define OVERSCANF_H

#include <stdio.h>
#include <stdarg.h>

typedef int (*custom_scanner_t)(const char **input, va_list *args);

typedef struct {
    const char *flag;
    custom_scanner_t handler;
} custom_scanflag_t;

static custom_scanflag_t custom_scanflags[32];
static int custom_scanflag_count = 0;

/**
 * %Ro — римское представление int
 * %Zr — цекендорфово представление unsigned int
 * %Cv — число в системе счисления (base, value), строчные буквы
 * %CV — число в системе счисления (base, value), заглавные буквы
 */
int oversvsscanf(const char *input, const char *format, va_list args);
int oversscanf(const char *str, const char *format, ...);
int overfscanf(FILE *stream, const char *format, ...);

int handle_scan_Ro(const char **input, va_list *args);
int handle_scan_Zr(const char **input, va_list *args);
int handle_scan_Cv(const char **input, va_list *args);
int handle_scan_CV(const char **input, va_list *args);

void register_default_scanformats(void);

#endif /* OVERSCANF_H */
