#define _POSIX_C_SOURCE 200809L // strdup
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "helpers.h"
#include "container.h"

struct container *create_container(struct container *first, enum container_type type, void *entry)
{
    CHECK_NULL(entry);

    ASSERT(first == NULL || first->type == type);
    if (first != NULL && first->type != type)
    {
        return NULL;
    }

    struct container *new;
    MALLOC(1, new);

    new->type = type;
    new->next = NULL;

    switch (type)
    {
    case ROOM:
        new->room = (struct room *)entry;
        break;

    case ITEM:
        new->item = (struct item *)entry;
        break;

    case COMMAND:
        new->command = (struct command *)entry;
        break;

    case TEXT:
        new->text = (char *)entry; // container's responsibility isn't allocating memory
        break;
    }

    // append
    struct container **next;
    for (next = &first; *next != NULL; next = &(*next)->next)
        ;

    return *next = new;
}

struct container *destroy_containers(struct container *first)
{
    CHECK_NULL(first);

    switch (first->type)
    {
    case ROOM:
        first->room = destroy_room(first->room);
        break;

    case ITEM:
        first->item = destroy_item(first->item);
        break;

    case COMMAND:
        first->command = destroy_command(first->command);
        break;

    case TEXT:
        FREE(first->text);
        break;
    }
    first->next = destroy_containers(first->next);
    FREE(first);

    return first;
}

void *get_from_container_by_name(struct container *first, const char *name)
{
    CHECK_NULL(first);
    CHECK_NULL(name);

    // find
    const char *entry_name;
    void *entry;
    switch (first->type)
    {
    case ROOM:
        entry_name = first->room->name;
        entry = (struct room *)first->room;
        break;

    case ITEM:
        entry_name = first->item->name;
        entry = (struct item *)first->item;
        break;

    case COMMAND:
        entry_name = first->command->name;
        entry = (struct command *)first->command;
        break;

    case TEXT:
        entry_name = first->text;
        entry = (char *)first->text;
        break;
    }

    // compare
    return strcasecmp(name, entry_name) == 0 ? entry : get_from_container_by_name(first->next, name);
}

struct container *remove_container(struct container *first, void *entry)
{
    CHECK_NULL(first);
    if (entry == NULL)
    {
        return first;
    }

    // find and remove container from linked container list
    for (struct container **next = &first; *next != NULL; next = &(*next)->next)
    {
        bool identical = false;
        switch ((*next)->type)
        {
        case ROOM:
            identical = (*next)->room == (struct room *)entry;
            break;

        case ITEM:
            identical = (*next)->item == (struct item *)entry;
            break;

        case COMMAND:
            identical = (*next)->command == (struct command *)entry;
            break;

        case TEXT:
            identical = (*next)->text == (char *)entry;
            break;
        }

        if (identical)
        {
            struct container *tmp = *next;
            *next = (*next)->next;
            FREE(tmp);
            break;
        }
    }

    return first; // already shifted if first element removed
}
