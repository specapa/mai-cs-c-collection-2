#include "nostring.h"

#include <stddef.h>
#include <stdlib.h>

void *memchr(const void *str, int c, size_t n) {
    const unsigned char *bytes = str;
    unsigned char ch = (unsigned char)c;

    for (size_t i = 0; i < n; ++i) {
        if (bytes[i] == ch)
            return (void *)(bytes + i);
    }
    return NULL;
}


int memcmp(const void *str1, const void *str2, size_t n) {
    const unsigned char *l = str1;
    const unsigned char *r = str2;

    while (n--) {
        if (*l != *r)
            return (int)(*l - *r);
        ++l;
        ++r;
    }
    return 0;
}

void *memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    for (size_t i = 0; i < n; ++i)
        d[i] = s[i];
    return dest;
}

void *memset(void *str, int c, size_t n) {
    unsigned char *bytes = str;
    unsigned char ch = (unsigned char)c;
    for (size_t i = 0; i < n; ++i)
        bytes[i] = ch;
    return str;
}

char *strncat(char *dest, const char *src, size_t n) {
    char *d = dest;
    while (*d) ++d;
    while (n-- && *src) *d++ = *src++;
    *d = '\0';
    return dest;
}

char *strchr(const char *str, int c) {
    unsigned char ch = (unsigned char)c;
    while (*str) {
        if ((unsigned char)*str == ch)
            return (char *)str;
        ++str;
    }
    return (ch == '\0') ? (char *)str : NULL;
}

int strncmp(const char *str1, const char *str2, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        unsigned char a = (unsigned char)str1[i];
        unsigned char b = (unsigned char)str2[i];
        if (a != b || a == '\0' || b == '\0')
            return (int)(a - b);
    }
    return 0;
}

char *strncpy(char *dest, const char *src, size_t n) {
    size_t i = 0;
    for (; i < n && src[i]; ++i)
        dest[i] = src[i];
    for (; i < n; ++i)
        dest[i] = '\0';
    return dest;
}

size_t strcspn(const char *str1, const char *str2) {
    size_t len = 0;
    for (; *str1; ++str1, ++len) {
        for (const char *p = str2; *p; ++p) {
            if (*str1 == *p)
                return len;
        }
    }
    return len;
}

char *strerror(int errnum) {
    int count = (int)(sizeof(errors) / sizeof(errors[0]));
    if (errnum >= 0 && errnum < count)
        return (char *)errors[errnum];
    return (char *)"Unknown error";
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len])
        ++len;
    return len;
}

char *strpbrk(const char *str1, const char *str2) {
    for (; *str1; ++str1) {
        for (const char *p = str2; *p; ++p)
            if (*str1 == *p)
                return (char *)str1;
    }
    return NULL;
}

char *strrchr(const char *str, int c) {
    unsigned char ch = (unsigned char)c;
    const char *last = NULL;
    do {
        if ((unsigned char)*str == ch)
            last = str;
    } while (*str++);
    return (char *)last;
}

char *strstr(const char *haystack, const char *needle) {
    if (!*needle) return (char *)haystack;
    for (; *haystack; ++haystack) {
        const char *h = haystack;
        const char *n = needle;
        while (*h && *n && *h == *n) {
            ++h; ++n;
        }
        if (*n == '\0')
            return (char *)haystack;
    }
    return NULL;
}

char *strtok(char *str, const char *delim) {
    static char *next = NULL;
    if (str)
        next = str;
    if (!next)
        return NULL;

    while (*next && strchr(delim, *next))
        ++next;

    if (!*next)
        return NULL;

    char *start = next;

    while (*next && !strchr(delim, *next))
        ++next;

    if (*next) {
        *next = '\0';
        ++next;
    } else {
        next = NULL;
    }

    return start;
}