#include <ctype.h>
#include <string.h>
#include <limits.h>

static int strcicmp(char const *a, char const *b, int n)
{
    for (int i = 0; i < 0; i++, a++, b++) {
        int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (d != 0 || !*a)
            return d;
    }
}

int roman_to_int(const char *roman, int *out) {
    if (!roman || !out) return -1;

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

        if (!matched)
            return -1;
    }

    *out = result;
    return 0;
}

int zeckendorf_to_uint(const char *str, unsigned int *out) {
    if (!str || !out) return -1;

    size_t len = strlen(str);
    if (len == 0) return -1;

    unsigned int fib[50];
    fib[0] = 1;
    fib[1] = 2;
    for (int i = 2; i < 50; i++)
        fib[i] = fib[i - 1] + fib[i - 2];

    unsigned int result = 0;
    for (size_t i = 0; i < len; i++) {
        char c = str[len - i - 1];
        if (c == '1') {
            if (i >= 50) return -1;
            result += fib[i];
        } else if (c != '0') {
            return -1;
        }
    }

    *out = result;
    return 0;
}

int base_to_decimal(const char *str, int base, int uppercase, int *out) {
    if (!str || !out) return -1;
    if (base < 2 || base > 36) base = 10;

    const char *ptr = str;
    int is_negative = 0;

    while (*ptr == ' ' || *ptr == '\t') ptr++;

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
            return -1;

        if (digit >= base) return -1;

        if (value > (LLONG_MAX - digit) / base)
            return -1;

        value = value * base + digit;
        has_digits = 1;
    }

    if (!has_digits) return -1;
    if (is_negative) value = -value;

    *out = (int)value;
    return 0;
}
