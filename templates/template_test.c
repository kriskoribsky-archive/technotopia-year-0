// ================================================================================
// This is a template file and is not meant for direct compilation.
// It is provided as a starting point for creating new C source files.
// Please copy this file as necessary to your project root.
// author: kristian.koribsky@student.tuke.sk
// ================================================================================
#include "unity/src/unity.h"
#include "utils/src/stdiotest.h"
#include "utils/src/macroutils.h"

#include "main.c"

#define TARGET main
#define DIR_IN "tests/testcases/"
#define DIR_OUT "build/tests/out/"

void setUp(void);
void tearDown(void);

void test_example(void);

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_example);

    return UNITY_END();
}

void setUp(void)
{
}

void tearDown(void)
{
}

void test_example(void)
{
    TEST_STDIO("input.txt", "expected.txt", "output.txt");
}