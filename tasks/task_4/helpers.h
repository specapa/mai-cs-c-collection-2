#ifndef NUMCONVERT_H
#define NUMCONVERT_H

#include <stddef.h>
#include <limits.h>

int roman_to_int(const char *roman, int *out);
int zeckendorf_to_uint(const char *str, unsigned int *out);
int base_to_decimal(const char *str, int base, int uppercase, int *out);

#endif /* NUMCONVERT_H */
