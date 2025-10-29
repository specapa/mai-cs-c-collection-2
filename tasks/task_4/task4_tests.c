#include "overscanf.h"
#include <stdio.h>
#include <string.h>

void print_separator(const char *title) {
    printf("\n");
    printf("=== %s ===\n", title);
    printf("\n");
}

// 1. Римские цифры (%Ri)
int roman_test() {
    unsigned int ires = 0;
    print_separator("1. Римские цифры (%Ro)");
    oversscanf("XIV", "%Ro", &ires);
    printf("   XIV = %d\n", ires);
    oversscanf("CMXCIX", "%Ro", &ires);
    printf("   CMXCIX = %d\n", ires);
    oversscanf("MCMXCIV", "%Ro", &ires);
    printf("   MCMXCIV = %d\n", ires);
    oversscanf("XLII", "%Ro", &ires);
    printf("   XLII = %d\n", ires);
    oversscanf("XXXXXXXX", "%Ro", &ires);
    printf("   XXXXXXXX = %d\n", ires);
    oversscanf("XXX1", "%Ro", &ires);
    printf("   XXX1 = %d\n", ires);

    return 0;
}

// 2. Представление Цекендорфа (%Zr)
int zekendorf_test() {
    unsigned int ires = 0;
    print_separator("2. Представление Цекендорфа (%Zr)");
    fprintf(stdout, "before");
    oversscanf("1 1", "%Zr %Zr", &ires, &ires);
    printf("   1 = %u\n", ires);
    oversscanf("10", "%Zr", &ires);
    printf("   10 = %u\n", ires);
    oversscanf("1001", "%Zr", &ires);
    printf("   1001 = %u\n", ires);
    oversscanf("101010", "%Zr", &ires);
    printf("   101010 = %u\n", ires);
    oversscanf("1100", "%Zr", &ires);
    printf("   1100 = %u\n", ires);

    return 0;
}

// 3. Число из системы счисления (строчные, %Cv)
int from_base_test() {
    int ires = 0;
    print_separator("3. Число из системы счисления (%Cv — строчные)");
    oversscanf("ff", "%Cv", 16, &ires);
    printf("   'ff' base 16 = %d\n", ires);
    oversscanf("101010", "%Cv", 2, &ires);
    printf("   '101010' base 2 = %d\n", ires);
    oversscanf("777", "%Cv", 8, &ires);
    printf("   '777' base 8 = %d\n", ires);
    oversscanf("zz", "%Cv", 36, &ires);
    printf("   'zz' base 36 = %d\n", ires);

    return 0;
}

// 4. Число из системы счисления (заглавные, %CV)
int from_base_uppercase_test() {
    int ires = 0;
    print_separator("4. Число из системы счисления (%CV — заглавные)");
    oversscanf("FF", "%CV", 16, &ires);
    printf("   'FF' base 16 = %d\n", ires);
    oversscanf("ABC", "%CV", 16, &ires);
    printf("   'ABC' base 16 = %d\n", ires);
    oversscanf("ZZ", "%CV", 36, &ires);
    printf("   'ZZ' base 36 = %d\n", ires);

    return 0;
}

// 5. Комбинированное использование
int combo_test() {
    print_separator("5. Комбинированное использование");
    int ires_2 = -1;
    int ires_3 = -1;
    oversscanf("FF 101010", "%CV %Cv", 16, &ires_2, 2, &ires_3);
    printf("FF -> %d, 101010 -> %d\n", ires_2, ires_3);
    oversscanf("FA||1010", "%CV||%Cv", 16, &ires_2, 2, &ires_3);
    printf("FA -> %d, 1010 -> %d\n", ires_2, ires_3);
    oversscanf("10||1001", "%Zr||%Zr",&ires_2, &ires_3);
    printf("Zeckendorf: 10 -> %d, 1001 -> %d\n", ires_2, ires_3);

    return 0;
}

// 6. Граничные случаи
int limits_test() {
    int ires = 0;
    unsigned int ures = 0;
    print_separator("6. Граничные случаи");
    oversscanf("MMMCMXCIX", "%Ro", &ires);
    printf("   'MMMCMXCIX' (3999) = %d\n", ires);
    oversscanf("0", "%Zr", &ures);
    printf("   '0' (Цекендорф) = %u\n", ures);
    oversscanf("0", "%Cv", 2, &ires);
    printf("   '0' base 2 = %d\n", ires);

    return 0;
}

// 7. Буферный тест (строчные)
int buffer_test() {
    int ires = 0;
    print_separator("7. Проверка oversscanf в буфере");
    char input[256] = "ff";
    oversscanf(input, "%Cv", 16, &ires);
    printf("   В буфере: 'ff' -> %d\n", ires);

    return 0;
}

typedef int (test_function)(void);

int runner(test_function f) {
    return f();
}

int main(void) {
    #ifdef OVERSCAN_WARNINGS
        fprintf(stdout, "Тестирование в режиме OVERSCAN_WARNINGS");
    #endif 

    register_default_scanformats();
    test_function *tests[] = {
        &roman_test,
        &zekendorf_test,
        &from_base_test,
        &from_base_uppercase_test,
        &combo_test,
        &limits_test,
        &buffer_test
    };

    for (size_t i = 0; i < 7; ++i) {
        if ((*tests[i])() != 0) {
            fprintf(stderr, "FAILED");
            return 145;
        };
    }
}