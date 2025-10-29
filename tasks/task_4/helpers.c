#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#define MAX_FIB_LEN 47

static int strcicmp(char const *a, char const *b, int n)
{
    for (int i = 0; i < n; i++, a++, b++) {
        int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (d != 0 || !*a)
            return d;
    }
    return 0;
}

int roman_to_int(const char *roman, int *out) {
    if (!roman || !out) {
        #ifdef OVERSCAN_WARNINGS
            fprintf(stderr, "Warning: NULL pointer at roman_to_int()\n");
        #endif 
        return -1;
    }

    struct {
        const char *sym;
        int val;
    } romans[] = {
        {"M", 1000}, {"CM", 900}, {"D", 500}, {"CD", 400},
        {"C", 100},  {"XC", 90},  {"L", 50},  {"XL", 40},
        {"X", 10},   {"IX", 9},   {"V", 5},   {"IV", 4}, {"I", 1}
    };

    int result = 0;
    size_t i = 0;

    while (roman[i]) {
        int matched = 0;

        for (int j = 0; j < 13; j++) {
            size_t len = strlen(romans[j].sym);
            if (strcicmp(&roman[i], romans[j].sym, len) == 0) {
                result += romans[j].val;
                i += len;
                matched = 1;
                break;
            }
        }

        if (!matched) {
            *out = (int)result;
            return i;
        }
    }

    *out = (int)result;
    return i;
}

int zeckendorf_to_uint(const char *str, unsigned int *out) {
    if (!str || !out) {
        #ifdef OVERSCAN_WARNINGS
            fprintf(stderr, "Warning AT zeckendorf_to_uint: !str || !out\n");
        #endif 
        return -1;
    }

    size_t len = 0;
    for (const char *c = str; *c == '1' || *c == '0'; ++c) ++len;
    
    if (len == 0) {
        #ifdef OVERSCAN_WARNINGS
            fprintf(stderr, "Warning AT zeckendorf_to_uint: len == 0\n");
        #endif 
        return -1;
    }
    if (len > MAX_FIB_LEN) {
        #ifdef OVERSCAN_WARNINGS
            fprintf(stderr, "Warning AT zeckendorf_to_uint: len > MAX_FIB_LEN\n");
        #endif 
        return -1;
    }

    unsigned int fib[MAX_FIB_LEN];
    fib[0] = 1;
    if (len > 1) {
        fib[1] = 2;
    }
    for (size_t i = 2; i < len; i++) {
        if (fib[i - 1] > (unsigned int)(-1) - fib[i - 2]) {
            #ifdef OVERSCAN_WARNINGS
                fprintf(stderr, "Warning: zeckendorf generation OVERFLOW\n");
            #endif 
            return -1;
        }
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    unsigned int result = 0;
    int last_was_one = 0;

    for (size_t i = 0; i < len; i++) {
        char c = str[i];
        
        if (c == '1') {
            if (last_was_one) {
                #ifdef OVERSCAN_WARNINGS
                    fprintf(stderr, "Warning: zeckendorf WRONG FORMAT\n");
                #endif 
                return -1;
            } 
            last_was_one = 1;
            if (result > (unsigned int)(-1) - fib[len - 1 - i]) {
                #ifdef OVERSCAN_WARNINGS
                    fprintf(stderr, "Warning: zeckendorf OVERFLOW\n");
                #endif
                return -1;
            }
            result += fib[len - 1 - i];
        } else if (c == '0') {
            last_was_one = 0;
        } else {
            break;
        }
    }

    *out = result;
    return (int)len;
}

int base_to_decimal(const char *str, int base, int uppercase, int *out) {
    if (!str || !out) return -1;
    if (base < 2 || base > 36) base = 10;

    const char *ptr = str;
    int is_negative = 0;

    while (*ptr == ' ' || *ptr == '\t') ptr++;
    // while (*ptr != '\0' && !isalpha(*ptr) && !isdigit(*ptr)) ptr++;

    if (*ptr == '-') { is_negative = 1; ptr++; }
    else if (*ptr == '+') ptr++;

    long long value = 0;
    int has_digits = 0;

    while (*ptr) {
        char c = *ptr++;
        if (isspace((unsigned char)c)) continue;

        if (uppercase)
            c = toupper((unsigned char)c);
        else
            c = tolower((unsigned char)c);

        int digit;
        if (c >= '0' && c <= '9')
            digit = c - '0';
        else if (c >= 'a' && c <= 'z')
            digit = c - 'a' + 10;
        else if (c >= 'A' && c <= 'Z')
            digit = c - 'A' + 10;
        else
            break;
            // return -1;

        if (digit >= base) return -1;

        if (value > (LLONG_MAX - digit) / base) {
            #ifdef OVERSCAN_WARNINGS
                    fprintf(stderr, "Warning: base lower then digit WRONG FORMAT\n");
            #endif 
            return -1;
        }

        value = value * base + digit;
        ++has_digits;
    }

    if (!has_digits) return -1;
    if (is_negative) value = -value;

    *out = (int)value;
    return has_digits;
}
