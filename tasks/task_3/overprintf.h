#ifndef OVERPRINTF_H
#define OVERPRINTF_H

#include <stdio.h>
#include <stdarg.h>

typedef int (*custom_formatter_t)(char *buffer, size_t size, va_list *args);

typedef struct {
    const char *flag;
    custom_formatter_t handler;
} custom_flag_t;

static custom_flag_t custom_flags[32];
static int custom_flag_count = 0;

/**
 * %Ro - римское представление int
 * %Zr - представление Цекендорфа для unsigned int
 * %Cv - число в системе счисления (base, value) - строчные буквы
 * %CV - число в системе счисления (base, value) - заглавные буквы
 * %to - перевод строки в decimal (string, base) - строчные буквы
 * %TO - перевод строки в decimal (string, base) - заглавные буквы
 * %mi - дамп памяти signed int (4 байта)
 * %mu - дамп памяти unsigned int (4 байта)
 * %md - дамп памяти double
 * %mf - дамп памяти float
 */
int oversvprintf(char *out, const char *format, va_list args);
int oversprintf(char *out, const char *format, ...);
int overfprintf(FILE *stream, const char *format, ...);

int handle_Ro(char *buffer, size_t size, va_list *args);
int handle_Zr(char *buffer, size_t size, va_list *args);
int handle_Cv(char *buffer, size_t size, va_list *args);
int handle_CV(char *buffer, size_t size, va_list *args);

int handle_to(char *buffer, size_t size, va_list *args);
int handle_TO(char *buffer, size_t size, va_list *args);

int handle_mi(char *buffer, size_t size, va_list *args);
int handle_mu(char *buffer, size_t size, va_list *args);
int handle_md(char *buffer, size_t size, va_list *args);
int handle_mf(char *buffer, size_t size, va_list *args);

void register_default_formats(void);

#endif