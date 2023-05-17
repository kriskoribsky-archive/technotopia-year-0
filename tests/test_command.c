#include <strings.h>
#include "../unity/src/unity.h"

#include "command.h"

void setUp(void);
void tearDown(void);

void test_create_command_handles_null_name(void);
void test_create_command_handles_null_description(void);
void test_create_command_handles_null_null_pattern(void);
void test_create_command_handles_empty_name(void);
void test_create_command_handles_empty_description(void);

void test_create_command_doesnt_change_name_or_description_1(void);
void test_create_command_doesnt_change_name_or_description_2(void);
void test_create_command_doesnt_change_name_or_description_3(void);
void test_create_command_doesnt_change_name_or_description_4(void);
void test_create_command_doesnt_change_name_or_description_5(void);
void test_create_command_doesnt_change_name_or_description_6(void);

struct command *cmd = NULL;

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_create_command_handles_null_name);
    RUN_TEST(test_create_command_handles_null_description);
    RUN_TEST(test_create_command_handles_null_null_pattern);
    RUN_TEST(test_create_command_handles_empty_name);
    RUN_TEST(test_create_command_handles_empty_description);

    RUN_TEST(test_create_command_doesnt_change_name_or_description_1);
    RUN_TEST(test_create_command_doesnt_change_name_or_description_2);
    RUN_TEST(test_create_command_doesnt_change_name_or_description_3);
    RUN_TEST(test_create_command_doesnt_change_name_or_description_4);
    RUN_TEST(test_create_command_doesnt_change_name_or_description_5);
    RUN_TEST(test_create_command_doesnt_change_name_or_description_6);

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

void test_create_command_handles_empty_name(void)
{
    TEST_ASSERT_NULL(create_command("", "Prejst na juh.", NULL, 0));
}
void test_create_command_handles_empty_description(void)
{
    TEST_ASSERT_NULL(create_command("VYCHOD", "", NULL, 0));
}

void test_create_command_doesnt_change_name_or_description_1(void)
{
    char *name = "ROZHLIADNI SA";
    char *description = "Zobrazi opis miestnosti.";
    struct command *command = create_command(name, description, NULL, 0);
    TEST_ASSERT_TRUE(strcasecmp(name, command->name) == 0 && strcasecmp(description, command->description) == 0);
    destroy_command(command);
}
void test_create_command_doesnt_change_name_or_description_2(void)
{
    char *name = "PRESKUMAJ";
    char *description = "Preskuma zvoleny predmet.";
    struct command *command = create_command(name, description, NULL, 0);
    TEST_ASSERT_TRUE(strcasecmp(name, command->name) == 0 && strcasecmp(description, command->description) == 0);
    destroy_command(command);
}
void test_create_command_doesnt_change_name_or_description_3(void)
{
    char *name = "PRIKAZY";
    char *description = "Zobrazi zoznam prikazov.";
    struct command *command = create_command(name, description, NULL, 0);
    TEST_ASSERT_TRUE(strcasecmp(name, command->name) == 0 && strcasecmp(description, command->description) == 0);
    destroy_command(command);
}
void test_create_command_doesnt_change_name_or_description_4(void)
{
    char *name = "POLOZ";
    char *description = "Polozi predmet do miestnosti.";
    struct command *command = create_command(name, description, NULL, 0);
    TEST_ASSERT_TRUE(strcasecmp(name, command->name) == 0 && strcasecmp(description, command->description) == 0);
    destroy_command(command);
}
void test_create_command_doesnt_change_name_or_description_5(void)
{
    char *name = "ULOZ";
    char *description = "Ulozi aktualny stav hry do suboru.";
    struct command *command = create_command(name, description, NULL, 0);
    TEST_ASSERT_TRUE(strcasecmp(name, command->name) == 0 && strcasecmp(description, command->description) == 0);
    destroy_command(command);
}
void test_create_command_doesnt_change_name_or_description_6(void)
{
    char *name = "KONIEC";
    char *description = "Ukonci hru.";
    struct command *command = create_command(name, description, NULL, 0);
    TEST_ASSERT_TRUE(strcasecmp(name, command->name) == 0 && strcasecmp(description, command->description) == 0);
    destroy_command(command);
}