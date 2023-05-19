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

void test_parse_input_correctly_parses_command_inspect_with_whitespace(void);
void test_parse_input_correctly_parses_command_take_with_whitespace(void);
void test_parse_input_correctly_parses_command_put_with_whitespace(void);
void test_parse_input_correctly_parses_command_use_with_whitespace(void);
void test_parse_input_correctly_parses_command_save_with_whitespace(void);
void test_parse_input_correctly_parses_command_load_with_whitespace(void);

void test_parse_input_correctly_parses_exact_predefined_commands_0(void);
void test_parse_input_correctly_parses_exact_predefined_commands_1(void);
void test_parse_input_correctly_parses_exact_predefined_commands_2(void);
void test_parse_input_correctly_parses_exact_predefined_commands_3(void);
void test_parse_input_correctly_parses_exact_predefined_commands_4(void);
void test_parse_input_correctly_parses_exact_predefined_commands_5(void);
void test_parse_input_correctly_parses_exact_predefined_commands_6(void);
void test_parse_input_correctly_parses_exact_predefined_commands_7(void);
void test_parse_input_correctly_parses_exact_predefined_commands_8(void);
void test_parse_input_correctly_parses_exact_predefined_commands_9(void);
void test_parse_input_correctly_parses_exact_predefined_commands_10(void);

void test_parse_input_correctly_parses_exact_predefined_commands_11(void);
void test_parse_input_correctly_parses_exact_predefined_commands_12(void);
void test_parse_input_correctly_parses_exact_predefined_commands_13(void);
void test_parse_input_correctly_parses_exact_predefined_commands_14(void);
void test_parse_input_correctly_parses_exact_predefined_commands_15(void);
void test_parse_input_correctly_parses_exact_predefined_commands_16(void);
void test_parse_input_correctly_parses_exact_predefined_commands_17(void);
void test_parse_input_correctly_parses_exact_predefined_commands_18(void);
void test_parse_input_correctly_parses_exact_predefined_commands_19(void);
void test_parse_input_correctly_parses_exact_predefined_commands_20(void);
void test_parse_input_correctly_parses_exact_predefined_commands_21(void);

void test_parse_input_corretly_parses_case_insensitive_predefined_commands_0(void);
void test_parse_input_corretly_parses_case_insensitive_predefined_commands_1(void);
void test_parse_input_corretly_parses_case_insensitive_predefined_commands_2(void);
void test_parse_input_corretly_parses_case_insensitive_predefined_commands_3(void);
void test_parse_input_corretly_parses_case_insensitive_predefined_commands_4(void);
void test_parse_input_corretly_parses_case_insensitive_predefined_commands_5(void);
void test_parse_input_corretly_parses_case_insensitive_predefined_commands_6(void);
void test_parse_input_corretly_parses_case_insensitive_predefined_commands_7(void);
void test_parse_input_corretly_parses_case_insensitive_predefined_commands_8(void);
void test_parse_input_corretly_parses_case_insensitive_predefined_commands_9(void);
void test_parse_input_corretly_parses_case_insensitive_predefined_commands_10(void);

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

    RUN_TEST(test_parse_input_correctly_parses_command_inspect_with_whitespace);
    RUN_TEST(test_parse_input_correctly_parses_command_take_with_whitespace);
    RUN_TEST(test_parse_input_correctly_parses_command_put_with_whitespace);
    RUN_TEST(test_parse_input_correctly_parses_command_use_with_whitespace);
    RUN_TEST(test_parse_input_correctly_parses_command_save_with_whitespace);
    RUN_TEST(test_parse_input_correctly_parses_command_load_with_whitespace);

    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_0);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_1);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_2);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_3);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_4);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_5);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_6);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_7);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_8);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_9);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_10);

    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_11);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_12);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_13);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_14);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_15);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_16);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_17);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_18);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_19);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_20);
    RUN_TEST(test_parse_input_correctly_parses_exact_predefined_commands_21);

    RUN_TEST(test_parse_input_corretly_parses_case_insensitive_predefined_commands_0);
    RUN_TEST(test_parse_input_corretly_parses_case_insensitive_predefined_commands_1);
    RUN_TEST(test_parse_input_corretly_parses_case_insensitive_predefined_commands_2);
    RUN_TEST(test_parse_input_corretly_parses_case_insensitive_predefined_commands_3);
    RUN_TEST(test_parse_input_corretly_parses_case_insensitive_predefined_commands_4);
    RUN_TEST(test_parse_input_corretly_parses_case_insensitive_predefined_commands_5);
    RUN_TEST(test_parse_input_corretly_parses_case_insensitive_predefined_commands_6);
    RUN_TEST(test_parse_input_corretly_parses_case_insensitive_predefined_commands_7);
    RUN_TEST(test_parse_input_corretly_parses_case_insensitive_predefined_commands_8);
    RUN_TEST(test_parse_input_corretly_parses_case_insensitive_predefined_commands_9);
    RUN_TEST(test_parse_input_corretly_parses_case_insensitive_predefined_commands_10);

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

void test_parse_input_correctly_parses_command_inspect_with_whitespace(void)
{
    struct command *cmd = parse_input(parser, "   preskUmaJ horcica    ");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("PRESKUMAJ", cmd->name);
    TEST_ASSERT_EQUAL_STRING("horcica", cmd->groups[COMMAND_PARAM]);
}

void test_parse_input_correctly_parses_command_take_with_whitespace(void)
{
    struct command *cmd = parse_input(parser, "VEZMi zlte tricko   \n");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("VEZMI", cmd->name);
    TEST_ASSERT_EQUAL_STRING("zlte tricko", cmd->groups[COMMAND_PARAM]);
}

void test_parse_input_correctly_parses_command_put_with_whitespace(void)
{
    struct command *cmd = parse_input(parser, "\tpoloz NIC NOVE\n");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("POLOZ", cmd->name);
    TEST_ASSERT_EQUAL_STRING("NIC NOVE", cmd->groups[COMMAND_PARAM]);
}

void test_parse_input_correctly_parses_command_use_with_whitespace(void)
{
    struct command *cmd = parse_input(parser, "\tPOUZI");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("POUZI", cmd->name);
    TEST_ASSERT_NULL(cmd->groups[COMMAND_PARAM]);
}
void test_parse_input_correctly_parses_command_save_with_whitespace(void)
{
    struct command *cmd = parse_input(parser, "\n\tsave ");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("ULOZ", cmd->name);
    TEST_ASSERT_NULL(cmd->groups[COMMAND_PARAM]);
}
void test_parse_input_correctly_parses_command_load_with_whitespace(void)
{
    struct command *cmd = parse_input(parser, "\t\rload file.txt        ");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("NAHRAJ", cmd->name);
    TEST_ASSERT_EQUAL_STRING("file.txt", cmd->groups[COMMAND_PARAM]);
}

void test_parse_input_correctly_parses_exact_predefined_commands_0(void)
{
    char *input = "KONIEC";
    TEST_ASSERT_NOT_NULL(parse_input(parser, input));
}

void test_parse_input_correctly_parses_exact_predefined_commands_1(void)
{
    char *input = "SEVER";
    TEST_ASSERT_NOT_NULL(parse_input(parser, input));
}

void test_parse_input_correctly_parses_exact_predefined_commands_2(void)
{
    char *input = "JUH";
    TEST_ASSERT_NOT_NULL(parse_input(parser, input));
}

void test_parse_input_correctly_parses_exact_predefined_commands_3(void)
{
    char *input = "VYCHOD";
    TEST_ASSERT_NOT_NULL(parse_input(parser, input));
}

void test_parse_input_correctly_parses_exact_predefined_commands_4(void)
{
    char *input = "ZAPAD";
    TEST_ASSERT_NOT_NULL(parse_input(parser, input));
}

void test_parse_input_correctly_parses_exact_predefined_commands_5(void)
{
    char *input = "ROZHLIADNI SA";
    TEST_ASSERT_NOT_NULL(parse_input(parser, input));
}

void test_parse_input_correctly_parses_exact_predefined_commands_6(void)
{
    char *input = "PRIKAZY";
    TEST_ASSERT_NOT_NULL(parse_input(parser, input));
}

void test_parse_input_correctly_parses_exact_predefined_commands_7(void)
{
    char *input = "VERZIA";
    TEST_ASSERT_NOT_NULL(parse_input(parser, input));
}

void test_parse_input_correctly_parses_exact_predefined_commands_8(void)
{
    char *input = "RESTART";
    TEST_ASSERT_NOT_NULL(parse_input(parser, input));
}

void test_parse_input_correctly_parses_exact_predefined_commands_9(void)
{
    char *input = "O HRE";
    TEST_ASSERT_NOT_NULL(parse_input(parser, input));
}

void test_parse_input_correctly_parses_exact_predefined_commands_10(void)
{
    char *input = "INVENTAR";
    TEST_ASSERT_NOT_NULL(parse_input(parser, input));
}

void test_parse_input_correctly_parses_exact_predefined_commands_11(void)
{
    struct command *command = parse_input(parser, "KONIEC");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_correctly_parses_exact_predefined_commands_12(void)
{
    struct command *command = parse_input(parser, "SEVER");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_correctly_parses_exact_predefined_commands_13(void)
{
    struct command *command = parse_input(parser, "JUH");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_correctly_parses_exact_predefined_commands_14(void)
{
    struct command *command = parse_input(parser, "VYCHOD");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_correctly_parses_exact_predefined_commands_15(void)
{
    struct command *command = parse_input(parser, "ZAPAD");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_correctly_parses_exact_predefined_commands_16(void)
{
    struct command *command = parse_input(parser, "ROZHLIADNI SA");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_correctly_parses_exact_predefined_commands_17(void)
{
    struct command *command = parse_input(parser, "PRIKAZY");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_correctly_parses_exact_predefined_commands_18(void)
{
    struct command *command = parse_input(parser, "VERZIA");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_correctly_parses_exact_predefined_commands_19(void)
{
    struct command *command = parse_input(parser, "RESTART");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_correctly_parses_exact_predefined_commands_20(void)
{
    struct command *command = parse_input(parser, "O HRE");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_correctly_parses_exact_predefined_commands_21(void)
{
    struct command *command = parse_input(parser, "INVENTAR");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_corretly_parses_case_insensitive_predefined_commands_0(void)
{
    struct command *command = parse_input(parser, "KONIeC");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_corretly_parses_case_insensitive_predefined_commands_1(void)
{
    struct command *command = parse_input(parser, "sevER");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_corretly_parses_case_insensitive_predefined_commands_2(void)
{
    struct command *command = parse_input(parser, "jUh");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_corretly_parses_case_insensitive_predefined_commands_3(void)
{
    struct command *command = parse_input(parser, "vyChOD");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_corretly_parses_case_insensitive_predefined_commands_4(void)
{
    struct command *command = parse_input(parser, "zApaD");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_corretly_parses_case_insensitive_predefined_commands_5(void)
{
    struct command *command = parse_input(parser, "rOZHLiAdNi sa");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_corretly_parses_case_insensitive_predefined_commands_6(void)
{
    struct command *command = parse_input(parser, "prIkAzy");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_corretly_parses_case_insensitive_predefined_commands_7(void)
{
    struct command *command = parse_input(parser, "VerZIA");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_corretly_parses_case_insensitive_predefined_commands_8(void)
{
    struct command *command = parse_input(parser, "rEStarT");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_corretly_parses_case_insensitive_predefined_commands_9(void)
{
    struct command *command = parse_input(parser, "O hRe");
    TEST_ASSERT_NOT_NULL(command);
}

void test_parse_input_corretly_parses_case_insensitive_predefined_commands_10(void)
{
    struct command *command = parse_input(parser, "inveNtaR");
    TEST_ASSERT_NOT_NULL(command);
}
