#define _POSIX_C_SOURCE 200809L // strdup
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"
#include "item.h"

struct item *create_item(char *name, char *description, unsigned int properties)
{
    ASSERT(name != NULL);
    ASSERT(description != NULL);
    CHECK_NULL(name);
    CHECK_NULL(description);
    CHECK_EMPTY(name);
    CHECK_EMPTY(description);

    struct item *new;
    MALLOC(1, new);

    new->name = strdup(name);
    new->description = strdup(description);
    new->properties = properties;

    assert(new->name != NULL);
    assert(new->description != NULL);
    return new;
}

struct item *destroy_item(struct item *item)
{
    ASSERT(item != NULL);
    CHECK_NULL(item);

    FREE(item->name);
    FREE(item->description);
    FREE(item);

    return item;
}