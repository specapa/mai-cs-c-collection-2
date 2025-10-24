#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define SEP printf("--------------------------------------------------\n")

void test_memchr() {
    printf("\n=== TEST memchr ===\n");

    const char s1[] = "Hello";
    printf("[BEFORE] str=\"%s\"\n", s1);
    printf("[CALL] memchr(\"%s\", 'e', 5)\n", s1);
    const char *res1 = memchr(s1, 'e', 5);
    printf("[RESULT] -> %s\n", res1 ? res1 : "NULL");
    assert(res1 && *res1 == 'e');
    SEP;

    printf("[CALL] memchr(\"%s\", 'z', 5)\n", s1);
    const char *res2 = memchr(s1, 'z', 5);
    printf("[RESULT] -> %s\n", res2 ? res2 : "NULL");
    assert(res2 == NULL);
    SEP;

    const char s2[] = "Hello\0World";
    printf("[CALL] memchr(\"Hello\\0World\", 'W', 11)\n");
    const char *res3 = memchr(s2, 'W', 11);
    printf("[RESULT] -> %s\n", res3 ? res3 : "NULL");
    assert(res3 && *res3 == 'W');
    SEP;
}

void test_memcmp() {
    printf("\n=== TEST memcmp ===\n");

    const char *a = "abc", *b = "abd", *c = "abc";
    printf("[BEFORE] a=\"%s\", b=\"%s\"\n", a, b);
    printf("[CALL] memcmp(a, b, 3)\n");
    int r1 = memcmp(a, b, 3);
    printf("[RESULT] -> %d\n", r1);
    assert(r1 < 0);
    SEP;

    printf("[CALL] memcmp(a, c, 3)\n");
    int r2 = memcmp(a, c, 3);
    printf("[RESULT] -> %d\n", r2);
    assert(r2 == 0);
    SEP;

    printf("[CALL] memcmp(a, b, 0)\n");
    int r3 = memcmp(a, b, 0);
    printf("[RESULT] -> %d\n", r3);
    assert(r3 == 0);
    SEP;
}

void test_memcpy() {
    printf("\n=== TEST memcpy ===\n");
    const char src[] = "Hello";
    char dest[10] = "-----";
    printf("[BEFORE] src=\"%s\", dest=\"%s\"\n", src, dest);
    printf("[CALL] memcpy(dest, src, 6)\n");
    memcpy(dest, src, 6);
    printf("[AFTER] dest=\"%s\"\n", dest);
    assert(strcmp(dest, "Hello") == 0);
    SEP;

    printf("[CALL] memcpy(dest, src, 3)\n");
    memcpy(dest, src, 3);
    dest[3] = '\0';
    printf("[AFTER] dest=\"%s\"\n", dest);
    assert(strcmp(dest, "Hel") == 0);
    SEP;
}

void test_memset() {
    printf("\n=== TEST memset ===\n");
    char buf[6] = "-----";
    printf("[BEFORE] buf=\"%s\"\n", buf);
    printf("[CALL] memset(buf, 'A', 5)\n");
    memset(buf, 'A', 5);
    buf[5] = '\0';
    printf("[AFTER] buf=\"%s\"\n", buf);
    assert(strcmp(buf, "AAAAA") == 0);
    SEP;

    printf("[CALL] memset(buf, 0, 3)\n");
    memset(buf, 0, 3);
    printf("[AFTER] first 3 bytes zeroed, buf=\"%s\"\n", buf);
    SEP;
}

void test_strncat() {
    printf("\n=== TEST strncat ===\n");
    char dest[20] = "Hi ";
    const char src[] = "Bob";
    printf("[BEFORE] dest=\"%s\", src=\"%s\"\n", dest, src);
    printf("[CALL] strncat(dest, src, 3)\n");
    strncat(dest, src, 3);
    printf("[AFTER] dest=\"%s\"\n", dest);
    assert(strcmp(dest, "Hi Bob") == 0);
    SEP;

    printf("[CALL] strncat(dest, src, 0)\n");
    strncat(dest, src, 0);
    printf("[AFTER] dest=\"%s\"\n", dest);
    assert(strcmp(dest, "Hi Bob") == 0);
    SEP;
}

void test_strchr() {
    printf("\n=== TEST strchr ===\n");
    const char *s = "Hello";
    printf("[CALL] strchr(\"%s\", 'l')\n", s);
    const char *res = strchr(s, 'l');
    printf("[RESULT] -> %s\n", res);
    assert(res && *res == 'l');
    SEP;

    printf("[CALL] strchr(\"%s\", 'x')\n", s);
    res = strchr(s, 'x');
    printf("[RESULT] -> %s\n", res ? res : "NULL");
    assert(res == NULL);
    SEP;

    printf("[CALL] strchr(\"%s\", '\\0')\n", s);
    res = strchr(s, '\0');
    printf("[RESULT] -> points to null terminator\n");
    assert(*res == '\0');
    SEP;
}

void test_strncmp() {
    printf("\n=== TEST strncmp ===\n");
    const char *a = "abc", *b = "abd";
    printf("[CALL] strncmp(a, b, 2)\n");
    int r1 = strncmp(a, b, 2);
    printf("[RESULT] -> %d\n", r1);
    assert(r1 == 0);
    SEP;

    printf("[CALL] strncmp(a, b, 3)\n");
    int r2 = strncmp(a, b, 3);
    printf("[RESULT] -> %d\n", r2);
    assert(r2 < 0);
    SEP;
}

void test_strncpy() {
    printf("\n=== TEST strncpy ===\n");
    const char src[] = "World";
    char dest[10] = "AAAAAAA";
    printf("[CALL] strncpy(dest, src, 3)\n");
    strncpy(dest, src, 3);
    dest[3] = '\0';
    printf("[AFTER] dest=\"%s\"\n", dest);
    assert(strcmp(dest, "Wor") == 0);
    SEP;

    printf("[CALL] strncpy(dest, src, 10)\n");
    strncpy(dest, src, 10);
    printf("[AFTER] dest=\"%s\"\n", dest);
    assert(strncmp(dest, "World", 5) == 0);
    SEP;
}

void test_strcspn() {
    printf("\n=== TEST strcspn ===\n");
    const char *s1 = "abcdef", *s2 = "xdy";
    printf("[CALL] strcspn(\"%s\", \"%s\")\n", s1, s2);
    size_t r = strcspn(s1, s2);
    printf("[RESULT] -> %zu\n", r);
    assert(r == 3);
    SEP;
}

void test_strerror() {
    printf("\n=== TEST strerror ===\n");
    printf("[CALL] strerror(1)\n");
    const char *msg = strerror(1);
    printf("[RESULT] -> \"%s\"\n", msg);
    assert(msg != NULL);
    SEP;
}

void test_strlen() {
    printf("\n=== TEST strlen ===\n");
    printf("[CALL] strlen(\"Test\")\n");
    size_t len = strlen("Test");
    printf("[RESULT] -> %zu\n", len);
    assert(len == 4);
    SEP;

    printf("[CALL] strlen(\"\")\n");
    len = strlen("");
    printf("[RESULT] -> %zu\n", len);
    assert(len == 0);
    SEP;
}

void test_strpbrk() {
    printf("\n=== TEST strpbrk ===\n");
    const char *s1 = "abcdef", *s2 = "xdy";
    printf("[CALL] strpbrk(\"%s\", \"%s\")\n", s1, s2);
    const char *res = strpbrk(s1, s2);
    printf("[RESULT] -> %s\n", res);
    assert(res && *res == 'd');
    SEP;
}

void test_strrchr() {
    printf("\n=== TEST strrchr ===\n");
    const char *s = "Hello";
    printf("[CALL] strrchr(\"%s\", 'l')\n", s);
    const char *res = strrchr(s, 'l');
    printf("[RESULT] -> %s\n", res);
    assert(strcmp(res, "lo") == 0);
    SEP;
}

void test_strstr() {
    printf("\n=== TEST strstr ===\n");
    const char *h = "Hello world", *n = "world";
    printf("[CALL] strstr(\"%s\", \"%s\")\n", h, n);
    const char *res = strstr(h, n);
    printf("[RESULT] -> %s\n", res);
    assert(strcmp(res, "world") == 0);
    SEP;

    printf("[CALL] strstr(\"Hello\", \"\")\n");
    res = strstr("Hello", "");
    printf("[RESULT] -> %s\n", res);
    assert(res && strcmp(res, "Hello") == 0);
    SEP;
}

void test_strtok() {
    printf("\n=== TEST strtok ===\n");
    char str[] = "one,,two,three";
    printf("[BEFORE] str=\"%s\"\n", str);
    printf("[CALL] strtok(str, \",\")\n");
    char *token = strtok(str, ",");
    while (token) {
        printf("[TOKEN] %s\n", token);
        token = strtok(NULL, ",");
    }
    printf("[AFTER] internal static pointer advanced\n");
    SEP;
}

int main() {
    printf("========== STRING FUNCTION TESTS ==========\n");
    test_memchr();
    test_memcmp();
    test_memcpy();
    test_memset();
    test_strncat();
    test_strchr();
    test_strncmp();
    test_strncpy();
    test_strcspn();
    test_strerror();
    test_strlen();
    test_strpbrk();
    test_strrchr();
    test_strstr();
    test_strtok();
    printf("\n✅ ALL TESTS COMPLETED SUCCESSFULLY\n");
    return 0;
}
