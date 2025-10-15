#include <gtest/gtest.h>
#include <sstream>

extern "C" {
    #include "task_math.h"
}

static usgshrt_vec_t* make_vec(std::initializer_list<unsigned short> data) {
    usgshrt_vec_t* v = nullptr;
    EXPECT_EQ(usgshrt_vec_init(&v), U_OK);
    for (auto d : data) {
        EXPECT_EQ(usgshrt_vec_push_back(v, d), U_OK);
    }
    return v;
}

static std::string capture_digits(void (*fn)(FILE*, usgshrt_vec_t*, size_t),
                                  usgshrt_vec_t* vec, size_t shift = 0) {
    std::stringstream ss;
    FILE* tmp = tmpfile();
    EXPECT_NE(tmp, nullptr);
    fn(tmp, vec, shift);
    rewind(tmp);
    char buf[128] = {0};
    fread(buf, 1, sizeof(buf) - 1, tmp);
    fclose(tmp);
    return std::string(buf);
}

//
// multiply_fraction
//

TEST(MultiplyFractionTest, HandlesEmptyFraction) {
    usgshrt_vec_t* res = nullptr;
    unsigned short digit = 999;
    u_status_t st = multiply_fraction(nullptr, 10, 2, &res, &digit);
    EXPECT_EQ(st, U_INVALID_FORMAT);
}

TEST(MultiplyFractionTest, MultipliesCorrectly) {
    auto frac = make_vec({1, 2, 3});
    usgshrt_vec_t* new_frac = nullptr;
    unsigned short digit = 0;

    u_status_t st = multiply_fraction(frac, 10, 2, &new_frac, &digit);
    EXPECT_EQ(st, U_OK);
    EXPECT_LE(digit, USHRT_MAX);

    EXPECT_EQ(new_frac->size, frac->size);
    usgshrt_vec_free(&new_frac);
    usgshrt_vec_free(&frac);
}

TEST(MultiplyFractionTest, OverflowCheck) {
    auto frac = make_vec({USHRT_MAX});
    usgshrt_vec_t* new_frac = nullptr;
    unsigned short digit = 0;
    u_status_t st = multiply_fraction(frac, 10, USHRT_MAX + 10ULL, &new_frac, &digit);
    EXPECT_TRUE(st == U_OVERFLOW);
    usgshrt_vec_free(&frac);
    if (new_frac) usgshrt_vec_free(&new_frac);
}

//
// has_a_finite_representation
//

TEST(RepresentationTest, HandlesInvalidInput) {
    rep_state_t rep;
    usgshrt_vec_t* digits = nullptr;
    size_t sft = 0;
    EXPECT_EQ(has_a_finite_representation(nullptr, 10, 2, 10, &rep, &digits, &sft), U_INVALID_FORMAT);
}

TEST(RepresentationTest, DetectsFiniteFraction) {
    auto frac = make_vec({5});
    rep_state_t rep;
    usgshrt_vec_t* digits = nullptr;
    size_t shift = 0;

    u_status_t st = has_a_finite_representation(frac, 10, 2, 20, &rep, &digits, &shift);
    EXPECT_EQ(st, U_OK);
    EXPECT_TRUE(rep == REP_FINITE);

    if (digits) usgshrt_vec_free(&digits);
    usgshrt_vec_free(&frac);
}

TEST(RepresentationTest, DetectsInfiniteCycle) {
    auto frac = make_vec({3});
    rep_state_t rep;
    usgshrt_vec_t* digits = nullptr;
    size_t shift = 0;

    u_status_t st = has_a_finite_representation(frac, 10, 3, 50, &rep, &digits, &shift);
    EXPECT_EQ(st, U_OK);
    EXPECT_TRUE(rep == REP_INFINITE);

    if (digits) usgshrt_vec_free(&digits);
    usgshrt_vec_free(&frac);
}

TEST(RepresentationTest, DetectsCycleForPointThreeInBase3) {
    usgshrt_vec_t* frac = nullptr;
    ASSERT_EQ(usgshrt_vec_init(&frac), U_OK);
    ASSERT_EQ(usgshrt_vec_push_back(frac, 3), U_OK);

    rep_state_t rep;
    usgshrt_vec_t* digits = nullptr;
    size_t shift = 0;

    u_status_t st = has_a_finite_representation(frac, 10, 3, 50, &rep, &digits, &shift);
    ASSERT_EQ(st, U_OK);

    EXPECT_EQ(rep, REP_INFINITE);
    ASSERT_NE(digits, nullptr);
    EXPECT_GT(digits->size, 0);
    EXPECT_LT(shift, digits->size);

    std::stringstream ss;
    for (size_t i = 0; i < digits->size; ++i) {
        if (i == shift) ss << "(";
        ss << digits->vec[i];
    }
    ss << ")";
    std::string result = ss.str();

    std::cout << "0.3 in base 3: " << result << " with cycle starting at index " << shift << std::endl;

    usgshrt_vec_free(&digits);
    usgshrt_vec_free(&frac);
}

TEST(RepresentationTest, UnDetectsCycleForPointThreeInBase3) {
    usgshrt_vec_t* frac = nullptr;
    ASSERT_EQ(usgshrt_vec_init(&frac), U_OK);
    ASSERT_EQ(usgshrt_vec_push_back(frac, 3), U_OK);

    rep_state_t rep;
    usgshrt_vec_t* digits = nullptr;
    size_t shift = 0;

    u_status_t st = has_a_finite_representation(frac, 10, 3, 3, &rep, &digits, &shift);
    ASSERT_EQ(st, U_OK);

    EXPECT_EQ(rep, REP_UNKNOWN);
    ASSERT_NE(digits, nullptr);
    EXPECT_GT(digits->size, 0);

    usgshrt_vec_free(&digits);
    usgshrt_vec_free(&frac);
}

//
// write_digits
//

TEST(WriteDigitsTest, WritesSequenceCorrectly) {
    auto v = make_vec({1, 2, 3});
    std::stringstream ss;
    FILE* tmp = tmpfile();
    ASSERT_NE(tmp, nullptr);

    write_digits(tmp, v);
    rewind(tmp);
    char buf[64] = {0};
    fread(buf, 1, sizeof(buf) - 1, tmp);
    fclose(tmp);

    EXPECT_STREQ(buf, "123");
    usgshrt_vec_free(&v);
}

//
// write_digits_with_cycle
//

TEST(WriteDigitsCycleTest, WritesWithParentheses) {
    auto v = make_vec({1, 2, 3});
    std::string out = capture_digits(write_digits_with_cycle, v, 1);
    EXPECT_EQ(out, "1(23)");
    usgshrt_vec_free(&v);
}
