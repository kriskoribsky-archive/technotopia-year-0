#include "../unity/src/unity.h"

#include "command.h"

void setUp(void);
void tearDown(void);

void test_create_command_handles_null_name(void);
void test_create_command_handles_null_description(void);
void test_create_command_handles_null_null_pattern(void);

struct command *cmd = NULL;

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_create_command_handles_null_name);
    RUN_TEST(test_create_command_handles_null_description);
    RUN_TEST(test_create_command_handles_null_null_pattern);

    return UNITY_END();
}

void setUp(void)
{
}

void tearDown(void)
{
}

void test_create_command_handles_null_name(void)
{
    TEST_ASSERT_NULL(create_command(NULL, "desc", "(pattern)", 2));
}

void test_create_command_handles_null_description(void)
{
    TEST_ASSERT_NULL(create_command("name", NULL, "(pattern)", 2));
}

void test_create_command_handles_null_null_pattern(void)
{
    TEST_ASSERT_NOT_NULL(create_command("PRIKAZY", "Zobrazi zoznam prikazov.", NULL, 0));
}