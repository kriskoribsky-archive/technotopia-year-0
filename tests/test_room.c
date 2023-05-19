#include "unity/src/unity.h"

#include "room.h"

void setUp(void);
void tearDown(void);

void test_create_room_handles_null_name(void);
void test_create_room_handles_null_desc(void);
void test_create_room_handles_empty_name(void);
void test_create_room_handles_empty_desc(void);

void test_set_exists_from_room_handles_null_room(void);

void test_get_item_from_room_handles_null_room(void);
void test_get_item_from_room_handles_empty_name(void);

struct container *c = NULL;

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_create_room_handles_null_name);
    RUN_TEST(test_create_room_handles_null_desc);
    RUN_TEST(test_create_room_handles_empty_name);
    RUN_TEST(test_create_room_handles_empty_desc);

    RUN_TEST(test_set_exists_from_room_handles_null_room);

    RUN_TEST(test_get_item_from_room_handles_null_room);
    RUN_TEST(test_get_item_from_room_handles_empty_name);

    return UNITY_END();
}

void setUp(void)
{
}

void tearDown(void)
{
}

void test_create_room_handles_null_name(void)
{
    TEST_ASSERT_NULL(create_room(NULL, "desc"));
}
void test_create_room_handles_null_desc(void)
{
    TEST_ASSERT_NULL(create_room("name", NULL));
}
void test_create_room_handles_empty_name(void)
{
    TEST_ASSERT_NULL(create_room("", "desc"));
}
void test_create_room_handles_empty_desc(void)
{
    TEST_ASSERT_NULL(create_room("name", ""));
}

void test_set_exists_from_room_handles_null_room(void)
{
    set_exits_from_room(NULL, NULL, NULL, NULL, NULL); // doesn't segfault
    TEST_ASSERT_TRUE(true);
}

void test_get_item_from_room_handles_null_room(void)
{
    TEST_ASSERT_NULL(get_item_from_room(NULL, "item"));
}

void test_get_item_from_room_handles_empty_name(void)
{
    TEST_ASSERT_NULL(get_item_from_room(create_room("name", "desc"), ""));
}