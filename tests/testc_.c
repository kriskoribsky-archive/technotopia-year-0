#include "../unity/src/unity.h"
#include "../utils/src/stdiotest.h"
#include "../utils/src/macrostring.h"

#define TARGET
#define DIR_IN
#define DIR_OUT

#define TEST_STDIO(i, e, o) TEST_ASSERT_TRUE(test_main_stdio(TARGET, JOIN(DIR_IN, i), JOIN(DIR_IN, e), JOIN(DIR_OUT, o)))

void setUp(void);
void tearDown(void);

int main(void)
{
    UNITY_BEGIN();

    return UNITY_END();
}

void setUp(void)
{
}

void tearDown(void)
{
}