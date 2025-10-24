#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "nostring.h"

#define TEST_BEGIN(name, args) \
    printf("\n[TEST] %s(%s)\n", name, args)

#define TEST_EXPECTED(expected) \
    printf(" expected: %s\n", expected)

#define TEST_RESULT(format, value) \
    printf(" result: " format "\n", value)

#define TEST_PASS() \
    printf(" ✅ PASSED\n")

/*   memchr   */
void test_my_memchr() {
    TEST_BEGIN("my_memchr", "\"Hello\", 'e', 5");
    const char *str = "Hello";
    void *res = memchr(str, 'e', 5);
    TEST_EXPECTED("'e'");
    TEST_RESULT("'%c'", *(char*)res);
    assert(*(char*)res == 'e');
    TEST_PASS();

    TEST_BEGIN("my_memchr", "\"Hello\", 'z', 5");
    res = memchr(str, 'z', 5);
    TEST_EXPECTED("NULL");
    TEST_RESULT("%s", res == NULL ? "NULL" : "NOT NULL");
    assert(res == NULL);
    TEST_PASS();
}

/*   memcmp   */
void test_my_memcmp() {
    TEST_BEGIN("my_memcmp", "\"abc\", \"abc\", 3");
    int res = memcmp("abc", "abc", 3);
    TEST_EXPECTED("0");
    TEST_RESULT("%d", res);
    assert(res == 0);
    TEST_PASS();

    TEST_BEGIN("my_memcmp", "\"abc\", \"abd\", 3");
    res = memcmp("abc", "abd", 3);
    TEST_EXPECTED("<0");
    TEST_RESULT("%d", res);
    assert(res < 0);
    TEST_PASS();
}

/*   memcpy   */
void test_my_memcpy() {
    TEST_BEGIN("my_memcpy", "src=\"Hello\", n=6");
    char src[] = "Hello";
    char dest[10];
    memcpy(dest, src, 6);
    TEST_EXPECTED("\"Hello\"");
    TEST_RESULT("\"%s\"", dest);
    assert(strcmp(dest, "Hello") == 0);
    TEST_PASS();
}

/*   memset   */
void test_my_memset() {
    TEST_BEGIN("my_memset", "('A', 5)");
    char buf[6];
    memset(buf, 'A', 5);
    buf[5] = '\0';
    TEST_EXPECTED("\"AAAAA\"");
    TEST_RESULT("\"%s\"", buf);
    assert(strcmp(buf, "AAAAA") == 0);
    TEST_PASS();
}

/*   strncat   */
void test_my_strncat() {
    TEST_BEGIN("my_strncat", "dest=\"Hi \", src=\"Bob\", n=3");
    char dest[20] = "Hi ";
    strncat(dest, "Bob", 3);
    TEST_EXPECTED("\"Hi Bob\"");
    TEST_RESULT("\"%s\"", dest);
    assert(strcmp(dest, "Hi Bob") == 0);
    TEST_PASS();
}

/*   strchr   */
void test_my_strchr() {
    TEST_BEGIN("my_strchr", "\"Hello\", 'l'");
    const char *res = strchr("Hello", 'l');
    TEST_EXPECTED("'l'");
    TEST_RESULT("'%c'", *res);
    assert(*res == 'l');
    TEST_PASS();
}

/*   strncmp   */
void test_my_strncmp() {
    TEST_BEGIN("my_strncmp", "\"abc\", \"abd\", 3");
    int res = strncmp("abc", "abd", 3);
    TEST_EXPECTED("<0");
    TEST_RESULT("%d", res);
    assert(res < 0);
    TEST_PASS();
}

/*   strncpy   */
void test_my_strncpy() {
    TEST_BEGIN("my_strncpy", "src=\"World\", n=3");
    char dest[10];
    strncpy(dest, "World", 3);
    dest[3] = '\0';
    TEST_EXPECTED("\"Wor\"");
    TEST_RESULT("\"%s\"", dest);
    assert(strcmp(dest, "Wor") == 0);
    TEST_PASS();
}

/*   strcspn   */
void test_my_strcspn() {
    TEST_BEGIN("my_strcspn", "\"abcdef\", \"xdy\"");
    size_t res = strcspn("abcdef", "xdy");
    TEST_EXPECTED("3");
    TEST_RESULT("%zu", res);
    assert(res == 3);
    TEST_PASS();
}

/*   strerror   */
void test_my_strerror() {
    TEST_BEGIN("my_strerror", "errnum=2");
    const char *msg = strerror(2);
    TEST_EXPECTED("system error message");
    TEST_RESULT("\"%s\"", msg);
    assert(msg != NULL);
    TEST_PASS();
}

/*   strlen   */
void test_my_strlen() {
    TEST_BEGIN("my_strlen", "\"Test\"");
    size_t len = strlen("Test");
    TEST_EXPECTED("4");
    TEST_RESULT("%zu", len);
    assert(len == 4);
    TEST_PASS();
}

/*   strpbrk   */
void test_my_strpbrk() {
    TEST_BEGIN("my_strpbrk", "\"abcdef\", \"xdy\"");
    const char *res = strpbrk("abcdef", "xdy");
    TEST_EXPECTED("'d'");
    TEST_RESULT("'%c'", *res);
    assert(*res == 'd');
    TEST_PASS();
}

/*   strrchr   */
void test_my_strrchr() {
    TEST_BEGIN("my_strrchr", "\"Hello\", 'l'");
    const char *res = strrchr("Hello", 'l');
    TEST_EXPECTED("'l'");
    TEST_RESULT("'%c'", *res);
    assert(*res == 'l');
    TEST_PASS();
}

/*   strstr   */
void test_my_strstr() {
    TEST_BEGIN("my_strstr", "\"Hello world\", \"world\"");
    const char *res = strstr("Hello world", "world");
    TEST_EXPECTED("\"world\"");
    TEST_RESULT("\"%s\"", res);
    assert(strcmp(res, "world") == 0);
    TEST_PASS();
}

/*   strtok   */
void test_my_strtok() {
    TEST_BEGIN("my_strtok", "\"one,two,three\", \",\"");
    char text[] = "one,two,three";
    char *token = strtok(text, ",");
    TEST_EXPECTED("\"one\"");
    TEST_RESULT("\"%s\"", token);
    assert(strcmp(token, "one") == 0);

    token = strtok(NULL, ",");
    TEST_EXPECTED("\"two\"");
    TEST_RESULT("\"%s\"", token);
    assert(strcmp(token, "two") == 0);

    token = strtok(NULL, ",");
    TEST_EXPECTED("\"three\"");
    TEST_RESULT("\"%s\"", token);
    assert(strcmp(token, "three") == 0);

    TEST_PASS();
}

/*   MAIN   */
int main() {
    printf("===== START TESTS =====\n");
    test_my_memchr();
    test_my_memcmp();
    test_my_memcpy();
    test_my_memset();
    test_my_strncat();
    test_my_strchr();
    test_my_strncmp();
    test_my_strncpy();
    test_my_strcspn();
    test_my_strerror();
    test_my_strlen();
    test_my_strpbrk();
    test_my_strrchr();
    test_my_strstr();
    test_my_strtok();
    printf("\n===== ALL TESTS PASSED =====\n");
    return 0;
}
