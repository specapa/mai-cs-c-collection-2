#include "overscanf.h"
#include <stdio.h>
#include <string.h>

int main() {
    int a = -1, b = -1, c = -1;
    char chr = '-';

    oversscanf("1450 1451 1452", "%u %u %u", &a, &b, &c);
    printf("%u %u %u\n", a, b, c);
    
    oversscanf("6", "%c", &chr);
    printf("%c\n", chr);

    oversscanf("14 15 16 g", "%u %u %u %c", &a, &b, &c, &chr);
    printf("%u %u %u %c\n", a, b, c, chr);

    oversscanf("1|2|3|k", "%u|%u|%u|%c", &a, &b, &c, &chr);
    printf("%u %u %u %c\n", a, b, c, chr);
}