#include "unity/src/unity.h"

#include "backpack.h"

void setUp(void);
void tearDown(void);

void test_add_item_to_backpack_handles_null_backpack(void);
void test_add_item_to_backpack_handles_null_item(void);
void test_add_item_to_backpack_adds_item_1(void);
void test_add_item_to_backpack_adds_item_2(void);
void test_add_item_to_backpack_adds_item_3(void);
void test_add_item_to_backpack_adds_item_4(void);
void test_add_item_to_backpack_adds_item_5(void);
void test_add_item_to_backpack_adds_item_6(void);
void test_add_item_to_backpack_adds_item_7(void);

void test_get_item_from_backpack_handles_null_backpack(void);
void test_get_item_from_backpack_handles_null_name(void);

struct backpack *b = NULL;

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_add_item_to_backpack_handles_null_backpack);
    RUN_TEST(test_add_item_to_backpack_handles_null_item);

    RUN_TEST(test_add_item_to_backpack_adds_item_1);
    RUN_TEST(test_add_item_to_backpack_adds_item_2);
    RUN_TEST(test_add_item_to_backpack_adds_item_3);
    RUN_TEST(test_add_item_to_backpack_adds_item_4);
    RUN_TEST(test_add_item_to_backpack_adds_item_5);
    RUN_TEST(test_add_item_to_backpack_adds_item_6);
    RUN_TEST(test_add_item_to_backpack_adds_item_7);

    RUN_TEST(test_get_item_from_backpack_handles_null_backpack);
    RUN_TEST(test_get_item_from_backpack_handles_null_name);

    return UNITY_END();
}

void setUp(void)
{
    b = create_backpack(5);
}

void tearDown(void)
{
    b = destroy_backpack(b);
}

void test_add_item_to_backpack_handles_null_backpack(void)
{
    add_item_to_backpack(NULL, create_item("test item", "item desc", 0));
    TEST_ASSERT_NULL(b->items);
}

void test_add_item_to_backpack_handles_null_item(void)
{
    add_item_to_backpack(b, NULL);
    TEST_ASSERT_NULL(b->items);
}

void test_add_item_to_backpack_adds_item_1(void)
{
    struct backpack *backpack = create_backpack(3);
    struct item *item = create_item("STOLICKA", "Drevena stolicka, mozno posluzi ako podpera.", MOVABLE);
    add_item_to_backpack(backpack, item);
    struct item *backpack_item = get_item_from_backpack(backpack, item->name);
    TEST_ASSERT_NOT_NULL(backpack_item);
}
void test_add_item_to_backpack_adds_item_2(void)
{
    struct backpack *backpack = create_backpack(4);
    struct item *item = create_item("TLACIDLO", "Zatlac ho a mozno sa otvori tajna skrysa. Alebo ta zabiju sipy.", MOVABLE | USABLE);
    add_item_to_backpack(backpack, item);
    struct item *backpack_item = get_item_from_backpack(backpack, item->name);
    TEST_ASSERT_NOT_NULL(backpack_item);
}

void test_add_item_to_backpack_adds_item_3(void)
{
    struct backpack *backpack = create_backpack(6);
    struct item *item = create_item("LANO", "Pouzitelne na dostanie sa von z hradu.", MOVABLE | USABLE);
    add_item_to_backpack(backpack, item);
    struct item *backpack_item = get_item_from_backpack(backpack, item->name);
    TEST_ASSERT_NOT_NULL(backpack_item);
}
void test_add_item_to_backpack_adds_item_4(void)
{
    struct backpack *backpack = create_backpack(2);
    struct item *item = create_item("KLUC", "Touto vecou sa odomykaju ine veci.", MOVABLE | USABLE | EXAMINABLE);
    add_item_to_backpack(backpack, item);
    struct item *backpack_item = get_item_from_backpack(backpack, item->name);
    TEST_ASSERT_NOT_NULL(backpack_item);
}

void test_add_item_to_backpack_adds_item_5(void)
{
    struct backpack *backpack = create_backpack(4);
    struct item *item = create_item("NEBO", "Ano, stale je tam hore.", MOVABLE);
    add_item_to_backpack(backpack, item);
    struct item *backpack_item = get_item_from_backpack(backpack, item->name);
    TEST_ASSERT_NOT_NULL(backpack_item);
}

void test_add_item_to_backpack_adds_item_6(void)
{
    struct backpack *backpack = create_backpack(4);
    struct item *item = create_item("SOCHA", "Vyobrazenie Bieleho plamena, ktory tancuje na mohylach svojich nepriatelov.", MOVABLE);
    add_item_to_backpack(backpack, item);
    struct item *backpack_item = get_item_from_backpack(backpack, item->name);
    TEST_ASSERT_NOT_NULL(backpack_item);
}

void test_add_item_to_backpack_adds_item_7(void)
{
    struct backpack *backpack = create_backpack(6);
    struct item *item = create_item("MEC", "Klasicky bojovy nastroj.", MOVABLE | USABLE | EXAMINABLE);
    add_item_to_backpack(backpack, item);
    struct item *backpack_item = get_item_from_backpack(backpack, item->name);
    TEST_ASSERT_NOT_NULL(backpack_item);
}

void test_get_item_from_backpack_handles_null_backpack(void)
{
    TEST_ASSERT_NULL(get_item_from_backpack(NULL, "test item"));
}

void test_get_item_from_backpack_handles_null_name(void)
{
    TEST_ASSERT_NULL(get_item_from_backpack(b, NULL));
}