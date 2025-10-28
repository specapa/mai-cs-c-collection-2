#include "overscanf.h"
#include <stdio.h>
#include <string.h>

void print_separator(const char *title) {
    printf("\n");
    printf("=== %s ===\n", title);
    printf("\n");
}

int main(void) {
    register_default_scanformats();

    int ires = 0;
    unsigned int ures = 0;
    char strbuf[128];

    // 1. Римские цифры (%Ri)
    print_separator("1. Римские цифры (%Ri)");
    oversscanf("XIV", "%Ri", &ires);
    printf("   XIV = %d\n", ires);
    oversscanf("CMXCIX", "%Ri", &ires);
    printf("   CMXCIX = %d\n", ires);
    oversscanf("MCMXCIV", "%Ri", &ires);
    printf("   MCMXCIV = %d\n", ires);
    oversscanf("XLII", "%Ri", &ires);
    printf("   XLII = %d\n", ires);

    // 2. Представление Цекендорфа (%Zr)
    print_separator("2. Представление Цекендорфа (%Zr)");
    oversscanf("1", "%Zr", &ures);
    printf("   1 = %u\n", ures);
    oversscanf("10", "%Zr", &ures);
    printf("   10 = %u\n", ures);
    oversscanf("1001", "%Zr", &ures);
    printf("   1001 = %u\n", ures);
    oversscanf("101010", "%Zr", &ures);
    printf("   101010 = %u\n", ures);

    // 3. Число из системы счисления (строчные, %Cv)
    print_separator("3. Число из системы счисления (%Cv — строчные)");
    oversscanf("ff", "%Cv", 16, &ires);
    printf("   'ff' base 16 = %d\n", ires);
    oversscanf("101010", "%Cv", 2, &ires);
    printf("   '101010' base 2 = %d\n", ires);
    oversscanf("777", "%Cv", 8, &ires);
    printf("   '777' base 8 = %d\n", ires);
    oversscanf("zz", "%Cv", 36, &ires);
    printf("   'zz' base 36 = %d\n", ires);

    // 4. Число из системы счисления (заглавные, %CV)
    print_separator("4. Число из системы счисления (%CV — заглавные)");
    oversscanf("FF", "%CV", 16, &ires);
    printf("   'FF' base 16 = %d\n", ires);
    oversscanf("ABC", "%CV", 16, &ires);
    printf("   'ABC' base 16 = %d\n", ires);
    oversscanf("ZZ", "%CV", 36, &ires);
    printf("   'ZZ' base 36 = %d\n", ires);

    // 5. Комбинированное использование
    print_separator("5. Комбинированное использование");
    {   
        int ires_2 = -1, ires_3 = -1;
        oversscanf("FF 101010", "%CV %Cv", 16, &ires_2, 2, &ires_3);
        printf("FF -> %d, 101010 -> %d\n", ires_2, ires_3);
        oversscanf("FA||1010", "%CV||%Cv", 16, &ires_2, 2, &ires_3);
        printf("FA -> %d, 1010 -> %d\n", ires_2, ires_3);
        oversscanf("10||1001", "%Zr||%Zr",&ires_2, &ires_3);
        printf("Zeckendorf: 10 -> %d, 1001 -> %d\n", ires_2, ires_3);
    }

    // 6. Граничные случаи
    print_separator("6. Граничные случаи");
    oversscanf("MMMCMXCIX", "%Ri", &ires);
    printf("   'MMMCMXCIX' (3999) = %d\n", ires);
    oversscanf("0", "%Zr", &ures);
    printf("   '0' (Цекендорф) = %u\n", ures);
    oversscanf("0", "%Cv", 2, &ires);
    printf("   '0' base 2 = %d\n", ires);

    // 7. Буферный тест (строчные)
    print_separator("7. Проверка oversscanf в буфере");
    char input[256] = "ff";
    oversscanf(input, "%Cv", 16, &ires);
    printf("   В буфере: 'ff' -> %d\n", ires);

    return 0;
}
