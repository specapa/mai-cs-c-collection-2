#ifndef HELPERS_H
#define HELPERS_H

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int int_to_roman(int num, char *buffer, int buf_size);
int uint_to_zeckendorf(unsigned int num, char *buffer, int buf_size);
int int_to_base(int num, int base, char *buffer, int buf_size, int uppercase);
int base_to_decimal(const char *str, int base, int uppercase, int *result);
int memory_dump(const void *ptr, int size, char *buffer, int buf_size);

#endif