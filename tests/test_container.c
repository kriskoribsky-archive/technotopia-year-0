#define _POSIX_C_SOURCE 200809L // strdup
#include <stdlib.h>
#include <string.h>
#include "unity/src/unity.h"

#include "container.h"

void setUp(void);
void tearDown(void);

void test_create_container_works_for_type_room(void);
void test_create_container_works_for_type_item(void);
void test_create_container_works_for_type_command(void);
void test_create_container_works_for_type_text(void);
void test_create_container_doesnt_set_first_implicitly(void);
void test_create_container_returns_null_on_null_entry(void);
void test_create_container_returns_null_on_different_entry_type(void);
void test_create_container_appends_entry_on_same_entry_type(void);

void test_destroy_containers_handles_null_input(void);
void test_destroy_containers_sets_pointer_to_null(void);

void test_get_from_container_by_name_works_for_type_room(void);
void test_get_from_container_by_name_works_for_type_item(void);
void test_get_from_container_by_name_works_for_type_command(void);
void test_get_from_container_by_name_works_for_type_text(void);
void test_get_from_container_by_name_returns_null_on_null_container(void);
void test_get_from_container_by_name_returns_null_on_null_name(void);
void test_get_from_container_by_name_returns_null_on_invalid_name(void);

void test_remove_container_works_for_type_room(void);
void test_remove_container_works_for_type_item(void);
void test_remove_container_works_for_type_command(void);
void test_remove_container_works_for_type_text(void);
void test_remove_container_doesnt_set_first_implicitly(void);
void test_remove_container_removes_first_entry(void);
void test_remove_container_returns_null_on_last_entry(void);
void test_remove_container_content_of_container_stays(void);
void test_remove_container_returns_null_on_null_first(void);
void test_remove_container_returns_first_on_null_entry(void);

#define TEXT_LEN 32

struct container *c = NULL;

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_create_container_works_for_type_room);
    RUN_TEST(test_create_container_works_for_type_item);
    RUN_TEST(test_create_container_works_for_type_command);
    RUN_TEST(test_create_container_works_for_type_text);
    RUN_TEST(test_create_container_doesnt_set_first_implicitly);
    RUN_TEST(test_create_container_returns_null_on_null_entry);
    RUN_TEST(test_create_container_returns_null_on_different_entry_type);
    RUN_TEST(test_create_container_appends_entry_on_same_entry_type);

    RUN_TEST(test_destroy_containers_handles_null_input);
    RUN_TEST(test_destroy_containers_sets_pointer_to_null);

    RUN_TEST(test_get_from_container_by_name_works_for_type_room);
    RUN_TEST(test_get_from_container_by_name_works_for_type_item);
    RUN_TEST(test_get_from_container_by_name_works_for_type_command);
    RUN_TEST(test_get_from_container_by_name_works_for_type_text);
    RUN_TEST(test_get_from_container_by_name_returns_null_on_null_container);
    RUN_TEST(test_get_from_container_by_name_returns_null_on_null_name);
    RUN_TEST(test_get_from_container_by_name_returns_null_on_invalid_name);

    RUN_TEST(test_remove_container_works_for_type_room);
    RUN_TEST(test_remove_container_works_for_type_item);
    RUN_TEST(test_remove_container_works_for_type_command);
    RUN_TEST(test_remove_container_works_for_type_text);
    RUN_TEST(test_remove_container_doesnt_set_first_implicitly);
    RUN_TEST(test_remove_container_removes_first_entry);
    RUN_TEST(test_remove_container_returns_null_on_last_entry);
    RUN_TEST(test_remove_container_content_of_container_stays);
    RUN_TEST(test_remove_container_returns_null_on_null_first);
    RUN_TEST(test_remove_container_returns_first_on_null_entry);

    return UNITY_END();
}

void setUp(void)
{
}

void tearDown(void)
{
    c = destroy_containers(c);
}

void test_create_container_works_for_type_room(void)
{
    c = create_container(NULL, ROOM, create_room("room_name", "room_description"));
    TEST_ASSERT_EQUAL(ROOM, c->type);
}

void test_create_container_works_for_type_item(void)
{
    c = create_container(NULL, ITEM, create_item("item_name", "item_descritpion", 0));
    TEST_ASSERT_EQUAL(ITEM, c->type);
}

void test_create_container_works_for_type_command(void)
{
    c = create_container(NULL, COMMAND, create_command("cmd_name", "cmd_description", "pattern", 0));
    TEST_ASSERT_EQUAL(COMMAND, c->type);
}

void test_create_container_works_for_type_text(void)
{
    c = create_container(NULL, TEXT, strdup("text"));
    TEST_ASSERT_EQUAL(TEXT, c->type);
}

void test_create_container_doesnt_set_first_implicitly(void)
{
    c = NULL;
    create_container(c, TEXT, strdup("text"));
    TEST_ASSERT_NULL(c);
}

void test_create_container_returns_null_on_null_entry(void)
{
    c = create_container(NULL, ROOM, NULL);
    TEST_ASSERT_NULL(c);
}

void test_create_container_returns_null_on_different_entry_type(void)
{
    c = create_container(NULL, TEXT, strdup("text"));
    TEST_ASSERT_NULL(create_container(c, ROOM, create_room("room_name", "room_description")));
}

void test_create_container_appends_entry_on_same_entry_type(void)
{
    c = create_container(NULL, TEXT, strdup("test text 1"));
    create_container(c, TEXT, strdup("test text 2"));
    create_container(c, TEXT, strdup("test text 3"));
    create_container(c, TEXT, strdup("test text 4"));

    TEST_ASSERT_EQUAL_STRING("test text 1", c->text);
    TEST_ASSERT_EQUAL_STRING("test text 2", c->next->text);
    TEST_ASSERT_EQUAL_STRING("test text 3", c->next->next->text);
    TEST_ASSERT_EQUAL_STRING("test text 4", c->next->next->next->text);
}

void test_destroy_containers_handles_null_input(void)
{
    TEST_ASSERT_NULL(destroy_containers(NULL));
}

void test_destroy_containers_sets_pointer_to_null(void)
{
    char *text1 = malloc(TEXT_LEN * sizeof(char));

    struct container *local = create_container(NULL, TEXT, strdup("test"));

    local = destroy_containers(local);

    TEST_ASSERT_NULL(local);
}

void test_get_from_container_by_name_works_for_type_room(void)
{
    struct room *target_room = create_room("room 3", "room desc 3");

    c = create_container(NULL, ROOM, create_room("room 1", "room desc 1"));
    create_container(c, ROOM, create_room("room 2", "room desc 2"));
    create_container(c, ROOM, target_room);
    create_container(c, ROOM, create_room("room 4", "room desc 4"));
    create_container(c, ROOM, create_room("room 5", "room desc 5"));

    // equal pointer & memory
    TEST_ASSERT_EQUAL_PTR(target_room, get_from_container_by_name(c, "room 3"));
    TEST_ASSERT_EQUAL_MEMORY(target_room, get_from_container_by_name(c, "room 3"), sizeof(target_room));
}

void test_get_from_container_by_name_works_for_type_item(void)
{
    struct item *target_item = create_item("item 1", "item desc 1", 0);

    c = create_container(NULL, ITEM, target_item);
    create_container(c, ITEM, create_item("item 2", "item desc 2", 0));
    create_container(c, ITEM, create_item("item 3", "item desc 3", 0));
    create_container(c, ITEM, create_item("item 4", "item desc 4", 0));
    create_container(c, ITEM, create_item("item 5", "item desc 5", 0));

    // equal pointer & memory
    TEST_ASSERT_EQUAL_PTR(target_item, get_from_container_by_name(c, "item 1"));
    TEST_ASSERT_EQUAL_MEMORY(target_item, get_from_container_by_name(c, "item 1"), sizeof(target_item));
}

void test_get_from_container_by_name_works_for_type_command(void)
{
    struct command *target_command = create_command("cmd 2", "cmd desc 2", "dummy 2", 1);

    c = create_container(NULL, COMMAND, create_command("cmd 1", "cmd desc 1", "dummy 1", 1));
    create_container(c, COMMAND, target_command);
    create_container(c, COMMAND, create_command("cmd 3", "cmd desc 3", "dummy 3", 1));
    create_container(c, COMMAND, create_command("cmd 4", "cmd desc 4", "dummy 4", 1));
    create_container(c, COMMAND, create_command("cmd 3", "cmd desc 3", "dummy 5", 1));

    // equal pointer & memory
    TEST_ASSERT_EQUAL_PTR(target_command, get_from_container_by_name(c, "cmd 2"));
    TEST_ASSERT_EQUAL_MEMORY(target_command, get_from_container_by_name(c, "cmd 2"), sizeof(target_command));
}

void test_get_from_container_by_name_works_for_type_text(void)
{
    char *text5 = strdup("text 5");

    c = create_container(NULL, TEXT, strdup("text 1"));
    create_container(c, TEXT, strdup("text 2"));
    create_container(c, TEXT, strdup("text 3"));
    create_container(c, TEXT, strdup("text 4"));
    create_container(c, TEXT, text5);

    // equal pointer & memory
    TEST_ASSERT_EQUAL_PTR(text5, get_from_container_by_name(c, "text 5"));
    TEST_ASSERT_EQUAL_MEMORY(text5, get_from_container_by_name(c, "text 5"), 7 * sizeof(char));
}

void test_get_from_container_by_name_returns_null_on_null_container(void)
{
    TEST_ASSERT_NULL(get_from_container_by_name(NULL, "test name"));
}

void test_get_from_container_by_name_returns_null_on_null_name(void)
{

    c = create_container(NULL, TEXT, strdup("test"));
    TEST_ASSERT_NULL(get_from_container_by_name(c, NULL));
}

void test_get_from_container_by_name_returns_null_on_invalid_name(void)
{
    c = create_container(NULL, TEXT, strdup("text 1"));
    create_container(c, TEXT, strdup("text 2"));
    create_container(c, TEXT, strdup("text 3"));
    create_container(c, TEXT, strdup("text 4"));
    create_container(c, TEXT, strdup("text 5"));

    TEST_ASSERT_NULL(get_from_container_by_name(c, "text 69"));
}

void test_remove_container_works_for_type_room(void)
{
    struct room *target_room = create_room("room 3", "room desc 3");
    struct room *next_room = create_room("room 4", "room desc 4");

    c = create_container(NULL, ROOM, create_room("room 1", "room desc 1"));
    create_container(c, ROOM, create_room("room 2", "room desc 2"));
    create_container(c, ROOM, target_room);
    create_container(c, ROOM, next_room);
    create_container(c, ROOM, create_room("room 5", "room desc 5"));

    // test pointer & memory equality
    TEST_ASSERT_NOT_EQUAL(next_room, c->next->next->room);
    TEST_ASSERT_NOT_EQUAL(target_room, remove_container(c, target_room)->next->next->room);
    TEST_ASSERT_EQUAL_PTR(next_room, c->next->next->room);
    TEST_ASSERT_EQUAL_MEMORY(next_room, c->next->next->room, sizeof(next_room));
}

void test_remove_container_works_for_type_item(void)
{
    struct item *target_item = create_item("item 2", "item desc 2", 0);
    struct item *next_item = create_item("item 3", "item desc 3", 0);

    c = create_container(NULL, ITEM, create_item("item 1", "item desc 1", 0));
    create_container(c, ITEM, target_item);
    create_container(c, ITEM, next_item);
    create_container(c, ITEM, create_item("item 4", "item desc 4", 0));
    create_container(c, ITEM, create_item("item 5", "item desc 5", 0));

    // test pointer & memory equality
    TEST_ASSERT_NOT_EQUAL(next_item, c->next->item);
    TEST_ASSERT_NOT_EQUAL(target_item, remove_container(c, target_item)->next->item);
    TEST_ASSERT_EQUAL_PTR(next_item, c->next->item);
    TEST_ASSERT_EQUAL_MEMORY(next_item, c->next->item, sizeof(next_item));
}

void test_remove_container_works_for_type_command(void)
{
    struct command *target_command = create_command("cmd 2", "cmd desc 2", "dummy 2", 1);
    struct command *next_command = create_command("cmd 3", "cmd desc 3", "dummy 3", 1);

    c = create_container(NULL, COMMAND, create_command("cmd 1", "cmd desc 1", "dummy 1", 1));
    create_container(c, COMMAND, target_command);
    create_container(c, COMMAND, next_command);
    create_container(c, COMMAND, create_command("cmd 4", "cmd desc 4", "dummy 4", 1));
    create_container(c, COMMAND, create_command("cmd 3", "cmd desc 3", "dummy 5", 1));

    // test pointer & memory equality
    TEST_ASSERT_NOT_EQUAL(next_command, c->next->command);
    TEST_ASSERT_NOT_EQUAL(target_command, remove_container(c, target_command)->next->command);
    TEST_ASSERT_EQUAL_PTR(next_command, c->next->command);
    TEST_ASSERT_EQUAL_MEMORY(next_command, c->next->command, sizeof(next_command));
}

void test_remove_container_works_for_type_text(void)
{
    char *text3 = strdup("text 3");
    char *text5 = strdup("text 5");

    c = create_container(NULL, TEXT, strdup("text 1"));
    create_container(c, TEXT, strdup("text 2"));
    create_container(c, TEXT, text3);
    create_container(c, TEXT, strdup("text 4"));
    create_container(c, TEXT, text5);

    // test pointer & memory equality
    TEST_ASSERT_NOT_EQUAL(text5, c->next->next->next->text);
    TEST_ASSERT_NOT_EQUAL(text3, remove_container(c, text3)->next->text);
    TEST_ASSERT_EQUAL_PTR(text5, c->next->next->next->text);
    TEST_ASSERT_EQUAL_MEMORY(text5, c->next->next->next->text, 7 * sizeof(char));
}

void test_remove_container_doesnt_set_first_implicitly(void)
{
    struct room *target_room = create_room("room 1", "room desc 1");
    struct container *local = create_container(NULL, ROOM, target_room);
    remove_container(local, target_room);

    TEST_ASSERT_NOT_NULL(local);
}

void test_remove_container_removes_first_entry(void)
{
    struct room *target_room = create_room("room 1", "room desc 1");
    struct room *next_room = create_room("room 2", "room desc 2");

    c = create_container(NULL, ROOM, target_room);
    create_container(c, ROOM, next_room);
    create_container(NULL, ROOM, create_room("room 3", "room desc 3"));
    create_container(c, ROOM, create_room("room 4", "room desc 4"));
    create_container(c, ROOM, create_room("room 5", "room desc 5"));

    TEST_ASSERT_EQUAL_PTR(target_room, c->room);
    TEST_ASSERT_EQUAL_PTR(next_room, c->next->room);
    TEST_ASSERT_EQUAL_MEMORY(target_room, c->room, sizeof(target_room));
    TEST_ASSERT_EQUAL_MEMORY(next_room, c->next->room, sizeof(next_room));
    c = remove_container(c, target_room);
    TEST_ASSERT_NOT_EQUAL(target_room, c->room);
    TEST_ASSERT_EQUAL_PTR(next_room, c->room);
    TEST_ASSERT_EQUAL_MEMORY(next_room, c->room, sizeof(next_room));
}

void test_remove_container_returns_null_on_last_entry(void)
{
    struct room *target_room = create_room("room 1", "room desc 1");
    struct container *local = create_container(NULL, ROOM, target_room);

    TEST_ASSERT_NULL(remove_container(local, target_room));
}

void test_remove_container_content_of_container_stays(void)
{
    struct room *target_room = create_room("room 1", "room desc 1");
    struct room *before = target_room;
    struct container *local = create_container(NULL, ROOM, target_room);
    TEST_ASSERT_EQUAL(before, local->room);
    remove_container(local, target_room);
    TEST_ASSERT_EQUAL(before, target_room);
    TEST_ASSERT_EQUAL_MEMORY(before, target_room, sizeof(target_room));
}

void test_remove_container_returns_null_on_null_first(void)
{
    TEST_ASSERT_NULL(remove_container(NULL, "text"));
}

void test_remove_container_returns_first_on_null_entry(void)
{
    c = create_container(NULL, ROOM, create_room("room 1", "room desc 1"));
    TEST_ASSERT_EQUAL_PTR(c, remove_container(c, NULL));
}
