#include <assert.h>
#include <stdlib.h>
#include <strings.h>

#include "world.h"

#ifdef DEBUG
#include <assert.h>
#define ASSERT(expr) assert(expr)
#else
#define ASSERT(expr) ((void)0)
#endif

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

struct container *create_world()
{
    // TODO complete game scenario with populating world, rooms & items

    struct room *home = create_room("home",
                                    "Nachadzas sa v chyzi svarneho suhaja. Na vychode sa nachadzaju dvere veduce z chyze von.");
    struct room *garden = create_room("garden",
                                      "Stojis pred chyzou a rozoznavas zahradku, ktora je znacne neudrziavana. este ze husty lesik na severe v porovnani so zahradkou nicim nevynika.");

    set_exits_from_room(home, NULL, NULL, garden, NULL);
    set_exits_from_room(garden, NULL, NULL, NULL, home);

    struct container *world = add_room_to_world(NULL, home);
    add_room_to_world(world, garden);

    // TODO place items
    struct item *key = create_item("key", "Kluc od domu.", MOVABLE | EXAMINABLE | USABLE);

    add_item_to_room(get_room(world, "home"), key);

    return world;
}

struct container *add_room_to_world(struct container *world, struct room *room)
{
    CHECK_NULL(room);

    // check for duplicate room
    for (struct container *current = world; current != NULL; current = current->next)
    {
        ASSERT(current->type == ROOM);
        if (strcasecmp(current->room->name, room->name) == 0)
        {
            return NULL;
        }
    }

    // add room
    return create_container(world, ROOM, room);
}

struct container *destroy_world(struct container *world)
{
    ASSERT(world != NULL);
    return destroy_containers(world);
}

struct room *get_room(struct container *world, char *name)
{
    ASSERT(world != NULL && name != NULL);
    return get_from_container_by_name(world, name);
}