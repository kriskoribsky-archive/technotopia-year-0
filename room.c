#define _POSIX_C_SOURCE 200809L // strdup
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"
#include "room.h"

struct room *create_room(char *name, char *description)
{
    CHECK_NULL(name);
    CHECK_NULL(description);
    CHECK_EMPTY(name);
    CHECK_EMPTY(description);

    struct room *new;
    MALLOC(1, new);

    new->name = strdup(name);
    new->description = strdup(description);
    new->north = new->south = new->east = new->west = NULL;
    new->items = NULL;

    assert(new->name != NULL);
    assert(new->description != NULL);
    return new;
}

struct room *destroy_room(struct room *room)
{
    CHECK_NULL(room);

    FREE(room->name);
    FREE(room->description);
    room->items = destroy_containers(room->items);
    FREE(room);

    return room;
}

void set_exits_from_room(struct room *room, struct room *north, struct room *south, struct room *east, struct room *west)
{
    ASSERT(room != NULL);
    CHECK_NULL_VOID(room);

    room->north = north;
    room->south = south;
    room->east = east;
    room->west = west;
}

void show_room(const struct room *room)
{
    ASSERT(room != NULL);
    CHECK_NULL_VOID(room);

    printf("%s\n", room->name);
    printf("%s\n", room->description);
}

void delete_item_from_room(struct room *room, struct item *item)
{
    ASSERT(room != NULL && item != NULL);
    CHECK_NULL_VOID(room);
    CHECK_NULL_VOID(item);

    room->items = remove_container(room->items, item);
}

void add_item_to_room(struct room *room, struct item *item)
{
    ASSERT(room != NULL && item != NULL);
    CHECK_NULL_VOID(room);
    CHECK_NULL_VOID(item);

    struct container *c = create_container(room->items, ITEM, item);
    room->items = room->items != NULL ? room->items : c;
}

struct item *get_item_from_room(const struct room *room, const char *name)
{
    ASSERT(room != NULL && name != NULL);
    CHECK_NULL(room);
    CHECK_NULL(name);

    return get_from_container_by_name(room->items, name);
}