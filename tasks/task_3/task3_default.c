#include <stdio.h>
#include "overprintf.h"

int main() {
    int num = 123;
    float f = 3.14159;
    
    // Флаги выравнивания и заполнения:
    
    // - выравнивание по левому краю
    overfprintf(stdout, "%-10d|\n", num);
    
    // + всегда показывать знак
    overfprintf(stdout, "%+d\n", num);
    
    // пробел - пробел перед положительными числами
    overfprintf(stdout, "% d\n", num);
    
    // # альтернативная форма
    overfprintf(stdout, "%#x\n", num);    // 0x7b
    overfprintf(stdout, "%#o\n", num);    // 0173
    overfprintf(stdout, "%#g\n", f);      // всегда десятичная точка
    
    // 0 заполнение нулями
    overfprintf(stdout, "%010d\n", num);
    
    // Модификаторы ширины и точности:
    
    // ширина поля
    overfprintf(stdout, "%10d\n", num);
    
    // точность для чисел
    overfprintf(stdout, "%.2f\n", f);
    
    // точность для строк
    overfprintf(stdout, "%.3s\n", "Hello");
    
    // Модификаторы длины:
    
    short s = 123;
    long l = 123456L;
    long long ll = 123456789LL;
    
    // %hd - short
    overfprintf(stdout, "%hd\n", s);
    
    // %ld - long
    overfprintf(stdout, "%ld\n", l);
    
    // %lld - long long
    overfprintf(stdout, "%lld\n", ll);
    
    // %hu - unsigned short
    overfprintf(stdout, "%hu\n", (unsigned short)s);
    
    // %lu - unsigned long
    overfprintf(stdout, "%lu\n", (unsigned long)l);
    
    // %llu - unsigned long long
    overfprintf(stdout, "%llu\n", (unsigned long long)ll);
    
    // %Lf - long double
    long double ld = 3.1415926535L;
    overfprintf(stdout, "%Lf\n", ld);
    
    // %zd - size_t
    size_t sz = 100;
    overfprintf(stdout, "%zd\n", sz);
    
    // Long test
    overfprintf(stdout, "%llu %llu %llu %llu %llu\n", 10UL, 5UL, 15UL, 200UL, 10000UL);

        
    return 0;
}