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

void test_parse_input_correctly_parses_command_inspect(void);
void test_parse_input_correctly_parses_command_take(void);
void test_parse_input_correctly_parses_command_put(void);
void test_parse_input_correctly_parses_command_use(void);
void test_parse_input_correctly_parses_command_save(void);
void test_parse_input_correctly_parses_command_load(void);

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

    RUN_TEST(test_parse_input_correctly_parses_command_inspect);
    RUN_TEST(test_parse_input_correctly_parses_command_take);
    RUN_TEST(test_parse_input_correctly_parses_command_put);
    RUN_TEST(test_parse_input_correctly_parses_command_use);
    RUN_TEST(test_parse_input_correctly_parses_command_save);
    RUN_TEST(test_parse_input_correctly_parses_command_load);

    return UNITY_END();
}

struct parser *parser = NULL;

#define COMMAND_ENTIRE 0 // groups index of whole command
#define COMMAND_PARAM 3  // group index of param part

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

void test_parse_input_correctly_parses_command_inspect(void)
{
    struct command *cmd = parse_input(parser, "   preskUmaJ horcica    ");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("PRESKUMAJ", cmd->name);
    TEST_ASSERT_EQUAL_STRING("horcica", cmd->groups[COMMAND_PARAM]);
}

void test_parse_input_correctly_parses_command_take(void)
{
    struct command *cmd = parse_input(parser, "VEZMi zlte tricko   \n");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("VEZMI", cmd->name);
    TEST_ASSERT_EQUAL_STRING("zlte tricko", cmd->groups[COMMAND_PARAM]);
}

void test_parse_input_correctly_parses_command_put(void)
{
    struct command *cmd = parse_input(parser, "\tpoloz NIC NOVE\n");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("POLOZ", cmd->name);
    TEST_ASSERT_EQUAL_STRING("NIC NOVE", cmd->groups[COMMAND_PARAM]);
}

void test_parse_input_correctly_parses_command_use(void)
{
    struct command *cmd = parse_input(parser, "\tPOUZI");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("POUZI", cmd->name);
    TEST_ASSERT_NULL(cmd->groups[COMMAND_PARAM]);
}
void test_parse_input_correctly_parses_command_save(void)
{
    struct command *cmd = parse_input(parser, "\n\tsave ");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("ULOZ", cmd->name);
    TEST_ASSERT_NULL(cmd->groups[COMMAND_PARAM]);
}
void test_parse_input_correctly_parses_command_load(void)
{
    struct command *cmd = parse_input(parser, "\t\rload file.txt        ");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("NAHRAJ", cmd->name);
    TEST_ASSERT_EQUAL_STRING("file.txt", cmd->groups[COMMAND_PARAM]);
}