#include "../unity/src/unity.h"

#include "parser.h"

void setUp(void);
void tearDown(void);

void test_parse_input_handles_null_parser(void);
void test_parse_input_handles_null_input(void);
void test_parse_input_handles_empty_input(void);

void test_parse_input_handles_uknown_input(void);

void test_parse_input_handles_whitespace_before(void);
void test_parse_input_handles_whitespace_after(void);
void test_parse_input_handles_whitespace_before_after(void);

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_parse_input_handles_null_parser);
    RUN_TEST(test_parse_input_handles_null_input);
    RUN_TEST(test_parse_input_handles_empty_input);

    RUN_TEST(test_parse_input_handles_uknown_input);

    RUN_TEST(test_parse_input_handles_whitespace_before);
    RUN_TEST(test_parse_input_handles_whitespace_after);
    RUN_TEST(test_parse_input_handles_whitespace_before_after);

    return UNITY_END();
}

struct parser *parser = NULL;

void setUp(void)
{
    parser = create_parser();
}

void tearDown(void)
{
    parser = destroy_parser(parser);
}

void test_parse_input_handles_null_parser(void)
{
    TEST_ASSERT_NULL(parse_input(NULL, "VERZIA"));
}

void test_parse_input_handles_null_input(void)
{
    TEST_ASSERT_NULL(parse_input(parser, NULL));
}

void test_parse_input_handles_empty_input(void)
{
    TEST_ASSERT_NULL(parse_input(parser, ""));
}

void test_parse_input_handles_uknown_input(void)
{
    TEST_ASSERT_NULL(parse_input(parser, "   CO JE V MIESTNOSTI?  "));
}

void test_parse_input_handles_whitespace_before(void)
{
    struct command *cmd = parse_input(parser, "     VeRzIA");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("VERZIA", cmd->name);
}

void test_parse_input_handles_whitespace_after(void)
{
    struct command *cmd = parse_input(parser, "HeLp              ");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("PRIKAZY", cmd->name);
}

void test_parse_input_handles_whitespace_before_after(void)
{
    struct command *cmd = parse_input(parser, "         s        ");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("SEVER", cmd->name);
}