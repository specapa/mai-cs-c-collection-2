#include "helpers.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <limits.h>


int int_to_roman(int num, char *buffer, int buf_size) {
    if (!buffer || buf_size <= 0) return -1;
    
    if (num <= 0 || num >= 4000) {
        buffer[0] = '\0';
        return -1;
    }
    
    const char *romans[] = {
        "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"
    };
    int values[] = {
        1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1
    };
    
    int pos = 0;
    for (int i = 0; i < 13; i++) {
        while (num >= values[i]) {
            int len = strlen(romans[i]);
            if (pos + len > buf_size - 1) return -1;
            strcpy(buffer + pos, romans[i]);
            pos += len;
            num -= values[i];
        }
    }
    buffer[pos] = '\0';
    return 0;
}

int uint_to_zeckendorf(unsigned int num, char *buffer, int buf_size) {
    if (!buffer || buf_size <= 0) return -1;
    
    if (num == 0) {
        if (buf_size < 2) return -1;
        strcpy(buffer, "0");
        return 0;
    }
    
    unsigned int fib[50];
    fib[0] = 1;
    fib[1] = 2;
    int fib_count = 2;
    
    while (fib_count < 50 && fib[fib_count - 1] <= num) {
        fib[fib_count] = fib[fib_count - 1] + fib[fib_count - 2];
        fib_count++;
    }
    
    char temp[60] = {0};
    int pos = 0;
    unsigned int remaining = num;
    
    for (int i = fib_count - 1; i >= 0; i--) {
        if (remaining >= fib[i]) {
            temp[pos++] = '1';
            remaining -= fib[i];
        } else {
            temp[pos++] = '0';
        }
    }

    int start = 0;
    while (start < pos - 1 && temp[start] == '0') {
        start++;
    }
    
    int result_pos = 0;
    for (int i = start; i < pos; i++) {
        if (result_pos >= buf_size - 1) return -1;
        buffer[result_pos++] = temp[i];
    }
    buffer[result_pos] = '\0';
    
    return 0;
}

int int_to_base(int num, int base, char *buffer, int buf_size, int uppercase) {
    if (!buffer || buf_size <= 0) return -1;
    
    if (base < 2 || base > 36) base = 10;
    
    const char *digits_lower = "0123456789abcdefghijklmnopqrstuvwxyz";
    const char *digits_upper = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *digits = uppercase ? digits_upper : digits_lower;
    
    if (num == 0) {
        if (buf_size < 2) return -1;
        strcpy(buffer, "0");
        return 0;
    }
    
    int is_negative = 0;
    unsigned int abs_num;
    if (num < 0) {
        is_negative = 1;
        abs_num = (unsigned int)(-(long long)num);
    } else {
        abs_num = (unsigned int)num;
    }
    
    char temp[100];
    int pos = 0;
    
    while (abs_num > 0 && pos < 99) {
        temp[pos++] = digits[abs_num % base];
        abs_num /= base;
    }
    
    int result_pos = 0;
    if (is_negative) {
        if (result_pos >= buf_size - 1) return -1;
        buffer[result_pos++] = '-';
    }
    
    for (int i = pos - 1; i >= 0; i--) {
        if (result_pos >= buf_size - 1) return -1;
        buffer[result_pos++] = temp[i];
    }
    
    buffer[result_pos] = '\0';
    return 0;
}

int base_to_decimal(const char *str, int base, int uppercase, int *result) {
    if (!str || !result) return -1;
    if (base < 2 || base > 36) base = 10;
    
    const char *ptr = str;
    int is_negative = 0;
    
    while (*ptr == ' ' || *ptr == '\t') ptr++;
    
    if (*ptr == '-') { is_negative = 1; ptr++; }
    else if (*ptr == '+') ptr++;
    
    while (*ptr == ' ' || *ptr == '\t') ptr++;
    
    long long value = 0;
    int has_digits = 0;
    
    while (*ptr) {
        char c = *ptr;
        if (c == ' ' || c == '\t') { ptr++; continue; }
        
        c = toupper((unsigned char)c);
        int digit;
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'A' && c <= 'Z') digit = c - 'A' + 10;
        else return -1;
        
        if (digit >= base) return -1;
        
        if (value > (LLONG_MAX - digit) / base) return -1;
        
        value = value * base + digit;
        has_digits = 1;
        ptr++;
    }
    
    if (!has_digits) return -1;
    if (is_negative) value = -value;
    
    *result = (int)value;
    return 0;
}

int memory_dump(const void *ptr, int size, char *buffer, int buf_size) {
    if (!ptr || !buffer || buf_size <= 0 || size <= 0) return -1;
    
    const unsigned char *bytes = (const unsigned char *)ptr;
    int pos = 0;
    
    for (int i = 0; i < size; i++) {
        unsigned char byte = bytes[i];
        if (pos + 9 > buf_size) return -1;
        
        for (int bit = 7; bit >= 0; bit--) {
            buffer[pos++] = ((byte >> bit) & 1) ? '1' : '0';
        }
        
        if (i < size - 1) buffer[pos++] = ' ';
    }
    
    buffer[pos] = '\0';
    return 0;
}
