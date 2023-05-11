#include "../unity/src/unity.h"

#include "container.h"

void setUp(void);
void tearDown(void);

void test_destroy_containers_sets_pointer_to_null(void);

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

void test_destroy_containers_sets_pointer_to_null(void)
{
    struct container *new = create_container(NULL, TEXT, "test");

    new = destroy_containers(new);

    TEST_ASSERT_EQUAL(NULL, new);
}