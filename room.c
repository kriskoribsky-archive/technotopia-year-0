#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "room.h"

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

struct room *create_room(char *name, char *description)
{
    CHECK_NULL(name);
    CHECK_NULL(description);

    struct room *new;
    MALLOC(1, new);

    new->name = strdup(name);
    new->description = strdup(description);
    new->north = new->south = new->east = new->west = NULL;
    new->items = NULL;

    ASSERT(new->name != NULL &&new->description != NULL);
    return new;
}

struct room *destroy_room(struct room *room)
{
    CHECK_NULL(room);

    FREE(room->name);
    FREE(room->description);
    room->items = destroy_containers(room->items);
    FREE(room);

    return NULL;
}

void set_exits_from_room(struct room *room, struct room *north, struct room *south, struct room *east, struct room *west)
{
    ASSERT(room != NULL);

    room->north = north;
    room->south = south;
    room->east = east;
    room->west = west;
}

void show_room(const struct room *room)
{
    ASSERT(room != NULL);

    // intro
    printf("Miestnosť:\n");
    printf("%s\n", room->name);
    printf("------------------------------------\n");

    printf("\n");
    printf("%s\n", room->description);

    // items
    if (room->items)
    {
        printf("Predmety v miestnosti:\n");

        for (struct container *current = room->items; current != NULL; current = current->next)
        {
            printf("%s\n", current->item->name);
        }
    }

    // exits
    printf("Možné východy z miestnosti:\n");
    if (room->north)
    {
        printf("Sever -> %s\n", room->north->name);
    }
    if (room->south)
    {
        printf("Juh -> %s\n", room->south->name);
    }
    if (room->east)
    {
        printf("Východ -> %s\n", room->east->name);
    }
    if (room->west)
    {
        printf("Západ -> %s\n", room->west->name);
    }
}

void delete_item_from_room(struct room *room, struct item *item)
{
    ASSERT(room != NULL && item != NULL);
    remove_container(room->items, item);
}

void add_item_to_room(struct room *room, struct item *item)
{
    ASSERT(room != NULL && item != NULL);
    create_container(room->items, ITEM, item);
}

struct item *get_item_from_room(const struct room *room, const char *name)
{
    ASSERT(room != NULL && name != NULL);
    return get_from_container_by_name(room->items, name);
}