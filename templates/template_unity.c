// ================================================================================
// This is a template file and is not meant for direct compilation.
// It is provided as a starting point for creating new C source files.
// Please copy this file as necessary to your project root.
// author: kristian.koribsky@student.tuke.sk
// ================================================================================
#include <stdbool.h>
#include "unity/src/unity.h"

// CONFIGURATION
// ================================================================================
#define UNITY_INCLUDE_PRINT_FORMATTED
#define UNITY_INCLUDE_CONFIG_H
#define UNITY_EXCLUDE_FLOAT
#define UNITY_INCLUDE_DOUBLE

#define UNITY_FLOAT_TYPE float16_t
#define UNITY_DOUBLE_TYPE long double
#define UNITY_FLOAT_PRECISION 0.001f

void setUp(void);
void tearDown(void);
void PRINT_FORMATTED(void);

// ASSERTIONS
// ================================================================================
extern bool ext_bool;
extern char ext_char;
extern int ext_int;
extern int ext_bin;
extern int ext_hex;
extern long ext_long;
extern float ext_float;
extern double ext_double;
extern int *ext_ptr;
extern int ext_array[64];
extern struct ext_struct
{
};

void test_general_assertions(void);
void test_bit_assertions(void);
void test_bool_assertions(void);
void test_char_assertions(void);
void test_int_assertions(void);
void test_long_assertions(void);
void test_float_assertions(void);
void test_double_assertions(void);
void test_array_assertions(void);
void test_struct_string_assertions(void);

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_general_assertions);
    RUN_TEST(test_bit_assertions);
    RUN_TEST(test_bool_assertions);
    RUN_TEST(test_char_assertions);
    RUN_TEST(test_int_assertions);
    RUN_TEST(test_long_assertions);
    RUN_TEST(test_float_assertions);
    RUN_TEST(test_float_assertions);
    RUN_TEST(test_double_assertions);
    RUN_TEST(test_array_assertions);
    RUN_TEST(test_struct_string_assertions);

    return UNITY_END();
}

void setUp(void)
{
    ext_bool = false;
    ext_char = '\0';
    ext_int = 0x5a5a;
    ext_bin = 0b0110;
    ext_hex = 0x5a5afff;
    ext_long = 185158418418l;
    ext_float = 0.584f;
    ext_double = 0.5484;
    ext_ptr = &ext_int;
    ext_array[0] = 0;
}

void tearDown(void)
{
}

void PRINT_FORMATTED(void)
{
    int a = 0xfab1;
    TEST_PRINTF("Decimal   %d\n", -7);
    TEST_PRINTF("Unsigned  %u\n", 987);
    TEST_PRINTF("Float     %f\n", 3.1415926535897932384);
    TEST_PRINTF("Binary    %b\n", 0xA);
    TEST_PRINTF("Hex       %X\n", 0xFAB);
    TEST_PRINTF("Pointer   %p\n", &a);
    TEST_PRINTF("Character %c\n", 'F');
    TEST_PRINTF("String    %s\n", "My string");
    TEST_PRINTF("Percent   %%\n");
    TEST_PRINTF("Unsigned long long %llu\n", 922337203685477580);
    TEST_PRINTF("Color Red \033[41mFAIL\033[0m\n");
    TEST_PRINTF("\n");
    TEST_PRINTF("Multiple (%d) (%i) (%u) (%x)\n", -100, 0, 200, 0x12345);
}

// https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md#basic-fail-pass-and-ignore
void test_general_assertions(void)
{
    TEST_MESSAGE("message");

    TEST_FAIL();
    TEST_FAIL_MESSAGE("message");

    TEST_PASS();
    TEST_PASS_MESSAGE("message");

    TEST_IGNORE();
    TEST_IGNORE_MESSAGE("message");
}

// https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md#boolean
void test_bool_assertions(void)
{
    TEST_ASSERT(ext_bool);

    TEST_ASSERT_TRUE(ext_bool);
    TEST_ASSERT_FALSE(ext_bool);

    TEST_ASSERT_NULL(ext_ptr);
    TEST_ASSERT_NOT_NULL(ext_ptr);

    TEST_ASSERT_EMPTY(ext_ptr);
    TEST_ASSERT_NOT_EMPTY(ext_ptr);
}

// https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md#characters
void test_char_assertions(void)
{
    TEST_ASSERT_EQUAL_CHAR(ext_char, ext_char);
}

// https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md#signed-and-unsigned-integers-of-all-sizes
void test_int_assertions(void)
{
    TEST_ASSERT_EQUAL_INT(ext_int, ext_int);
    TEST_ASSERT_EQUAL_INT8(ext_int, ext_int);
    TEST_ASSERT_EQUAL_INT64(ext_long, ext_long);

    TEST_ASSERT_EQUAL_UINT(ext_int, ext_int);
    TEST_ASSERT_EQUAL_UINT8(ext_int, ext_int);

    TEST_ASSERT_EQUAL_HEX(ext_hex, ext_hex);
    TEST_ASSERT_EQUAL_HEX8(ext_hex, ext_hex);

    TEST_ASSERT_GREATER_THAN_INT8(ext_int, ext_int);
    TEST_ASSERT_LESS_THAN_INT32(ext_int, ext_int);
    TEST_ASSERT_LESS_OR_EQUAL_UINT(ext_int, ext_int);

    TEST_ASSERT_INT_WITHIN(ext_int, ext_int, ext_int);
    TEST_ASSERT_INT8_WITHIN(ext_int, ext_int, ext_int);
    TEST_ASSERT_INT32_WITHIN(ext_int, ext_int, ext_int);

    TEST_ASSERT_UINT_WITHIN(ext_int, ext_int, ext_int);
    TEST_ASSERT_HEX_WITHIN(ext_hex, ext_hex, ext_hex);
}

// https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md#masked-and-bit-level-assertions
void test_bit_assertions(void)
{
    TEST_ASSERT_BITS(ext_bin, ext_bin, ext_bin);
    TEST_ASSERT_BITS_HIGH(ext_bin, ext_bin);
    TEST_ASSERT_BITS_LOW(ext_bin, ext_bin);
    TEST_ASSERT_BIT_HIGH(ext_bin, ext_bin);
    TEST_ASSERT_BIT_LOW(ext_bin, ext_bin);
}

// https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md#floating-point-if-enabled
void test_float_assertions(void)
{
    TEST_ASSERT_FLOAT_WITHIN(ext_float, ext_float, ext_float);
    TEST_ASSERT_FLOAT_NOT_WITHIN(ext_float, ext_float, ext_float);
    TEST_ASSERT_EQUAL_FLOAT(ext_float, ext_float);
    TEST_ASSERT_NOT_EQUAL_FLOAT(ext_float, ext_float);

    TEST_ASSERT_FLOAT_IS_INF(ext_float);
    TEST_ASSERT_FLOAT_IS_NAN(ext_float);

    // TEST_ASSERT_FLOAT_ARRAY_WITHIN(delta, expected, actual, num_elements)
    // TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected, actual, num_elements)
}

// https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md#double-if-enabled
void test_double_assertions(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(ext_double, ext_double, ext_double);
    TEST_ASSERT_DOUBLE_NOT_WITHIN(ext_double, ext_double, ext_double);
    TEST_ASSERT_EQUAL_DOUBLE(ext_double, ext_double);
    TEST_ASSERT_NOT_EQUAL_DOUBLE(ext_double, ext_double);

    TEST_ASSERT_DOUBLE_IS_INF(ext_double);
    TEST_ASSERT_DOUBLE_IS_NAN(ext_double);

    // TEST_ASSERT_DOUBLE_ARRAY_WITHIN (delta, expected, actual, num_elements)
    // TEST_ASSERT_EQUAL_DOUBLE_ARRAY (expected, actual, num_elements)
}

// https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md#arrays
void test_array_assertions(void)
{
    TEST_ASSERT_EQUAL_INT_ARRAY(ext_array, ext_array, ext_array);
    TEST_ASSERT_EQUAL_INT64_ARRAY(ext_array, ext_array, ext_array);
    TEST_ASSERT_EQUAL_UINT_ARRAY(ext_array, ext_array, ext_array);

    TEST_ASSERT_EQUAL_HEX_ARRAY(ext_array, ext_array, ext_array);

    TEST_ASSERT_INT_ARRAY_WITHIN(ext_array, ext_array, ext_array, ext_array);
    TEST_ASSERT_EACH_EQUAL_INT(ext_int, ext_array, ext_array);
}

// https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md#structs-and-strings
void test_struct_string_assertions(void)
{
    TEST_ASSERT_EQUAL_PTR(ext_ptr, ext_ptr);

    TEST_ASSERT_EQUAL_STRING(ext_ptr, ext_ptr);

    TEST_ASSERT_EQUAL_MEMORY(ext_ptr, ext_ptr, ext_int);
}