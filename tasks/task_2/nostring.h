#ifndef NOSTRING_H
#define NOSTRING_H


#if defined(__linux__)
static const char *errors[] = {
    "Success",
    "Operation not permitted",
    "No such file or directory",
    "No such process",
    "Interrupted system call",
    "I/O error",
    "No such device or address",
    "Argument list too long",
    "Exec format error",
    "Bad file number"
};
#elif defined(__APPLE__)
static const char *errors[] = {
    "Undefined error: 0",
    "Operation not permitted",
    "No such file or directory",
    "No such process",
    "Interrupted system call",
    "Input/output error",
    "Device not configured",
    "Argument list too long",
    "Exec format error",
    "Bad file descriptor"
};
#else
static const char *errors[] = {"Unknown OS"};
#endif

#include "nostring.h"

#include <stddef.h>
#include <stdlib.h>

void *memchr(const void *str, int c, size_t n);
int memcmp(const void *str1, const void *str2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *str, int c, size_t n);
char *strncat(char *dest, const char *src, size_t n);
char *strchr(const char *str, int c);
int strncmp(const char *str1, const char *str2, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
size_t strcspn(const char *str1, const char *str2);
char *strerror(int errnum);
size_t strlen(const char *str);
char *strpbrk(const char *str1, const char *str2);
char *strrchr(const char *str, int c);
char *strstr(const char *haystack, const char *needle);
char *strtok(char *str, const char *delim);

#endif