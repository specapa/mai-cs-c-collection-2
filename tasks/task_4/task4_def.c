#include "overscanf.h"
#include <stdio.h>
#include <string.h>

int main() {
    int a = -1;
    oversscanf("1450", "%u");
    printf("%u\n", a);
    char b = '-';
    oversscanf("6", "%c");
    printf("%c\n", b);
}