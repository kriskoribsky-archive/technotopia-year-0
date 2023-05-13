#include <assert.h>
#include <stdlib.h>

#include "backpack.h"

#ifdef DEBUG
#include <assert.h>
#define ASSERT(expr) assert(expr)
#else
#define ASSERT(expr) ((void)0)
#endif

#define MALLOC(nmemb, ptr)                          \
    {                                               \
        (ptr) = malloc((nmemb) * (sizeof(*(ptr)))); \
        assert((ptr) != NULL);                      \
    }

#define FREE(ptr)     \
    {                 \
        free(ptr);    \
        (ptr) = NULL; \
    }

#define CHECK_NULL(ptr)    \
    {                      \
        if ((ptr) == NULL) \
        {                  \
            return NULL;   \
        }                  \
    }

#define CHECK_NULL_FALSE(ptr) \
    {                         \
        if ((ptr) == NULL)    \
        {                     \
            return false;     \
        }                     \
    }

#define CHECK_NULL_VOID(ptr) \
    {                        \
        if ((ptr) == NULL)   \
        {                    \
            return;          \
        }                    \
    }

#define CHECK_EMPTY(s)    \
    {                     \
        if ((*s) == '\0') \
        {                 \
            return NULL;  \
        }                 \
    }

struct backpack *create_backpack(const int capacity)
{
    struct backpack *new;
    MALLOC(1, new);

    new->capacity = capacity;
    new->size = 0;
    new->items = NULL;

    return new;
}

struct backpack *destroy_backpack(struct backpack *backpack)
{
    CHECK_NULL(backpack);

    backpack->items = destroy_containers(backpack->items);
    FREE(backpack);

    return backpack;
}

bool add_item_to_backpack(struct backpack *backpack, struct item *item)
{
    ASSERT(backpack != NULL && item != NULL);
    CHECK_NULL_FALSE(backpack);
    CHECK_NULL_FALSE(item);

    if ((item->properties & MOVABLE) && (backpack->size + 1 <= backpack->capacity))
    {
        backpack->items = create_container(backpack->items, ITEM, item);
        backpack->size++;
        return true;
    }

    return false;
}

void delete_item_from_backpack(struct backpack *backpack, struct item *item)
{
    ASSERT(backpack != NULL && item != NULL);
    CHECK_NULL_VOID(backpack);
    CHECK_NULL_VOID(item);

    backpack->items = remove_container(backpack->items, item);
    backpack->size--;
}

struct item *get_item_from_backpack(const struct backpack *backpack, char *name)
{
    ASSERT(backpack != NULL && name != NULL);
    CHECK_NULL(backpack);
    CHECK_NULL(name);

    return get_from_container_by_name(backpack->items, name);
}
