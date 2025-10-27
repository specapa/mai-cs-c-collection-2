#include "overprintf.h"
#include <stdio.h>
#include <string.h>

void print_separator(const char *title) {
    printf("\n");
    printf("=== %s ===\n", title);
    printf("\n");
}

int main(void) {
    register_default_formats();
    char buffer[2048];
    
    // 1. Римские цифры
    print_separator("1. Римские цифры (%Ro)");
    overfprintf(stdout, "   2024 = %Ro 2025 = %Ro\n", 2024, 2025);
    overfprintf(stdout, "   58 = %Ro\n", 58);
    overfprintf(stdout, "   1999 = %Ro\n", 1999);
    overfprintf(stdout, "   444 = %Ro\n", 444);
    oversprintf(buffer, "   Buffer test: 3999 = %Ro", 3999);
    printf("   %s\n", buffer);
    
    // 2. Представление Цекендорфа
    print_separator("2. Представление Цекендорфа (%Zr)");
    printf("   (коэффициенты при числах Фибоначчи от младшего к старшему + '1')\n\n");
    overfprintf(stdout, "   0 = %Zr\n", 0u);
    overfprintf(stdout, "   1 = %Zr\n", 1u);
    overfprintf(stdout, "   10 = %Zr\n", 10u);
    overfprintf(stdout, "   25 = %Zr\n", 25u);
    overfprintf(stdout, "   100 = %Zr\n", 100u);
    oversprintf(buffer, "   Buffer test: 100 = %Zr", 100u);
    printf("   %s\n", buffer);
    
    // 3. Число в системе счисления (строчные)
    print_separator("3. Число в системе счисления (%Cv - строчные)");
    overfprintf(stdout, "   255 в base 16 = %Cv\n", 16, 255);
    overfprintf(stdout, "   42 в base 2 = %Cv\n", 2, 42);
    overfprintf(stdout, "   1000 в base 8 = %Cv\n", 8, 1000);
    overfprintf(stdout, "   -15 в base 16 = %Cv\n", 16, -15);
    overfprintf(stdout, "   3000 в base 36 = %Cv\n", 36, 3000);
    oversprintf(buffer, "   Buffer test: 255 в base 2 = %Cv", 2, 255);
    printf("   %s\n", buffer);
    
    // 4. Число в системе счисления (заглавные)
    print_separator("4. Число в системе счисления (%CV - заглавные)");
    overfprintf(stdout, "   255 в base 16 = %CV\n", 16, 255);
    overfprintf(stdout, "   2748 в base 16 = %CV (ABC в hex)\n", 16, 2748);
    overfprintf(stdout, "   3000 в base 36 = %CV\n", 36, 3000);
    oversprintf(buffer, "   Buffer test: 1000 в base 16 = %CV", 16, 1000);
    printf("   %s\n", buffer);
    
    // 5. Перевод строки в decimal (строчные)
    print_separator("5. Перевод строки в decimal (%to - строчные)");
    overfprintf(stdout, "   'ff' (base 16) = %to\n", "FF", 16);
    overfprintf(stdout, "   '101010' (base 2) = %to\n", "101010", 2);
    overfprintf(stdout, "   'abc' (base 16) = %to\n", "abc", 16);
    overfprintf(stdout, "   '777' (base 8) = %to\n", "777", 8);
    overfprintf(stdout, "   'zz' (base 36) = %to\n", "zz", 36);
    oversprintf(buffer, "   Buffer test: '1111' (base 2) = %to", "1111", 2);
    printf("   %s\n", buffer);
    
    // 6. Перевод строки в decimal (заглавные)
    print_separator("6. Перевод строки в decimal (%TO - заглавные)");
    overfprintf(stdout, "   'FF' (base 16) = %TO\n", "FF", 16);
    overfprintf(stdout, "   'ABC' (base 16) = %TO\n", "ABC", 16);
    overfprintf(stdout, "   'ZZ' (base 36) = %TO\n", "ZZ", 36);
    oversprintf(buffer, "   Buffer test: '1A' (base 16) = %TO", "1A", 16);
    printf("   %s\n", buffer);
    
    // 7. Дамп памяти signed int
    print_separator("7. Дамп памяти signed int (%mi)");
    printf("   (4 байта в двоичном виде, порядок зависит от endianness)\n\n");
    overfprintf(stdout, "   42 = %mi\n", 42);
    overfprintf(stdout, "   -1 = %mi\n", -1);
    overfprintf(stdout, "   256 = %mi\n", 256);
    overfprintf(stdout, "   0 = %mi\n", 0);
    oversprintf(buffer, "   Buffer test: -42 = %mi", -42);
    printf("   %s\n", buffer);
    
    // 8. Дамп памяти unsigned int
    print_separator("8. Дамп памяти unsigned int (%mu)");
    overfprintf(stdout, "   255u = %mu\n", 255u);
    overfprintf(stdout, "   0u = %mu\n", 0u);
    overfprintf(stdout, "   65535u = %mu\n", 65535u);
    oversprintf(buffer, "   Buffer test: 1024u = %mu", 1024u);
    printf("   %s\n", buffer);
    
    // 9. Дамп памяти double
    print_separator("9. Дамп памяти double (%md)");
    printf("   (8 байт в двоичном виде, IEEE 754)\n\n");
    overfprintf(stdout, "   3.14 = %md\n", 3.14);
    overfprintf(stdout, "   -1.5 = %md\n", -1.5);
    overfprintf(stdout, "   0.0 = %md\n", 0.0);
    oversprintf(buffer, "   Buffer test: 2.71828 = %md", 2.71828);
    printf("   %s\n", buffer);
    
    // 10. Дамп памяти float
    print_separator("10. Дамп памяти float (%mf)");
    printf("   (4 байта в двоичном виде, IEEE 754)\n\n");
    overfprintf(stdout, "   3.14f = %mf\n", 3.14f);
    overfprintf(stdout, "   -2.5f = %mf\n", -2.5f);
    overfprintf(stdout, "   1.0f = %mf\n", 1.0f);
    oversprintf(buffer, "   Buffer test: 0.5f = %mf", 0.5f);
    printf("   %s\n", buffer);
    
    // 11. Комбинированное использование
    print_separator("11. Комбинированное использование");
    overfprintf(stdout, "   Год %d = %Ro (римскими)\n", 2024, 2024);
    overfprintf(stdout, "   Число %d в двоичном: %Cv\n", 42, 2, 42);
    overfprintf(stdout, "   255 = %Ro = %Cv (hex) = %CV (HEX)\n", 
                255, 16, 255, 16, 255);
    oversprintf(buffer, "   Комбо: %d в base %d = %Cv, римскими = %Ro", 
                100, 16, 16, 100, 100);
    printf("   %s\n", buffer);
    
    // 12. Работа со стандартными флагами
    print_separator("12. Стандартные флаги printf");
    overfprintf(stdout, "   Integer: %d, Float: %.2f, String: %s\n", 
                42, 3.14159, "Hello");
    overfprintf(stdout, "   Hex: %#x, Octal: %o, Char: %c\n", 
                255, 64, 'A');
    overfprintf(stdout, "   Pointer: %p, Percent: %%\n", (void*)buffer);
    oversprintf(buffer, "   Formatted: |%10d| |%-10s| |%08x|", 
                42, "test", 255);
    printf("   %s\n", buffer);
    
    // 13. Форматирование с шириной поля
    print_separator("13. Форматирование с шириной и выравниванием");
    overfprintf(stdout, "   |%10d| |%-10d| |%010d|\n", 42, 42, 42);
    overfprintf(stdout, "   |%10.2f| |%-10.2f| |%+10.2f|\n", 3.14, 3.14, 3.14);
    overfprintf(stdout, "   |%10s| |%-10s|\n", "test", "test");
    
    // 14. Практические примеры
    print_separator("14. Практические примеры");
    
    printf("   a) Конвертер систем счисления:\n");
    int num = 42;
    overfprintf(stdout, "      Decimal: %d\n", num);
    overfprintf(stdout, "      Binary:  %Cv\n", 2, num);
    overfprintf(stdout, "      Octal:   %Cv\n", 8, num);
    overfprintf(stdout, "      Hex:     %Cv\n", 16, num);
    overfprintf(stdout, "      Base36:  %Cv\n", 36, num);
    
    printf("\n   b) Анализ представлений числа:\n");
    num = 255;
    overfprintf(stdout, "      Число: %d\n", num);
    overfprintf(stdout, "      Hex: %CV\n", 16, num);
    overfprintf(stdout, "      Roman: %Ro\n", num);
    overfprintf(stdout, "      Memory: %mi\n", num);
    
    printf("\n   c) Обратное преобразование:\n");
    overfprintf(stdout, "      'FF' (hex) = %TO (decimal)\n", "FF", 16);
    overfprintf(stdout, "      'ff' (hex) = %to (decimal)\n", "ff", 16);
    overfprintf(stdout, "      '101010' (bin) = %to (decimal)\n", "101010", 2);
    
    printf("\n   d) Числа Фибоначчи в Цекендорфе:\n");
    unsigned int fib[] = {1, 2, 3, 5, 8, 13, 21, 34};
    for (int i = 0; i < 8; i++) {
        overfprintf(stdout, "      F(%d) = %u = %Zr\n", i, fib[i], fib[i]);
    }
    
    // 15. Тест записи в файл
    print_separator("15. Запись в файл (overfprintf)");
    FILE *f = fopen("test_output.txt", "w");
    if (f) {
        overfprintf(f, "=== Test Output File ===\n\n");
        overfprintf(f, "Roman: %Ro\n", 2024);
        overfprintf(f, "Hex: %CV\n", 16, 255);
        overfprintf(f, "Zeckendorf: %Zr\n", 100u);
        overfprintf(f, "Standard: %d %.2f %s\n", 42, 3.14, "test");
        fclose(f);
        printf("     Файл 'test_output.txt' создан успешно!\n");
        
        // Читаем и выводим содержимое
        f = fopen("test_output.txt", "r");
        if (f) {
            printf("\n   Содержимое файла:\n");
            char line[256];
            while (fgets(line, sizeof(line), f)) {
                printf("   > %s", line);
            }
            fclose(f);
        }
    } else {
        printf("     Ошибка создания файла\n");
    }
    
    // 16. Граничные случаи
    print_separator("16. Граничные случаи");
    overfprintf(stdout, "   MIN int в hex: %Cv\n", 16, -2147483647-1);
    overfprintf(stdout, "   MAX int римскими: %Ro\n", 3999);
    overfprintf(stdout, "   Нуль: decimal=%d, hex=%Cv, roman=%Ro\n", 
                0, 16, 0, 1);
    overfprintf(stdout, "   Отрицательные: %d = %Cv (hex)\n", 
                -100, 16, -100);
    
    // 17. Демонстрация буфера
    print_separator("17. Работа с oversprintf (буфер)");
    char large_buffer[1024];
    int written = oversprintf(large_buffer, 
        "Сложная строка: %d в римских = %Ro, в hex = %Cv, binary = %Cv, "
        "обратно из 'FF'(hex) = %TO, float %.2f, memory int %d = %mi",
        255, 255, 16, 255, 2, 255, "FF", 16, 3.14159, 42, 42);
    
    printf("   Записано символов: %d\n", written);
    printf("   Результат:\n   %s\n", large_buffer);
    
    return 0;
}